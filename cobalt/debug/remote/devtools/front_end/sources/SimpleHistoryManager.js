/*
 * Copyright (C) 2014 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/**
 * @interface
 */
Sources.HistoryEntry = function() {};

Sources.HistoryEntry.prototype = {
  /**
   * @return {boolean}
   */
  valid() {},

  reveal() {}
};

/**
 * @unrestricted
 */
Sources.SimpleHistoryManager = class {
  /**
   * @param {number} historyDepth
   */
  constructor(historyDepth) {
    this._entries = [];
    this._activeEntryIndex = -1;
    this._coalescingReadonly = 0;
    this._historyDepth = historyDepth;
  }

  readOnlyLock() {
    ++this._coalescingReadonly;
  }

  releaseReadOnlyLock() {
    --this._coalescingReadonly;
  }

  /**
   * @return {boolean}
   */
  readOnly() {
    return !!this._coalescingReadonly;
  }

  /**
   * @param {function(!Sources.HistoryEntry):boolean} filterOutCallback
   */
  filterOut(filterOutCallback) {
    if (this.readOnly())
      return;
    const filteredEntries = [];
    let removedBeforeActiveEntry = 0;
    for (let i = 0; i < this._entries.length; ++i) {
      if (!filterOutCallback(this._entries[i]))
        filteredEntries.push(this._entries[i]);
      else if (i <= this._activeEntryIndex)
        ++removedBeforeActiveEntry;
    }
    this._entries = filteredEntries;
    this._activeEntryIndex = Math.max(0, this._activeEntryIndex - removedBeforeActiveEntry);
  }

  /**
   * @return {boolean}
   */
  empty() {
    return !this._entries.length;
  }

  /**
   * @return {?Sources.HistoryEntry}
   */
  active() {
    return this.empty() ? null : this._entries[this._activeEntryIndex];
  }

  /**
   * @param {!Sources.HistoryEntry} entry
   */
  push(entry) {
    if (this.readOnly())
      return;
    if (!this.empty())
      this._entries.splice(this._activeEntryIndex + 1);
    this._entries.push(entry);
    if (this._entries.length > this._historyDepth)
      this._entries.shift();
    this._activeEntryIndex = this._entries.length - 1;
  }

  /**
   * @return {boolean}
   */
  rollback() {
    if (this.empty())
      return false;

    let revealIndex = this._activeEntryIndex - 1;
    while (revealIndex >= 0 && !this._entries[revealIndex].valid())
      --revealIndex;
    if (revealIndex < 0)
      return false;

    this.readOnlyLock();
    this._entries[revealIndex].reveal();
    this.releaseReadOnlyLock();

    this._activeEntryIndex = revealIndex;
    return true;
  }

  /**
   * @return {boolean}
   */
  rollover() {
    let revealIndex = this._activeEntryIndex + 1;

    while (revealIndex < this._entries.length && !this._entries[revealIndex].valid())
      ++revealIndex;
    if (revealIndex >= this._entries.length)
      return false;

    this.readOnlyLock();
    this._entries[revealIndex].reveal();
    this.releaseReadOnlyLock();

    this._activeEntryIndex = revealIndex;
    return true;
  }
};
