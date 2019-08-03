// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
/**
 * @implements {UI.Searchable}
 * @unrestricted
 */
Profiler.HeapProfileView = class extends Profiler.ProfileView {
  /**
   * @param {!Profiler.SamplingHeapProfileHeader} profileHeader
   */
  constructor(profileHeader) {
    super();
    this._profileHeader = profileHeader;
    this.profile = new Profiler.SamplingHeapProfileModel(profileHeader._profile || profileHeader.protocolProfile());
    this.adjustedTotal = this.profile.total;
    const views = [
      Profiler.ProfileView.ViewTypes.Flame, Profiler.ProfileView.ViewTypes.Heavy, Profiler.ProfileView.ViewTypes.Tree
    ];
    this.initialize(new Profiler.HeapProfileView.NodeFormatter(this), views);
  }

  /**
   * @override
   * @param {string} columnId
   * @return {string}
   */
  columnHeader(columnId) {
    switch (columnId) {
      case 'self':
        return Common.UIString('Self Size (bytes)');
      case 'total':
        return Common.UIString('Total Size (bytes)');
    }
    return '';
  }

  /**
   * @override
   * @return {!PerfUI.FlameChartDataProvider}
   */
  createFlameChartDataProvider() {
    return new Profiler.HeapFlameChartDataProvider(this.profile, this._profileHeader.heapProfilerModel());
  }
};

/**
 * @unrestricted
 */
Profiler.SamplingHeapProfileTypeBase = class extends Profiler.ProfileType {
  constructor(typeId, description) {
    super(typeId, description);
    this._recording = false;
  }

  /**
   * @override
   * @return {?Profiler.SamplingHeapProfileHeader}
   */
  profileBeingRecorded() {
    return /** @type {?Profiler.SamplingHeapProfileHeader} */ (super.profileBeingRecorded());
  }

  /**
   * @override
   * @return {string}
   */
  typeName() {
    return 'Heap';
  }

  /**
   * @override
   * @return {string}
   */
  fileExtension() {
    return '.heapprofile';
  }

  get buttonTooltip() {
    return this._recording ? Common.UIString('Stop heap profiling') : Common.UIString('Start heap profiling');
  }

  /**
   * @override
   * @return {boolean}
   */
  buttonClicked() {
    const wasRecording = this._recording;
    if (wasRecording)
      this.stopRecordingProfile();
    else
      this.startRecordingProfile();
    return !wasRecording;
  }

  startRecordingProfile() {
    const heapProfilerModel = UI.context.flavor(SDK.HeapProfilerModel);
    if (this.profileBeingRecorded() || !heapProfilerModel)
      return;
    const profile = new Profiler.SamplingHeapProfileHeader(heapProfilerModel, this);
    this.setProfileBeingRecorded(profile);
    SDK.targetManager.suspendAllTargets();
    this.addProfile(profile);
    profile.updateStatus(Common.UIString('Recording\u2026'));
    this._recording = true;
    this._startSampling();
  }

  async stopRecordingProfile() {
    this._recording = false;
    if (!this.profileBeingRecorded() || !this.profileBeingRecorded().heapProfilerModel())
      return;

    this.profileBeingRecorded().updateStatus(Common.UIString('Stopping\u2026'));
    const profile = await this._stopSampling();
    const recordedProfile = this.profileBeingRecorded();
    if (recordedProfile) {
      console.assert(profile);
      recordedProfile.setProtocolProfile(profile);
      recordedProfile.updateStatus('');
      this.setProfileBeingRecorded(null);
    }

    await SDK.targetManager.resumeAllTargets();
    this.dispatchEventToListeners(Profiler.ProfileType.Events.ProfileComplete, recordedProfile);
  }

  /**
   * @override
   * @param {string} title
   * @return {!Profiler.ProfileHeader}
   */
  createProfileLoadedFromFile(title) {
    return new Profiler.SamplingHeapProfileHeader(null, this, title);
  }

  /**
   * @override
   */
  profileBeingRecordedRemoved() {
    this.stopRecordingProfile();
  }

  _startSampling() {
    throw 'Not implemented';
  }

  /**
   * return {!Promise<!Protocol.HeapProfiler.SamplingHeapProfile>}
   */
  _stopSampling() {
    throw 'Not implemented';
  }
};


