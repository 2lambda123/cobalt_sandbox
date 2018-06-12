# Copyright 2016 Google Inc. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
{
  'includes': [
    '<(DEPTH)/starboard/shared/starboard/player/filter/player_filter.gypi',
  ],
  'targets': [
    {
      'target_name': 'starboard_base_symbolize',
      'type': 'static_library',
      'sources': [
        '<(DEPTH)/base/third_party/symbolize/demangle.cc',
        '<(DEPTH)/base/third_party/symbolize/symbolize.cc',
      ],
    },
    # Copy sources that we compile from the NDK so that we can reference them
    # by a relative path. Otherwise, without GYP pathname relativization
    # different configuration builds would clobber each other since they'd all
    # generate their .o at the same path in the NDK sources directory.
    {
      'target_name': 'ndk_sources',
      'type': 'none',
      'copies': [{
        'destination': '<(SHARED_INTERMEDIATE_DIR)/ndk-sources/',
        'files': [
          '<(NDK_HOME)/sources/android/cpufeatures/cpu-features.c',
        ],
      }],
    },
    {
      'target_name': 'starboard_platform',
      'type': 'static_library',
      'sources': [
        '<@(filter_based_player_sources)',
        'accessibility_get_caption_settings.cc',
        'accessibility_get_display_settings.cc',
        'accessibility_get_text_to_speech_settings.cc',
        'accessibility_set_captions_enabled.cc',
        'android_main.cc',
        'application_android.cc',
        'application_android.h',
        'atomic_public.h',
        'audio_decoder.cc',
        'audio_decoder.h',
        'audio_renderer.h',
        'audio_sink_get_max_channels.cc',
        'audio_sink_get_nearest_supported_sample_frequency.cc',
        'audio_sink_is_audio_frame_storage_type_supported.cc',
        'audio_sink_is_audio_sample_type_supported.cc',
        'audio_track_audio_sink_type.cc',
        'audio_track_audio_sink_type.h',
        'configuration_public.h',
        'decode_target_create.cc',
        'decode_target_create.h',
        'decode_target_get_info.cc',
        'decode_target_internal.cc',
        'decode_target_internal.h',
        'decode_target_release.cc',
        'directory_close.cc',
        'directory_get_next.cc',
        'directory_internal.h',
        'directory_open.cc',
        'drm_create_system.cc',
        'drm_system.cc',
        'drm_system.h',
        'egl_swap_buffers.cc',
        'file_can_open.cc',
        'file_close.cc',
        'file_delete.cc',
        'file_exists.cc',
        'file_flush.cc',
        'file_get_info.cc',
        'file_get_path_info.cc',
        'file_internal.cc',
        'file_internal.h',
        'file_open.cc',
        'file_read.cc',
        'file_seek.cc',
        'file_truncate.cc',
        'file_write.cc',
        'get_home_directory.cc',
        'input_events_filter.cc',
        'input_events_filter.h',
        'input_events_generator.cc',
        'input_events_generator.h',
        'jni_env_ext.cc',
        'jni_env_ext.h',
        'jni_utils.h',
        'log.cc',
        'log_flush.cc',
        'log_format.cc',
        'log_internal.h',
        'log_is_tty.cc',
        'log_raw.cc',
        'main.cc',
        'media_codec_bridge.cc',
        'media_codec_bridge.h',
        'media_common.h',
        'media_decoder.cc',
        'media_decoder.h',
        'media_get_audio_configuration.cc',
        'media_get_audio_output_count.cc',
        'media_is_audio_supported.cc',
        'media_is_output_protected.cc',
        'media_is_supported.cc',
        'media_is_transfer_characteristics_supported.cc',
        'media_is_video_supported.cc',
        'media_set_output_protection.cc',
        'player_components_impl.cc',
        'player_create.cc',
        'player_destroy.cc',
        'player_set_bounds.cc',
        'player_set_playback_rate.cc',
        'sanitizer_options.cc',
        'socket_get_interface_address.cc',
        'speech_recognizer_impl.cc',
        'speech_synthesis_cancel.cc',
        'speech_synthesis_speak.cc',
        'system_get_connection_type.cc',
        'system_get_device_type.cc',
        'system_get_locale_id.cc',
        'system_get_path.cc',
        'system_get_property.cc',
        'system_get_stack.cc',
        'system_has_capability.cc',
        'system_platform_error.cc',
        'system_request_stop.cc',
        'system_request_suspend.cc',
        'thread_create.cc',
        'thread_create_priority.cc',
        'thread_get_name.cc',
        'thread_types_public.h',
        'time_zone_get_dst_name.cc',
        'time_zone_get_name.cc',
        'trace_util.h',
        'video_decoder.cc',
        'video_decoder.h',
        'video_render_algorithm.cc',
        'video_render_algorithm.h',
        'video_window.cc',
        'video_window.h',
        'window_create.cc',
        'window_destroy.cc',
        'window_get_platform_handle.cc',
        'window_get_size.cc',
        'window_internal.h',
        '<(SHARED_INTERMEDIATE_DIR)/ndk-sources/cpu-features.c',
        '<(DEPTH)/starboard/accessibility.h',
        '<(DEPTH)/starboard/atomic.h',
        '<(DEPTH)/starboard/audio_sink.h',
        '<(DEPTH)/starboard/common/ref_counted.h',
        '<(DEPTH)/starboard/common/scoped_ptr.h',
        '<(DEPTH)/starboard/condition_variable.h',
        '<(DEPTH)/starboard/configuration.h',
        '<(DEPTH)/starboard/decode_target.h',
        '<(DEPTH)/starboard/directory.h',
        '<(DEPTH)/starboard/drm.h',
        '<(DEPTH)/starboard/event.h',
        '<(DEPTH)/starboard/export.h',
        '<(DEPTH)/starboard/file.h',
        '<(DEPTH)/starboard/input.h',
        '<(DEPTH)/starboard/key.h',
        '<(DEPTH)/starboard/log.h',
        '<(DEPTH)/starboard/media.h',
        '<(DEPTH)/starboard/memory.h',
        '<(DEPTH)/starboard/mutex.h',
        '<(DEPTH)/starboard/once.h',
        '<(DEPTH)/starboard/player.h',
        '<(DEPTH)/starboard/queue.h',
        '<(DEPTH)/starboard/socket.h',
        '<(DEPTH)/starboard/speech_synthesis.h',
        '<(DEPTH)/starboard/string.h',
        '<(DEPTH)/starboard/system.h',
        '<(DEPTH)/starboard/thread.h',
        '<(DEPTH)/starboard/time_zone.h',
        '<(DEPTH)/starboard/types.h',
        '<(DEPTH)/starboard/window.h',
        '<(DEPTH)/starboard/shared/dlmalloc/memory_map.cc',
        '<(DEPTH)/starboard/shared/dlmalloc/memory_protect.cc',
        '<(DEPTH)/starboard/shared/dlmalloc/memory_unmap.cc',
        '<(DEPTH)/starboard/shared/gcc/atomic_gcc_public.h',
        '<(DEPTH)/starboard/shared/gles/gl_call.h',
        '<(DEPTH)/starboard/shared/internal_only.h',
        '<(DEPTH)/starboard/shared/iso/character_is_alphanumeric.cc',
        '<(DEPTH)/starboard/shared/iso/character_is_digit.cc',
        '<(DEPTH)/starboard/shared/iso/character_is_hex_digit.cc',
        '<(DEPTH)/starboard/shared/iso/character_is_space.cc',
        '<(DEPTH)/starboard/shared/iso/character_is_upper.cc',
        '<(DEPTH)/starboard/shared/iso/character_to_lower.cc',
        '<(DEPTH)/starboard/shared/iso/character_to_upper.cc',
        '<(DEPTH)/starboard/shared/iso/double_absolute.cc',
        '<(DEPTH)/starboard/shared/iso/double_exponent.cc',
        '<(DEPTH)/starboard/shared/iso/double_floor.cc',
        '<(DEPTH)/starboard/shared/iso/double_is_finite.cc',
        '<(DEPTH)/starboard/shared/iso/double_is_nan.cc',
        '<(DEPTH)/starboard/shared/iso/impl/directory_close.h',
        '<(DEPTH)/starboard/shared/iso/impl/directory_get_next.h',
        '<(DEPTH)/starboard/shared/iso/impl/directory_open.h',
        '<(DEPTH)/starboard/shared/iso/memory_allocate_unchecked.cc',
        '<(DEPTH)/starboard/shared/iso/memory_compare.cc',
        '<(DEPTH)/starboard/shared/iso/memory_copy.cc',
        '<(DEPTH)/starboard/shared/iso/memory_find_byte.cc',
        '<(DEPTH)/starboard/shared/iso/memory_free.cc',
        '<(DEPTH)/starboard/shared/iso/memory_move.cc',
        '<(DEPTH)/starboard/shared/iso/memory_reallocate_unchecked.cc',
        '<(DEPTH)/starboard/shared/iso/memory_set.cc',
        '<(DEPTH)/starboard/shared/iso/string_compare.cc',
        '<(DEPTH)/starboard/shared/iso/string_compare_all.cc',
        '<(DEPTH)/starboard/shared/iso/string_find_character.cc',
        '<(DEPTH)/starboard/shared/iso/string_find_last_character.cc',
        '<(DEPTH)/starboard/shared/iso/string_find_string.cc',
        '<(DEPTH)/starboard/shared/iso/string_get_length.cc',
        '<(DEPTH)/starboard/shared/iso/string_get_length_wide.cc',
        '<(DEPTH)/starboard/shared/iso/string_parse_double.cc',
        '<(DEPTH)/starboard/shared/iso/string_parse_signed_integer.cc',
        '<(DEPTH)/starboard/shared/iso/string_parse_uint64.cc',
        '<(DEPTH)/starboard/shared/iso/string_parse_unsigned_integer.cc',
        '<(DEPTH)/starboard/shared/iso/string_scan.cc',
        '<(DEPTH)/starboard/shared/iso/system_binary_search.cc',
        '<(DEPTH)/starboard/shared/iso/system_sort.cc',
        '<(DEPTH)/starboard/shared/libevent/socket_waiter_add.cc',
        '<(DEPTH)/starboard/shared/libevent/socket_waiter_create.cc',
        '<(DEPTH)/starboard/shared/libevent/socket_waiter_destroy.cc',
        '<(DEPTH)/starboard/shared/libevent/socket_waiter_internal.cc',
        '<(DEPTH)/starboard/shared/libevent/socket_waiter_remove.cc',
        '<(DEPTH)/starboard/shared/libevent/socket_waiter_wait.cc',
        '<(DEPTH)/starboard/shared/libevent/socket_waiter_wait_timed.cc',
        '<(DEPTH)/starboard/shared/libevent/socket_waiter_wake_up.cc',
        '<(DEPTH)/starboard/shared/linux/byte_swap.cc',
        '<(DEPTH)/starboard/shared/linux/memory_get_stack_bounds.cc',
        '<(DEPTH)/starboard/shared/linux/page_internal.cc',
        '<(DEPTH)/starboard/shared/linux/system_get_random_data.cc',
        '<(DEPTH)/starboard/shared/linux/system_get_total_cpu_memory.cc',
        '<(DEPTH)/starboard/shared/linux/system_get_used_cpu_memory.cc',
        '<(DEPTH)/starboard/shared/linux/system_is_debugger_attached.cc',
        '<(DEPTH)/starboard/shared/linux/system_symbolize.cc',
        '<(DEPTH)/starboard/shared/linux/thread_get_id.cc',
        '<(DEPTH)/starboard/shared/linux/thread_set_name.cc',
        '<(DEPTH)/starboard/shared/nouser/user_get_current.cc',
        '<(DEPTH)/starboard/shared/nouser/user_get_property.cc',
        '<(DEPTH)/starboard/shared/nouser/user_get_signed_in.cc',
        '<(DEPTH)/starboard/shared/nouser/user_internal.cc',
        '<(DEPTH)/starboard/shared/nouser/user_internal.h',
        '<(DEPTH)/starboard/shared/posix/directory_create.cc',
        '<(DEPTH)/starboard/shared/posix/impl/file_can_open.h',
        '<(DEPTH)/starboard/shared/posix/impl/file_close.h',
        '<(DEPTH)/starboard/shared/posix/impl/file_delete.h',
        '<(DEPTH)/starboard/shared/posix/impl/file_flush.h',
        '<(DEPTH)/starboard/shared/posix/impl/file_get_info.h',
        '<(DEPTH)/starboard/shared/posix/impl/file_get_path_info.h',
        '<(DEPTH)/starboard/shared/posix/impl/file_open.h',
        '<(DEPTH)/starboard/shared/posix/impl/file_read.h',
        '<(DEPTH)/starboard/shared/posix/impl/file_seek.h',
        '<(DEPTH)/starboard/shared/posix/impl/file_truncate.h',
        '<(DEPTH)/starboard/shared/posix/impl/file_write.h',
        '<(DEPTH)/starboard/shared/posix/memory_allocate_aligned_unchecked.cc',
        '<(DEPTH)/starboard/shared/posix/memory_flush.cc',
        '<(DEPTH)/starboard/shared/posix/memory_free_aligned.cc',
        '<(DEPTH)/starboard/shared/posix/set_non_blocking_internal.cc',
        '<(DEPTH)/starboard/shared/posix/socket_accept.cc',
        '<(DEPTH)/starboard/shared/posix/socket_bind.cc',
        '<(DEPTH)/starboard/shared/posix/socket_clear_last_error.cc',
        '<(DEPTH)/starboard/shared/posix/socket_connect.cc',
        '<(DEPTH)/starboard/shared/posix/socket_create.cc',
        '<(DEPTH)/starboard/shared/posix/socket_destroy.cc',
        '<(DEPTH)/starboard/shared/posix/socket_free_resolution.cc',
        '<(DEPTH)/starboard/shared/posix/socket_get_last_error.cc',
        '<(DEPTH)/starboard/shared/posix/socket_get_local_address.cc',
        '<(DEPTH)/starboard/shared/posix/socket_internal.cc',
        '<(DEPTH)/starboard/shared/posix/socket_is_connected.cc',
        '<(DEPTH)/starboard/shared/posix/socket_is_connected_and_idle.cc',
        '<(DEPTH)/starboard/shared/posix/socket_join_multicast_group.cc',
        '<(DEPTH)/starboard/shared/posix/socket_listen.cc',
        '<(DEPTH)/starboard/shared/posix/socket_receive_from.cc',
        '<(DEPTH)/starboard/shared/posix/socket_resolve.cc',
        '<(DEPTH)/starboard/shared/posix/socket_send_to.cc',
        '<(DEPTH)/starboard/shared/posix/socket_set_broadcast.cc',
        '<(DEPTH)/starboard/shared/posix/socket_set_receive_buffer_size.cc',
        '<(DEPTH)/starboard/shared/posix/socket_set_reuse_address.cc',
        '<(DEPTH)/starboard/shared/posix/socket_set_send_buffer_size.cc',
        '<(DEPTH)/starboard/shared/posix/socket_set_tcp_keep_alive.cc',
        '<(DEPTH)/starboard/shared/posix/socket_set_tcp_no_delay.cc',
        '<(DEPTH)/starboard/shared/posix/socket_set_tcp_window_scaling.cc',
        '<(DEPTH)/starboard/shared/posix/string_compare_no_case.cc',
        '<(DEPTH)/starboard/shared/posix/string_compare_no_case_n.cc',
        '<(DEPTH)/starboard/shared/posix/string_compare_wide.cc',
        '<(DEPTH)/starboard/shared/posix/string_format.cc',
        '<(DEPTH)/starboard/shared/posix/string_format_wide.cc',
        '<(DEPTH)/starboard/shared/posix/system_break_into_debugger.cc',
        '<(DEPTH)/starboard/shared/posix/system_clear_last_error.cc',
        '<(DEPTH)/starboard/shared/posix/system_get_error_string.cc',
        '<(DEPTH)/starboard/shared/posix/system_get_last_error.cc',
        '<(DEPTH)/starboard/shared/posix/system_get_number_of_processors.cc',
        '<(DEPTH)/starboard/shared/posix/thread_sleep.cc',
        '<(DEPTH)/starboard/shared/posix/time_get_monotonic_now.cc',
        '<(DEPTH)/starboard/shared/posix/time_get_monotonic_thread_now.cc',
        '<(DEPTH)/starboard/shared/posix/time_get_now.cc',
        '<(DEPTH)/starboard/shared/posix/time_zone_get_current.cc',
        '<(DEPTH)/starboard/shared/pthread/condition_variable_broadcast.cc',
        '<(DEPTH)/starboard/shared/pthread/condition_variable_create.cc',
        '<(DEPTH)/starboard/shared/pthread/condition_variable_destroy.cc',
        '<(DEPTH)/starboard/shared/pthread/condition_variable_signal.cc',
        '<(DEPTH)/starboard/shared/pthread/condition_variable_wait.cc',
        '<(DEPTH)/starboard/shared/pthread/condition_variable_wait_timed.cc',
        '<(DEPTH)/starboard/shared/pthread/mutex_acquire.cc',
        '<(DEPTH)/starboard/shared/pthread/mutex_acquire_try.cc',
        '<(DEPTH)/starboard/shared/pthread/mutex_create.cc',
        '<(DEPTH)/starboard/shared/pthread/mutex_destroy.cc',
        '<(DEPTH)/starboard/shared/pthread/mutex_release.cc',
        '<(DEPTH)/starboard/shared/pthread/once.cc',
        '<(DEPTH)/starboard/shared/pthread/thread_create_local_key.cc',
        '<(DEPTH)/starboard/shared/pthread/thread_create_priority.h',
        '<(DEPTH)/starboard/shared/pthread/thread_destroy_local_key.cc',
        '<(DEPTH)/starboard/shared/pthread/thread_detach.cc',
        '<(DEPTH)/starboard/shared/pthread/thread_get_current.cc',
        '<(DEPTH)/starboard/shared/pthread/thread_get_local_value.cc',
        '<(DEPTH)/starboard/shared/pthread/thread_is_equal.cc',
        '<(DEPTH)/starboard/shared/pthread/thread_join.cc',
        '<(DEPTH)/starboard/shared/pthread/thread_set_local_value.cc',
        '<(DEPTH)/starboard/shared/pthread/thread_yield.cc',
        '<(DEPTH)/starboard/shared/pthread/types_public.h',
        '<(DEPTH)/starboard/shared/signal/crash_signals.h',
        '<(DEPTH)/starboard/shared/signal/crash_signals_sigaction.cc',
        '<(DEPTH)/starboard/shared/signal/suspend_signals.cc',
        '<(DEPTH)/starboard/shared/signal/suspend_signals.h',
        '<(DEPTH)/starboard/shared/starboard/application.cc',
        '<(DEPTH)/starboard/shared/starboard/application.h',
        '<(DEPTH)/starboard/shared/starboard/audio_sink/audio_sink_create.cc',
        '<(DEPTH)/starboard/shared/starboard/audio_sink/audio_sink_destroy.cc',
        '<(DEPTH)/starboard/shared/starboard/audio_sink/audio_sink_internal.cc',
        '<(DEPTH)/starboard/shared/starboard/audio_sink/audio_sink_internal.h',
        '<(DEPTH)/starboard/shared/starboard/audio_sink/audio_sink_is_valid.cc',
        '<(DEPTH)/starboard/shared/starboard/audio_sink/stub_audio_sink_type.cc',
        '<(DEPTH)/starboard/shared/starboard/audio_sink/stub_audio_sink_type.h',
        '<(DEPTH)/starboard/shared/starboard/command_line.cc',
        '<(DEPTH)/starboard/shared/starboard/command_line.h',
        '<(DEPTH)/starboard/shared/starboard/directory_can_open.cc',
        '<(DEPTH)/starboard/shared/starboard/drm/drm_close_session.cc',
        '<(DEPTH)/starboard/shared/starboard/drm/drm_destroy_system.cc',
        '<(DEPTH)/starboard/shared/starboard/drm/drm_generate_session_update_request.cc',
        '<(DEPTH)/starboard/shared/starboard/drm/drm_system_internal.h',
        '<(DEPTH)/starboard/shared/starboard/drm/drm_update_session.cc',
        '<(DEPTH)/starboard/shared/starboard/event_cancel.cc',
        '<(DEPTH)/starboard/shared/starboard/event_schedule.cc',
        '<(DEPTH)/starboard/shared/starboard/file_mode_string_to_flags.cc',
        '<(DEPTH)/starboard/shared/starboard/file_storage/storage_close_record.cc',
        '<(DEPTH)/starboard/shared/starboard/file_storage/storage_delete_record.cc',
        '<(DEPTH)/starboard/shared/starboard/file_storage/storage_get_record_size.cc',
        '<(DEPTH)/starboard/shared/starboard/file_storage/storage_open_record.cc',
        '<(DEPTH)/starboard/shared/starboard/file_storage/storage_read_record.cc',
        '<(DEPTH)/starboard/shared/starboard/file_storage/storage_write_record.cc',
        '<(DEPTH)/starboard/shared/starboard/log_message.cc',
        '<(DEPTH)/starboard/shared/starboard/log_raw_dump_stack.cc',
        '<(DEPTH)/starboard/shared/starboard/log_raw_format.cc',
        '<(DEPTH)/starboard/shared/starboard/media/codec_util.cc',
        '<(DEPTH)/starboard/shared/starboard/media/codec_util.h',
        '<(DEPTH)/starboard/shared/starboard/media/media_can_play_mime_and_key_system.cc',
        '<(DEPTH)/starboard/shared/starboard/media/media_util.cc',
        '<(DEPTH)/starboard/shared/starboard/media/media_util.h',
        '<(DEPTH)/starboard/shared/starboard/media/mime_type.cc',
        '<(DEPTH)/starboard/shared/starboard/media/mime_type.h',
        '<(DEPTH)/starboard/shared/starboard/new.cc',
        '<(DEPTH)/starboard/shared/starboard/player/decoded_audio_internal.cc',
        '<(DEPTH)/starboard/shared/starboard/player/decoded_audio_internal.h',
        '<(DEPTH)/starboard/shared/starboard/player/input_buffer_internal.cc',
        '<(DEPTH)/starboard/shared/starboard/player/input_buffer_internal.h',
        '<(DEPTH)/starboard/shared/starboard/player/job_queue.cc',
        '<(DEPTH)/starboard/shared/starboard/player/job_queue.h',
        '<(DEPTH)/starboard/shared/starboard/player/player_get_current_frame.cc',
        '<(DEPTH)/starboard/shared/starboard/player/player_get_info.cc',
        '<(DEPTH)/starboard/shared/starboard/player/player_get_info2.cc',
        '<(DEPTH)/starboard/shared/starboard/player/player_internal.cc',
        '<(DEPTH)/starboard/shared/starboard/player/player_internal.h',
        '<(DEPTH)/starboard/shared/starboard/player/player_output_mode_supported.cc',
        '<(DEPTH)/starboard/shared/starboard/player/player_seek.cc',
        '<(DEPTH)/starboard/shared/starboard/player/player_seek2.cc',
        '<(DEPTH)/starboard/shared/starboard/player/player_set_volume.cc',
        '<(DEPTH)/starboard/shared/starboard/player/player_worker.cc',
        '<(DEPTH)/starboard/shared/starboard/player/player_worker.h',
        '<(DEPTH)/starboard/shared/starboard/player/player_write_end_of_stream.cc',
        '<(DEPTH)/starboard/shared/starboard/player/player_write_sample.cc',
        '<(DEPTH)/starboard/shared/starboard/player/player_write_sample2.cc',
        '<(DEPTH)/starboard/shared/starboard/queue_application.cc',
        '<(DEPTH)/starboard/shared/starboard/queue_application.h',
        '<(DEPTH)/starboard/shared/starboard/speech_recognizer/speech_recognizer_cancel.cc',
        '<(DEPTH)/starboard/shared/starboard/speech_recognizer/speech_recognizer_create.cc',
        '<(DEPTH)/starboard/shared/starboard/speech_recognizer/speech_recognizer_destroy.cc',
        '<(DEPTH)/starboard/shared/starboard/speech_recognizer/speech_recognizer_internal.h',
        '<(DEPTH)/starboard/shared/starboard/speech_recognizer/speech_recognizer_start.cc',
        '<(DEPTH)/starboard/shared/starboard/speech_recognizer/speech_recognizer_stop.cc',
        '<(DEPTH)/starboard/shared/starboard/string_concat.cc',
        '<(DEPTH)/starboard/shared/starboard/string_concat_wide.cc',
        '<(DEPTH)/starboard/shared/starboard/string_copy.cc',
        '<(DEPTH)/starboard/shared/starboard/string_copy_wide.cc',
        '<(DEPTH)/starboard/shared/starboard/string_duplicate.cc',
        '<(DEPTH)/starboard/shared/starboard/system_get_random_uint64.cc',
        '<(DEPTH)/starboard/shared/starboard/system_supports_resume.cc',
        '<(DEPTH)/starboard/shared/starboard/thread_checker.h',
        '<(DEPTH)/starboard/shared/starboard/window_set_default_options.cc',
        '<(DEPTH)/starboard/shared/stub/cryptography_create_transformer.cc',
        '<(DEPTH)/starboard/shared/stub/cryptography_destroy_transformer.cc',
        '<(DEPTH)/starboard/shared/stub/cryptography_get_tag.cc',
        '<(DEPTH)/starboard/shared/stub/cryptography_set_authenticated_data.cc',
        '<(DEPTH)/starboard/shared/stub/cryptography_set_initialization_vector.cc',
        '<(DEPTH)/starboard/shared/stub/cryptography_transform.cc',
        '<(DEPTH)/starboard/shared/stub/drm_is_server_certificate_updatable.cc',
        '<(DEPTH)/starboard/shared/stub/drm_update_server_certificate.cc',
        '<(DEPTH)/starboard/shared/stub/image_decode.cc',
        '<(DEPTH)/starboard/shared/stub/image_is_decode_supported.cc',
        '<(DEPTH)/starboard/shared/stub/system_get_total_gpu_memory.cc',
        '<(DEPTH)/starboard/shared/stub/system_get_used_gpu_memory.cc',
        '<(DEPTH)/starboard/shared/stub/system_hide_splash_screen.cc',
        '<(DEPTH)/starboard/shared/stub/system_request_pause.cc',
        '<(DEPTH)/starboard/shared/stub/system_request_unpause.cc',
      ],
      'defines': [
        # This must be defined when building Starboard, and must not when
        # building Starboard client code.
        'STARBOARD_IMPLEMENTATION',
      ],
      'dependencies': [
        '<(DEPTH)/starboard/common/common.gyp:common',
        '<(DEPTH)/third_party/libevent/libevent.gyp:libevent',
        'starboard_base_symbolize',
      ],
    },
  ],
}
