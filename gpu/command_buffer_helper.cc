// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "media/gpu/command_buffer_helper.h"

#include <utility>
#include <vector>

#include "base/logging.h"
#include "base/single_thread_task_runner.h"
#include "base/threading/thread_checker.h"
#include "build/build_config.h"
#include "gpu/command_buffer/common/scheduling_priority.h"
#include "gpu/command_buffer/service/decoder_context.h"
#include "gpu/command_buffer/service/scheduler.h"
#include "gpu/command_buffer/service/shared_image_backing.h"
#include "gpu/command_buffer/service/shared_image_representation.h"
#include "gpu/command_buffer/service/sync_point_manager.h"
#include "gpu/ipc/service/command_buffer_stub.h"
#include "gpu/ipc/service/gpu_channel.h"
#include "gpu/ipc/service/gpu_channel_manager.h"
#include "media/gpu/gles2_decoder_helper.h"
#include "ui/gl/gl_context.h"

namespace media {

namespace {

class CommandBufferHelperImpl
    : public CommandBufferHelper,
      public gpu::CommandBufferStub::DestructionObserver {
 public:
  explicit CommandBufferHelperImpl(gpu::CommandBufferStub* stub)
      : CommandBufferHelper(stub->channel()->task_runner()),
        stub_(stub),
        memory_tracker_(this),
        memory_type_tracker_(&memory_tracker_) {
    DVLOG(1) << __func__;
    DCHECK(stub_->channel()->task_runner()->BelongsToCurrentThread());

    stub_->AddDestructionObserver(this);
    wait_sequence_id_ = stub_->channel()->scheduler()->CreateSequence(
#if defined(OS_MAC)
        // Workaround for crbug.com/1035750.
        // TODO(sandersd): Investigate whether there is a deeper scheduling
        // problem that can be resolved.
        gpu::SchedulingPriority::kHigh
#else
        gpu::SchedulingPriority::kNormal
#endif  // defined(OS_MAC)
        ,
        stub_->channel()->task_runner());
    decoder_helper_ = GLES2DecoderHelper::Create(stub_->decoder_context());
  }

  gl::GLContext* GetGLContext() override {
    DVLOG(2) << __func__;
    DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

    if (!decoder_helper_)
      return nullptr;

    return decoder_helper_->GetGLContext();
  }

  gpu::SharedImageStub* GetSharedImageStub() override {
    return shared_image_stub();
  }

  // Const variant of above method for internal callers.
  gpu::SharedImageStub* shared_image_stub() const {
    if (!stub_)
      return nullptr;
    return stub_->channel()->shared_image_stub();
  }

  bool HasStub() override {
    DVLOG(4) << __func__;
    DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

    return stub_;
  }

  bool MakeContextCurrent() override {
    DVLOG(2) << __func__;
    DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

    return decoder_helper_ && decoder_helper_->MakeContextCurrent();
  }

  std::unique_ptr<gpu::SharedImageRepresentationFactoryRef> Register(
      std::unique_ptr<gpu::SharedImageBacking> backing) override {
    DVLOG(2) << __func__;
    DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
    return stub_->channel()
        ->gpu_channel_manager()
        ->shared_image_manager()
        ->Register(std::move(backing), &memory_type_tracker_);
  }

  gpu::TextureBase* GetTexture(GLuint service_id) const override {
    DVLOG(2) << __func__ << "(" << service_id << ")";
    DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
    DCHECK(stub_->decoder_context()->GetGLContext()->IsCurrent(nullptr));
    DCHECK(textures_.count(service_id));
    return textures_.at(service_id)->GetTextureBase();
  }

  GLuint CreateTexture(GLenum target,
                       GLenum internal_format,
                       GLsizei width,
                       GLsizei height,
                       GLenum format,
                       GLenum type) override {
    DVLOG(2) << __func__;
    DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
    DCHECK(stub_->decoder_context()->GetGLContext()->IsCurrent(nullptr));

    std::unique_ptr<gpu::gles2::AbstractTexture> texture =
        decoder_helper_->CreateTexture(target, internal_format, width, height,
                                       format, type);
    GLuint service_id = texture->service_id();
    textures_[service_id] = std::move(texture);
    return service_id;
  }

  void DestroyTexture(GLuint service_id) override {
    DVLOG(2) << __func__ << "(" << service_id << ")";
    DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);
    DCHECK(stub_->decoder_context()->GetGLContext()->IsCurrent(nullptr));
    DCHECK(textures_.count(service_id));

    textures_.erase(service_id);
  }

  void SetCleared(GLuint service_id) override {
    DVLOG(2) << __func__ << "(" << service_id << ")";
    DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

    DCHECK(textures_.count(service_id));
    textures_[service_id]->SetCleared();
  }

  bool BindImage(GLuint service_id,
                 gl::GLImage* image,
                 bool client_managed) override {
    DVLOG(2) << __func__ << "(" << service_id << ")";
    DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

    DCHECK(textures_.count(service_id));
    textures_[service_id]->BindImage(image, client_managed);
    return true;
  }

  gpu::Mailbox CreateMailbox(GLuint service_id) override {
    DVLOG(2) << __func__ << "(" << service_id << ")";
    DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

    if (!decoder_helper_)
      return gpu::Mailbox();

    DCHECK(textures_.count(service_id));
    return decoder_helper_->CreateMailbox(textures_[service_id].get());
  }