/**
 * @unrestricted
 */
Profiler.SamplingHeapProfileType = class extends Profiler.SamplingHeapProfileTypeBase {
  constructor() {
    super(Profiler.SamplingHeapProfileType.TypeId, ls`Allocation sampling`);
    Profiler.SamplingHeapProfileType.instance = this;
  }

  get treeItemTitle() {
    return ls`SAMPLING PROFILES`;
  }

  get description() {
    return ls`Record memory allocations using sampling method.
              This profile type has minimal performance overhead and can be used for long running operations.
              It provides good approximation of allocations broken down by JavaScript execution stack.`;
  }

  /**
   * @override
   */
  _startSampling() {
    this.profileBeingRecorded().heapProfilerModel().startSampling();
  }

  /**
   * @override
   * return {!Promise<!Protocol.HeapProfiler.SamplingHeapProfile>}
   */
  _stopSampling() {
    return this.profileBeingRecorded().heapProfilerModel().stopSampling();
  }
};

Profiler.SamplingHeapProfileType.TypeId = 'SamplingHeap';

/**
 * @unrestricted
 */
Profiler.SamplingNativeHeapProfileType = class extends Profiler.SamplingHeapProfileTypeBase {
  constructor() {
    super(Profiler.SamplingNativeHeapProfileType.TypeId, ls`Native memory allocation sampling`);
    Profiler.SamplingNativeHeapProfileType.instance = this;
  }

  get treeItemTitle() {
    return ls`NATIVE SAMPLING PROFILES`;
  }

  get description() {
    return ls`Allocation profiles show sampled native memory allocations from the renderer process.`;
  }

  /**
   * @override
   */
  _startSampling() {
    this.profileBeingRecorded().heapProfilerModel().startNativeSampling();
  }

  /**
   * @override
   * return {!Promise<!Protocol.HeapProfiler.SamplingHeapProfile>}
   */
  _stopSampling() {
    return this.profileBeingRecorded().heapProfilerModel().stopNativeSampling();
  }
};

Profiler.SamplingNativeHeapProfileType.TypeId = 'SamplingNativeHeapRecording';

/**
 * @unrestricted
 */
Profiler.SamplingNativeHeapSnapshotType = class extends Profiler.SamplingHeapProfileTypeBase {
  constructor() {
    super(Profiler.SamplingNativeHeapSnapshotType.TypeId, ls`Native memory allocation snapshot`);
    Profiler.SamplingNativeHeapSnapshotType.instance = this;
  }

  /**
   * @override
   * @return {boolean}
   */
  isInstantProfile() {
    return true;
  }

  get treeItemTitle() {
    return ls`NATIVE SNAPSHOTS`;
  }

  get description() {
    return ls`Native memory snapshots show sampled native allocations in the renderer process since start up.
              Chrome has to be started with --sampling-heap-profiler flag.
              Check flags at chrome://flags`;
  }

  /**
   * @override
   * @return {boolean}
   */
  buttonClicked() {
    this._takeSnapshot();
    return false;
  }

  /**
   * @return {!Promise}
   */
  async _takeSnapshot() {
    if (this.profileBeingRecorded())
      return;
    const heapProfilerModel = UI.context.flavor(SDK.HeapProfilerModel);
    if (!heapProfilerModel)
      return;

    const profile = new Profiler.SamplingHeapProfileHeader(
        heapProfilerModel, this, Common.UIString('Snapshot %d', this.nextProfileUid()));
    this.setProfileBeingRecorded(profile);
    this.addProfile(profile);
    profile.updateStatus(Common.UIString('Snapshotting\u2026'));

    const protocolProfile = await heapProfilerModel.takeNativeSnapshot();
    const recordedProfile = this.profileBeingRecorded();
    if (recordedProfile) {
      console.assert(protocolProfile);
      recordedProfile.setProtocolProfile(protocolProfile);
      recordedProfile.updateStatus('');
      this.setProfileBeingRecorded(null);
    }

    this.dispatchEventToListeners(Profiler.ProfileType.Events.ProfileComplete, recordedProfile);
  }
};

