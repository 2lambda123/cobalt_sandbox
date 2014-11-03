// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

// This file has been auto-generated by code_generator_v8.py. DO NOT MODIFY!

#ifndef V8TestCallbackInterface_h
#define V8TestCallbackInterface_h

#include "bindings/core/v8/ActiveDOMCallback.h"
#include "bindings/core/v8/DOMWrapperWorld.h"
#include "bindings/core/v8/ScopedPersistent.h"
#include "bindings/tests/idls/core/TestCallbackInterface.h"

namespace blink {

class V8TestCallbackInterface final : public TestCallbackInterface, public ActiveDOMCallback {
public:
    static V8TestCallbackInterface* create(v8::Handle<v8::Function> callback, ScriptState* scriptState)
    {
        return new V8TestCallbackInterface(callback, scriptState);
    }

    virtual ~V8TestCallbackInterface();

    virtual void voidMethod() override;
    virtual bool booleanMethod() override;
    virtual void voidMethodBooleanArg(bool boolArg) override;
    virtual void voidMethodSequenceArg(const Vector<RefPtr<TestInterfaceEmpty> >& sequenceArg) override;
    virtual void voidMethodFloatArg(float floatArg) override;
    virtual void voidMethodTestInterfaceEmptyArg(TestInterfaceEmpty* testInterfaceEmptyArg) override;
    virtual void voidMethodTestInterfaceEmptyStringArg(TestInterfaceEmpty* testInterfaceEmptyArg, const String& stringArg) override;
    virtual void callbackWithThisValueVoidMethodStringArg(ScriptValue thisValue, const String& stringArg) override;
    virtual void customVoidMethodTestInterfaceEmptyArg(TestInterfaceEmpty* testInterfaceEmptyArg) override;
    virtual void voidMethodWillBeGarbageCollectedSequenceArg(const WillBeHeapVector<RefPtrWillBeMember<TestInterfaceWillBeGarbageCollected> >& sequenceArg) override;
    virtual void voidMethodWillBeGarbageCollectedArrayArg(const WillBeHeapVector<RefPtrWillBeMember<TestInterfaceWillBeGarbageCollected> >& arrayArg) override;
private:
    V8TestCallbackInterface(v8::Handle<v8::Function>, ScriptState*);

    ScopedPersistent<v8::Function> m_callback;
    RefPtr<ScriptState> m_scriptState;
};

}
#endif // V8TestCallbackInterface_h