  void ProduceTexture(const gpu::Mailbox& mailbox, GLuint service_id) override {
    DVLOG(2) << __func__ << "(" << mailbox.ToDebugString() << ", " << service_id
             << ")";
    DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

    if (!decoder_helper_)
      return;

    DCHECK(textures_.count(service_id));
    return decoder_helper_->ProduceTexture(mailbox,
                                           textures_[service_id].get());
  }

  void WaitForSyncToken(gpu::SyncToken sync_token,
                        base::OnceClosure done_cb) override {
    DVLOG(2) << __func__;
    DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

    if (!stub_)
      return;

    // TODO(sandersd): Do we need to keep a ref to |this| while there are
    // pending waits? If we destruct while they are pending, they will never
    // run.
    stub_->channel()->scheduler()->ScheduleTask(
        gpu::Scheduler::Task(wait_sequence_id_, std::move(done_cb),
                             std::vector<gpu::SyncToken>({sync_token})));
  }

  void SetWillDestroyStubCB(WillDestroyStubCB will_destroy_stub_cb) override {
    DCHECK(!will_destroy_stub_cb_);
    will_destroy_stub_cb_ = std::move(will_destroy_stub_cb);
  }

  bool IsPassthrough() const override {
    if (!stub_)
      return false;
    return stub_->decoder_context()
        ->GetFeatureInfo()
        ->is_passthrough_cmd_decoder();
  }

  bool SupportsTextureRectangle() const override {
    if (!stub_)
      return false;
    return stub_->decoder_context()
        ->GetFeatureInfo()
        ->feature_flags()
        .arb_texture_rectangle;
  }

 private:
  // Helper class to forward memory tracking calls to shared image stub.
  // Necessary because the underlying stub and channel can get destroyed before
  // the CommandBufferHelper and its clients.
  class MemoryTrackerImpl : public gpu::MemoryTracker {
   public:
    explicit MemoryTrackerImpl(CommandBufferHelperImpl* helper)
        : helper_(helper) {
      if (auto* stub = helper_->shared_image_stub()) {
        // We assume these don't change after initialization.
        client_id_ = stub->ClientId();
        client_tracing_id_ = stub->ClientTracingId();
        context_group_tracing_id_ = stub->ContextGroupTracingId();
      }
    }
    ~MemoryTrackerImpl() override = default;

    MemoryTrackerImpl(const MemoryTrackerImpl&) = delete;
    MemoryTrackerImpl& operator=(const MemoryTrackerImpl&) = delete;

    void TrackMemoryAllocatedChange(int64_t delta) override {
      if (auto* stub = helper_->shared_image_stub())
        stub->TrackMemoryAllocatedChange(delta);
    }

    uint64_t GetSize() const override {
      if (auto* stub = helper_->shared_image_stub())
        return stub->GetSize();
      return 0;
    }

    int ClientId() const override { return client_id_; }

    uint64_t ClientTracingId() const override { return client_tracing_id_; }

    uint64_t ContextGroupTracingId() const override {
      return context_group_tracing_id_;
    }

   private:
    CommandBufferHelperImpl* const helper_;
    int client_id_ = 0;
    uint64_t client_tracing_id_ = 0;
    uint64_t context_group_tracing_id_ = 0;
  };

  ~CommandBufferHelperImpl() override {
    DVLOG(1) << __func__;
    DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

    if (stub_)
      DestroyStub();
  }

  void OnWillDestroyStub(bool have_context) override {
    DVLOG(1) << __func__;
    DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

    // In case |will_destroy_stub_cb_| drops the last reference to |this|, make
    // sure that we're around a bit longer.
    scoped_refptr<CommandBufferHelper> thiz(this);

    if (will_destroy_stub_cb_)
      std::move(will_destroy_stub_cb_).Run(have_context);

    DestroyStub();
  }

  void DestroyStub() {
    DVLOG(3) << __func__;
    DCHECK_CALLED_ON_VALID_THREAD(thread_checker_);

    decoder_helper_ = nullptr;

    // If the last reference to |this| is in a |done_cb|, destroying the wait
    // sequence can delete |this|. Clearing |stub_| first prevents DestroyStub()
    // being called twice.
    gpu::CommandBufferStub* stub = stub_;
    stub_ = nullptr;

    stub->RemoveDestructionObserver(this);
    stub->channel()->scheduler()->DestroySequence(wait_sequence_id_);
  }

  gpu::CommandBufferStub* stub_;
  // Wait tasks are scheduled on our own sequence so that we can't inadvertently
  // block the command buffer.
  gpu::SequenceId wait_sequence_id_;
  // TODO(sandersd): Merge GLES2DecoderHelper implementation into this class.
  std::unique_ptr<GLES2DecoderHelper> decoder_helper_;
  std::map<GLuint, std::unique_ptr<gpu::gles2::AbstractTexture>> textures_;

  WillDestroyStubCB will_destroy_stub_cb_;

  MemoryTrackerImpl memory_tracker_;
  gpu::MemoryTypeTracker memory_type_tracker_;

  THREAD_CHECKER(thread_checker_);
  DISALLOW_COPY_AND_ASSIGN(CommandBufferHelperImpl);
};

}  // namespace

CommandBufferHelper::CommandBufferHelper(
    scoped_refptr<base::SequencedTaskRunner> task_runner)
    : base::RefCountedDeleteOnSequence<CommandBufferHelper>(
          std::move(task_runner)) {}

// static
scoped_refptr<CommandBufferHelper> CommandBufferHelper::Create(
    gpu::CommandBufferStub* stub) {
  return base::MakeRefCounted<CommandBufferHelperImpl>(stub);
}

}  // namespace media
