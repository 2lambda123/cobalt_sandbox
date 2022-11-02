/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef GrYUVtoRGBEffect_DEFINED
#define GrYUVtoRGBEffect_DEFINED

#include "SkImageInfo.h"
#include "GrTextureAccess.h"

class GrFragmentProcessor;
class GrTexture;

namespace GrYUVtoRGBEffect {
    /**
     * Creates an effect that performs color conversion from YUV to RGB
     */
    GrFragmentProcessor* Create(GrTexture* yTexture, GrTexture* uTexture, GrTexture* vTexture,
                                SkYUVColorSpace colorSpace, bool nv12);
    GrFragmentProcessor* Create(GrTexture* yTexture, GrTexture* uTexture, GrTexture* vTexture,
                                const SkMatrix& localMatrix,
                                SkYUVColorSpace colorSpace, bool nv12);

    GrFragmentProcessor* Create(GrTexture* yTexture, GrTexture* uTexture, GrTexture* vTexture,
                                const GrTextureParams& params, SkYUVColorSpace colorSpace, bool nv12);
    GrFragmentProcessor* Create(GrTexture* yTexture, GrTexture* uTexture, GrTexture* vTexture,
                                const SkMatrix& localMatrix, const GrTextureParams& params,
                                SkYUVColorSpace colorSpace, bool nv12);
};

#endif
