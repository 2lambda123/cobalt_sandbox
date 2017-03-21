/*
 * Copyright (C) 2010 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef IDBBindingUtilities_h
#define IDBBindingUtilities_h

#include "bindings/core/v8/ScriptState.h"
#include "bindings/core/v8/ScriptValue.h"
#include "wtf/Forward.h"
#include <v8.h>

namespace blink {

class IDBAny;
class IDBKey;
class IDBKeyPath;
class IDBKeyRange;
class SerializedScriptValue;
class SharedBuffer;
class WebBlobInfo;

// Exposed for unit testing:
bool injectV8KeyIntoV8Value(v8::Isolate*, v8::Local<v8::Value> key, v8::Local<v8::Value>, const IDBKeyPath&);

// For use by Source/modules/indexeddb:
IDBKey* createIDBKeyFromScriptValueAndKeyPath(v8::Isolate*, const ScriptValue&, const IDBKeyPath&);
bool canInjectIDBKeyIntoScriptValue(v8::Isolate*, const ScriptValue&, const IDBKeyPath&);
ScriptValue idbAnyToScriptValue(ScriptState*, IDBAny*);
ScriptValue idbKeyToScriptValue(ScriptState*, IDBKey*);
IDBKey* scriptValueToIDBKey(v8::Isolate*, const ScriptValue&);
IDBKeyRange* scriptValueToIDBKeyRange(v8::Isolate*, const ScriptValue&);
ScriptValue deserializeScriptValue(ScriptState*, SerializedScriptValue*, const Vector<blink::WebBlobInfo>*);

#if ENABLE(ASSERT)
void assertPrimaryKeyValidOrInjectable(ScriptState*, PassRefPtr<SharedBuffer>, const Vector<blink::WebBlobInfo>*, IDBKey*, const IDBKeyPath&);
#endif

} // namespace blink

#endif // IDBBindingUtilities_h