Profiler.SamplingNativeHeapSnapshotType.TypeId = 'SamplingNativeHeapSnapshot';

/**
 * @unrestricted
 */
Profiler.SamplingHeapProfileHeader = class extends Profiler.WritableProfileHeader {
  /**
   * @param {?SDK.HeapProfilerModel} heapProfilerModel
   * @param {!Profiler.SamplingHeapProfileTypeBase} type
   * @param {string=} title
   */
  constructor(heapProfilerModel, type, title) {
    super(
        heapProfilerModel && heapProfilerModel.debuggerModel(), type,
        title || Common.UIString('Profile %d', type.nextProfileUid()));
    this._heapProfilerModel = heapProfilerModel;
  }

  /**
   * @override
   * @return {!Profiler.ProfileView}
   */
  createView() {
    return new Profiler.HeapProfileView(this);
  }

  /**
   * @return {!Protocol.HeapProfiler.SamplingHeapProfile}
   */
  protocolProfile() {
    return this._protocolProfile;
  }

  /**
   * @return {?SDK.HeapProfilerModel}
   */
  heapProfilerModel() {
    return this._heapProfilerModel;
  }
};

/**
 * @unrestricted
 */
Profiler.SamplingHeapProfileNode = class extends SDK.ProfileNode {
  /**
   * @param {!Protocol.HeapProfiler.SamplingHeapProfileNode} node
   */
  constructor(node) {
    const callFrame = node.callFrame || /** @type {!Protocol.Runtime.CallFrame} */ ({
                        // Backward compatibility for old CpuProfileNode format.
                        functionName: node['functionName'],
                        scriptId: node['scriptId'],
                        url: node['url'],
                        lineNumber: node['lineNumber'] - 1,
                        columnNumber: node['columnNumber'] - 1
                      });
    super(callFrame);
    this.self = node.selfSize;
  }
};

/**
 * @unrestricted
 */
Profiler.SamplingHeapProfileModel = class extends SDK.ProfileTreeModel {
  /**
   * @param {!Protocol.HeapProfiler.SamplingHeapProfile} profile
   */
  constructor(profile) {
    super();
    this.initialize(translateProfileTree(profile.head));

    /**
     * @param {!Protocol.HeapProfiler.SamplingHeapProfileNode} root
     * @return {!Profiler.SamplingHeapProfileNode}
     */
    function translateProfileTree(root) {
      const resultRoot = new Profiler.SamplingHeapProfileNode(root);
      const targetNodeStack = [resultRoot];
      const sourceNodeStack = [root];
      while (sourceNodeStack.length) {
        const sourceNode = sourceNodeStack.pop();
        const parentNode = targetNodeStack.pop();
        parentNode.children = sourceNode.children.map(child => new Profiler.SamplingHeapProfileNode(child));
        sourceNodeStack.push.apply(sourceNodeStack, sourceNode.children);
        targetNodeStack.push.apply(targetNodeStack, parentNode.children);
      }
      return resultRoot;
    }
  }
};

/**
 * @implements {Profiler.ProfileDataGridNode.Formatter}
 * @unrestricted
 */
