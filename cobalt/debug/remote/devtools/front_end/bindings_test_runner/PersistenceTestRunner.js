// Copyright 2017 The Chromium Authors. All
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

/**
 * @fileoverview using private properties isn't a Closure violation in tests.
 * @suppress {accessControls}
 */

Persistence.PersistenceBinding.prototype.toString = function() {
  const lines = ['{', '       network: ' + this.network.url(), '    fileSystem: ' + this.fileSystem.url(), '}'];

  return lines.join('\n');
};

Persistence.AutomappingStatus.prototype.toString = function() {
  const lines = [
    '{', '       network: ' + this.network.url(), '    fileSystem: ' + this.fileSystem.url(),
    '    exactMatch: ' + this.exactMatch, '}'
  ];

  return lines.join('\n');
};


BindingsTestRunner.waitForBinding = function(fileName) {
  const uiSourceCodes = Workspace.workspace.uiSourceCodes();

  for (const uiSourceCode of uiSourceCodes) {
    const binding = Persistence.persistence.binding(uiSourceCode);

    if (!binding)
      continue;

    if (uiSourceCode.name() === fileName)
      return Promise.resolve(binding);
  }

  return TestRunner.waitForEvent(
      Persistence.Persistence.Events.BindingCreated, Persistence.persistence,
      binding => binding.network.name() === fileName || binding.fileSystem.name() === fileName);
};

BindingsTestRunner.addFooJSFile = function(fs) {
  return fs.root.mkdir('devtools')
      .mkdir('persistence')
      .mkdir('resources')
      .addFile('foo.js', '\n\nwindow.foo = ()=>\'foo\';\n');
};

BindingsTestRunner.initializeTestMapping = function() {
  return new TestMapping(Persistence.persistence);
};

class TestMapping {
  constructor(persistence) {
    this._persistence = persistence;
    persistence.setAutomappingEnabled(false);
    this._bindings = new Set();
  }

  async addBinding(urlSuffix) {
    if (this._findBinding(urlSuffix)) {
      TestRunner.addResult(`FAILED TO ADD BINDING: binding already exists for ${urlSuffix}`);
      TestRunner.completeTest();
      return;
    }

    const networkUISourceCode = await TestRunner.waitForUISourceCode(urlSuffix, Workspace.projectTypes.Network);
    const fileSystemUISourceCode = await TestRunner.waitForUISourceCode(urlSuffix, Workspace.projectTypes.FileSystem);
    const binding = new Persistence.PersistenceBinding(networkUISourceCode, fileSystemUISourceCode);
    this._bindings.add(binding);
    this._persistence.addBindingForTest(binding);
  }

  _findBinding(urlSuffix) {
    for (const binding of this._bindings) {
      if (binding.network.url().endsWith(urlSuffix))
        return binding;
    }

    return null;
  }

  async removeBinding(urlSuffix) {
    const binding = this._findBinding(urlSuffix);

    if (!binding) {
      TestRunner.addResult(`FAILED TO REMOVE BINDING: binding does not exist for ${urlSuffix}`);
      TestRunner.completeTest();
      return;
    }

    this._bindings.delete(binding);
    this._persistence.removeBindingForTest(binding);
  }

  dispose() {
    for (const binding of this._bindings)
      this._persistence.removeBindingForTest(binding);

    this._bindings.clear();
  }
}
