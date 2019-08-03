// Copyright 2018 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

package org.chromium.base.task;

/**
 * A task queue that posts Java tasks onto the C++ browser scheduler, if loaded. Otherwise this
 * will be backed by an {@link android.os.Handler} or the java thread pool. The TaskQueue interface
 * provides no guarantee over the order or the thread on which the task will be executed.
 *
 * Very similar to {@link java.util.concurrent.Executor} but conforms to chromium terminology.
 */
public interface TaskRunner {
    /**
     * Posts a task to run immediately.
     *
     * @param task The task to be run immediately.
     */
    public void postTask(Runnable task);

    /**
     * Instructs the TaskRunner to initialize the native TaskRunner and migrate any tasks over to
     * it.
     */
    void initNativeTaskRunner();
}