Profiler.HeapProfileView.NodeFormatter = class {
  /**
   * @param {!Profiler.HeapProfileView} profileView
   */
  constructor(profileView) {
    this._profileView = profileView;
  }

  /**
   * @override
   * @param {number} value
   * @return {string}
   */
  formatValue(value) {
    return Number.withThousandsSeparator(value);
  }

  /**
   * @override
   * @param {number} value
   * @param {!Profiler.ProfileDataGridNode} node
   * @return {string}
   */
  formatPercent(value, node) {
    return Common.UIString('%.2f\xa0%%', value);
  }

  /**
   * @override
   * @param  {!Profiler.ProfileDataGridNode} node
   * @return {?Element}
   */
  linkifyNode(node) {
    const heapProfilerModel = this._profileView._profileHeader.heapProfilerModel();
    return this._profileView.linkifier().maybeLinkifyConsoleCallFrame(
        heapProfilerModel ? heapProfilerModel.target() : null, node.profileNode.callFrame, 'profile-node-file');
  }
};

/**
 * @unrestricted
 */
Profiler.HeapFlameChartDataProvider = class extends Profiler.ProfileFlameChartDataProvider {
  /**
   * @param {!SDK.ProfileTreeModel} profile
   * @param {?SDK.HeapProfilerModel} heapProfilerModel
   */
  constructor(profile, heapProfilerModel) {
    super();
    this._profile = profile;
    this._heapProfilerModel = heapProfilerModel;
  }

  /**
   * @override
   * @return {number}
   */
  minimumBoundary() {
    return 0;
  }

  /**
   * @override
   * @return {number}
   */
  totalTime() {
    return this._profile.root.total;
  }

  /**
   * @override
   * @param {number} value
   * @param {number=} precision
   * @return {string}
   */
  formatValue(value, precision) {
    return Common.UIString('%s\xa0KB', Number.withThousandsSeparator(value / 1e3));
  }

  /**
   * @override
   * @return {!PerfUI.FlameChart.TimelineData}
   */
  _calculateTimelineData() {
    /**
     * @param  {!SDK.ProfileNode} node
     * @return {number}
     */
    function nodesCount(node) {
      return node.children.reduce((count, node) => count + nodesCount(node), 1);
    }
    const count = nodesCount(this._profile.root);
    /** @type {!Array<!SDK.ProfileNode>} */
    const entryNodes = new Array(count);
    const entryLevels = new Uint16Array(count);
    const entryTotalTimes = new Float32Array(count);
    const entryStartTimes = new Float64Array(count);
    let depth = 0;
    let maxDepth = 0;
    let position = 0;
    let index = 0;

    /**
     * @param {!SDK.ProfileNode} node
     */
    function addNode(node) {
      const start = position;
      entryNodes[index] = node;
      entryLevels[index] = depth;
      entryTotalTimes[index] = node.total;
      entryStartTimes[index] = position;
      ++index;
      ++depth;
      node.children.forEach(addNode);
      --depth;
      maxDepth = Math.max(maxDepth, depth);
      position = start + node.total;
    }
    addNode(this._profile.root);

    this._maxStackDepth = maxDepth + 1;
    this._entryNodes = entryNodes;
    this._timelineData = new PerfUI.FlameChart.TimelineData(entryLevels, entryTotalTimes, entryStartTimes, null);

    return this._timelineData;
  }

  /**
   * @override
   * @param {number} entryIndex
   * @return {?Element}
   */
  prepareHighlightedEntryInfo(entryIndex) {
    const node = this._entryNodes[entryIndex];
    if (!node)
      return null;
    const entryInfo = [];
    /**
     * @param {string} title
     * @param {string} value
     */
    function pushEntryInfoRow(title, value) {
      entryInfo.push({title: title, value: value});
    }
    pushEntryInfoRow(Common.UIString('Name'), UI.beautifyFunctionName(node.functionName));
    pushEntryInfoRow(Common.UIString('Self size'), Number.bytesToString(node.self));
    pushEntryInfoRow(Common.UIString('Total size'), Number.bytesToString(node.total));
    const linkifier = new Components.Linkifier();
    const link = linkifier.maybeLinkifyConsoleCallFrame(
        this._heapProfilerModel ? this._heapProfilerModel.target() : null, node.callFrame);
    if (link)
      pushEntryInfoRow(Common.UIString('URL'), link.textContent);
    linkifier.dispose();
    return Profiler.ProfileView.buildPopoverTable(entryInfo);
  }
};
