// Copyright 2016 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

const MaxWorkers = 2;

/**
 * @unrestricted
 */
export class FormatterWorkerPool {
  constructor() {
    this._taskQueue = [];
    /** @type {!Map<!Common.Worker, ?Task>} */
    this._workerTasks = new Map();
  }

  /**
   * @return {!Common.Worker}
   */
  _createWorker() {
    const worker = new Common.Worker('formatter_worker');
    worker.onmessage = this._onWorkerMessage.bind(this, worker);
    worker.onerror = this._onWorkerError.bind(this, worker);
    return worker;
  }

  _processNextTask() {
    if (!this._taskQueue.length) {
      return;
    }

    let freeWorker = this._workerTasks.keysArray().find(worker => !this._workerTasks.get(worker));
    if (!freeWorker && this._workerTasks.size < MaxWorkers) {
      freeWorker = this._createWorker();
    }
    if (!freeWorker) {
      return;
    }

    const task = this._taskQueue.shift();
    this._workerTasks.set(freeWorker, task);
    freeWorker.postMessage({method: task.method, params: task.params});
  }

  /**
   * @param {!Common.Worker} worker
   * @param {!MessageEvent} event
   */
  _onWorkerMessage(worker, event) {
    const task = this._workerTasks.get(worker);
    if (task.isChunked && event.data && !event.data['isLastChunk']) {
      task.callback(event.data);
      return;
    }

    this._workerTasks.set(worker, null);
    this._processNextTask();
    task.callback(event.data ? event.data : null);
  }

  /**
   * @param {!Common.Worker} worker
   * @param {!Event} event
   */
  _onWorkerError(worker, event) {
    console.error(event);
    const task = this._workerTasks.get(worker);
    worker.terminate();
    this._workerTasks.delete(worker);

    const newWorker = this._createWorker();
    this._workerTasks.set(newWorker, null);
    this._processNextTask();
    task.callback(null);
  }

  /**
   * @param {string} methodName
   * @param {!Object<string, string>} params
   * @param {function(boolean, *)} callback
   */
  _runChunkedTask(methodName, params, callback) {
    const task = new Task(methodName, params, onData, true);
    this._taskQueue.push(task);
    this._processNextTask();

    /**
     * @param {?Object} data
     */
    function onData(data) {
      if (!data) {
        callback(true, null);
        return;
      }
      const isLastChunk = !!data['isLastChunk'];
      const chunk = data['chunk'];
      callback(isLastChunk, chunk);
    }
  }

  /**
   * @param {string} methodName
   * @param {!Object<string, string>} params
   * @return {!Promise<*>}
   */
  _runTask(methodName, params) {
    let callback;
    const promise = new Promise(fulfill => callback = fulfill);
    const task = new Task(methodName, params, callback, false);
    this._taskQueue.push(task);
    this._processNextTask();
    return promise;
  }

  /**
   * @param {string} content
   * @return {!Promise<*>}
   */
  parseJSONRelaxed(content) {
    return this._runTask('parseJSONRelaxed', {content: content});
  }

  /**
   * @param {string} content
   * @return {!Promise<!Array<!SCSSRule>>}
   */
  parseSCSS(content) {
    return this._runTask('parseSCSS', {content: content}).then(rules => rules || []);
  }

  /**
   * @param {string} mimeType
   * @param {string} content
   * @param {string} indentString
   * @return {!Promise<!FormatResult>}
   */
  format(mimeType, content, indentString) {
    const parameters = {mimeType: mimeType, content: content, indentString: indentString};
    return /** @type {!Promise<!FormatResult>} */ (this._runTask('format', parameters));
  }

  /**
   * @param {string} content
   * @return {!Promise<!Array<!{name: string, offset: number}>>}
   */
  javaScriptIdentifiers(content) {
    return this._runTask('javaScriptIdentifiers', {content: content}).then(ids => ids || []);
  }

