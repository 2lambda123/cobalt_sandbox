/*
 * Copyright 2012 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrBackendProcessorFactory_DEFINED
#define GrBackendProcessorFactory_DEFINED

#include "GrTypes.h"
#include "SkTemplates.h"
#include "SkThread.h"
#include "SkTypes.h"
#include "SkTArray.h"

class GrGLProcessor;
class GrGLCaps;
class GrProcessor;

/**
 * Used by effects to build their keys. It incorporates each per-processor key into a larger shader
 *  key.
 */
class GrProcessorKeyBuilder {
public:
    GrProcessorKeyBuilder(SkTArray<unsigned char, true>* data) : fData(data), fCount(0) {
        SkASSERT(0 == fData->count() % sizeof(uint32_t));
    }

    void add32(uint32_t v) {
        ++fCount;
        fData->push_back_n(4, reinterpret_cast<uint8_t*>(&v));
    }

    /** Inserts count uint32_ts into the key. The returned pointer is only valid until the next
        add*() call. */
    uint32_t* SK_WARN_UNUSED_RESULT add32n(int count) {
        SkASSERT(count > 0);
        fCount += count;
        return reinterpret_cast<uint32_t*>(fData->push_back_n(4 * count));
    }

    size_t size() const { return sizeof(uint32_t) * fCount; }

private:
    SkTArray<uint8_t, true>* fData; // unowned ptr to the larger key.
    int fCount;                     // number of uint32_ts added to fData by the effect.
};

/**
 * This class is used to pass the key that was created for a GrGLProcessor back to it
 * when it emits code. It may allow the emit step to skip calculations that were
 * performed when computing the key.
 */
class GrProcessorKey {
public:
    GrProcessorKey(const uint32_t* key, int count) : fKey(key), fCount(count) {
        SkASSERT(0 == reinterpret_cast<intptr_t>(key) % sizeof(uint32_t));
    }

    /** Gets the uint32_t values that the effect inserted into the key. */
    uint32_t get32(int index) const {
        SkASSERT(index >=0 && index < fCount);
        return fKey[index];
    }

    /** Gets the number of uint32_t values that the effect inserted into the key. */
    int count32() const { return fCount; }

private:
    const uint32_t* fKey;           // unowned ptr into the larger key.
    int             fCount;         // number of uint32_ts inserted by the effect into its key.
};

/**
 * Given a GrProcessor of a particular type, creates the corresponding graphics-backend-specific
 * effect object. It also tracks equivalence of shaders generated via a key. The factory for an
 * effect is accessed via GrProcessor::getFactory(). Each factory instance is assigned an ID at
 * construction. The ID of GrProcessor::getFactory() is used as a type identifier. Thus, a
 * GrProcessor subclass must always return the same object from getFactory() and that factory object
 * must be unique to the GrProcessor subclass (and unique from any further derived subclasses).
 *
 * Rather than subclassing this class themselves, it is recommended that GrProcessor authors use 
 * the templated subclass GrTBackendEffectFactory by writing their getFactory() method as:
 *
 * const GrBackendEffectFactory& MyEffect::getFactory() const {
 *     return GrTBackendEffectFactory<MyEffect>::getInstance();
 * }
 *
 * Using GrTBackendEffectFactory places a few constraints on the effect. See that class's comments.
 */
class GrBackendProcessorFactory : SkNoncopyable {
public:
    /** 
     * Generates an effect's key. The key is based on the aspects of the GrProcessor object's
     * configuration that affect GLSL code generation. Two GrProcessor instances that would cause
     * this->createGLInstance()->emitCode() to produce different code must produce different keys.
     */
    virtual void getGLProcessorKey(const GrProcessor&, const GrGLCaps&,
                                   GrProcessorKeyBuilder*) const = 0;

    /**
     * Produces a human-reable name for the effect.
     */
    virtual const char* name() const = 0;

    /**
     * A unique value for every instance of this factory. It is automatically incorporated into the
     * effect's key. This allows keys generated by getGLProcessorKey() to only be unique within a
     * GrProcessor subclass and not necessarily across subclasses.
     */
    uint32_t effectClassID() const { return fEffectClassID; }

protected:
    GrBackendProcessorFactory() : fEffectClassID(GenID()) {}
    virtual ~GrBackendProcessorFactory() {}

private:
    enum {
        kIllegalEffectClassID = 0,
    };

    static uint32_t GenID() {
        // fCurrEffectClassID has been initialized to kIllegalEffectClassID. The
        // atomic inc returns the old value not the incremented value. So we add
        // 1 to the returned value.
        uint32_t id = static_cast<uint32_t>(sk_atomic_inc(&fCurrEffectClassID)) + 1;
        if (!id) {
            SkFAIL("This should never wrap as it should only be called once for each GrProcessor "
                   "subclass.");
        }
        return id;
    }

    const uint32_t fEffectClassID;
    static int32_t fCurrEffectClassID;
};

class GrFragmentProcessor;
class GrGeometryProcessor;
class GrGLFragmentProcessor;
class GrGLGeometryProcessor;

/**
 * Backend processor factory cannot actually create anything, it is up to subclasses to implement
 * a create binding which matches Gr to GL in a type safe way
 */

class GrBackendFragmentProcessorFactory : public GrBackendProcessorFactory {
public:
    /**
     * Creates a GrGLProcessor instance that is used both to generate code for the GrProcessor in a
     * GLSL program and to manage updating uniforms for the program when it is used.
     */
    virtual GrGLFragmentProcessor* createGLInstance(const GrFragmentProcessor&) const = 0;
};

class GrBackendGeometryProcessorFactory : public GrBackendProcessorFactory {
public:
    /**
     * Creates a GrGLProcessor instance that is used both to generate code for the GrProcessor in a
     * GLSL program and to manage updating uniforms for the program when it is used.
     */
    virtual GrGLGeometryProcessor* createGLInstance(const GrGeometryProcessor&) const = 0;
};

#endif