  /**
   * @param {string} content
   * @return {!Promise<string>}
   */
  evaluatableJavaScriptSubstring(content) {
    return this._runTask('evaluatableJavaScriptSubstring', {content: content}).then(text => text || '');
  }

  /**
   * @param {string} content
   * @return {!Promise<string>}
   */
  preprocessTopLevelAwaitExpressions(content) {
    return this._runTask('preprocessTopLevelAwaitExpressions', {content: content}).then(text => text || '');
  }

  /**
   * @param {string} content
   * @param {function(boolean, !Array<!Formatter.FormatterWorkerPool.CSSRule>)} callback
   */
  parseCSS(content, callback) {
    this._runChunkedTask('parseCSS', {content: content}, onDataChunk);

    /**
     * @param {boolean} isLastChunk
     * @param {*} data
     */
    function onDataChunk(isLastChunk, data) {
      const rules = /** @type {!Array<!Formatter.FormatterWorkerPool.CSSRule>} */ (data || []);
      callback(isLastChunk, rules);
    }
  }

  /**
   * @param {string} content
   * @param {function(boolean, !Array<!JSOutlineItem>)} callback
   */
  javaScriptOutline(content, callback) {
    this._runChunkedTask('javaScriptOutline', {content: content}, onDataChunk);

    /**
     * @param {boolean} isLastChunk
     * @param {*} data
     */
    function onDataChunk(isLastChunk, data) {
      const items = /** @type {!Array.<!JSOutlineItem>} */ (data || []);
      callback(isLastChunk, items);
    }
  }

  /**
   * @param {string} content
   * @param {string} mimeType
   * @param {function(boolean, !Array<!Formatter.FormatterWorkerPool.OutlineItem>)} callback
   * @return {boolean}
   */
  outlineForMimetype(content, mimeType, callback) {
    switch (mimeType) {
      case 'text/html':
      case 'text/javascript':
        this.javaScriptOutline(content, javaScriptCallback);
        return true;
      case 'text/css':
        this.parseCSS(content, cssCallback);
        return true;
    }
    return false;

    /**
     * @param {boolean} isLastChunk
     * @param {!Array<!JSOutlineItem>} items
     */
    function javaScriptCallback(isLastChunk, items) {
      callback(
          isLastChunk,
          items.map(item => ({line: item.line, column: item.column, title: item.name, subtitle: item.arguments})));
    }

    /**
     * @param {boolean} isLastChunk
     * @param {!Array<!Formatter.FormatterWorkerPool.CSSRule>} rules
     */
    function cssCallback(isLastChunk, rules) {
      callback(
          isLastChunk,
          rules.map(
              rule => ({line: rule.lineNumber, column: rule.columnNumber, title: rule.selectorText || rule.atRule})));
    }
  }

  /**
   * @param {string} content
   * @return {!Promise<?{baseExpression: string, possibleSideEffects:boolean}>}
   */
  findLastExpression(content) {
    return /** @type {!Promise<?{baseExpression: string, possibleSideEffects:boolean}>} */ (
        this._runTask('findLastExpression', {content}));
  }

  /**
   * @param {string} content
   * @return {!Promise<?{baseExpression: string, possibleSideEffects:boolean, receiver: string, argumentIndex: number, functionName: string}>}
   */
  findLastFunctionCall(content) {
    return /** @type {!Promise<?{baseExpression: string, possibleSideEffects:boolean, receiver: string, argumentIndex: number, functionName: string}>} */ (
        this._runTask('findLastFunctionCall', {content}));
  }

  /**
   * @param {string} content
   * @return {!Promise<!Array<string>>}
   */
  argumentsList(content) {
    return /** @type {!Promise<!Array<string>>} */ (this._runTask('argumentsList', {content}));
  }
}

/**
 * @unrestricted
 */
class Task {
  /**
   * @param {string} method
   * @param {!Object<string, string>} params
   * @param {function(?MessageEvent)} callback
   * @param {boolean=} isChunked
   */
  constructor(method, params, callback, isChunked) {
    this.method = method;
    this.params = params;
    this.callback = callback;
    this.isChunked = isChunked;
  }
}

export class FormatResult {
  constructor() {
    /** @type {string} */
    this.content;
    /** @type {!Formatter.FormatterWorkerPool.FormatMapping} */
    this.mapping;
  }
}

// eslint-disable-next-line no-unused-vars
class JSOutlineItem {
  constructor() {
    /** @type {string} */
    this.name;
    /** @type {(string|undefined)} */
    this.arguments;
    /** @type {number} */
    this.line;
    /** @type {number} */
    this.column;
  }
}

// eslint-disable-next-line no-unused-vars
class CSSProperty {
  constructor() {
    /** @type {string} */
    this.name;
    /** @type {!Formatter.FormatterWorkerPool.TextRange} */
    this.nameRange;
    /** @type {string} */
    this.value;
    /** @type {!Formatter.FormatterWorkerPool.TextRange} */
    this.valueRange;
    /** @type {!Formatter.FormatterWorkerPool.TextRange} */
    this.range;
    /** @type {(boolean|undefined)} */
    this.disabled;
  }
}

// eslint-disable-next-line no-unused-vars
class CSSStyleRule {
  constructor() {
    /** @type {string} */
    this.selectorText;
    /** @type {!Formatter.FormatterWorkerPool.TextRange} */
    this.styleRange;
    /** @type {number} */
    this.lineNumber;
    /** @type {number} */
    this.columnNumber;
    /** @type {!Array.<!CSSProperty>} */
    this.properties;
  }
}

// eslint-disable-next-line no-unused-vars
class SCSSProperty {
  constructor() {
    /** @type {!Formatter.FormatterWorkerPool.TextRange} */
    this.range;
    /** @type {!Formatter.FormatterWorkerPool.TextRange} */
    this.name;
    /** @type {!Formatter.FormatterWorkerPool.TextRange} */
    this.value;
    /** @type {boolean} */
    this.disabled;
  }
}

// eslint-disable-next-line no-unused-vars
class SCSSRule {
  constructor() {
    /** @type {!Array<!Formatter.FormatterWorkerPool.TextRange>} */
    this.selectors;
    /** @type {!Array<!SCSSProperty>} */
    this.properties;
    /** @type {!Formatter.FormatterWorkerPool.TextRange} */
    this.styleRange;
  }
}

/**
 * @return {!FormatterWorkerPool}
 */
export function formatterWorkerPool() {
  if (!Formatter._formatterWorkerPool) {
    Formatter._formatterWorkerPool = new FormatterWorkerPool();
  }
  return Formatter._formatterWorkerPool;
}

/* Legacy exported object */
self.Formatter = self.Formatter || {};

/* Legacy exported object */
Formatter = Formatter || {};

/** @constructor */
Formatter.FormatterWorkerPool = FormatterWorkerPool;

Formatter.formatterWorkerPool = formatterWorkerPool;

/** @constructor */
Formatter.FormatterWorkerPool.FormatResult = FormatResult;

/** @typedef {{original: !Array<number>, formatted: !Array<number>}} */
Formatter.FormatterWorkerPool.FormatMapping;

/** @typedef {{line: number, column: number, title: string, subtitle: (string|undefined) }} */
Formatter.FormatterWorkerPool.OutlineItem;

/**
 * @typedef {{atRule: string, lineNumber: number, columnNumber: number}}
 */
Formatter.FormatterWorkerPool.CSSAtRule;

/**
 * @typedef {(CSSStyleRule|Formatter.FormatterWorkerPool.CSSAtRule)}
 */
Formatter.FormatterWorkerPool.CSSRule;

/**
 * @typedef {{startLine: number, startColumn: number, endLine: number, endColumn: number}}
 */
Formatter.FormatterWorkerPool.TextRange;
