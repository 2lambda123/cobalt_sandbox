/*
 * Copyright 2014 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#ifndef SkMultiPictureDraw_DEFINED
#define SkMultiPictureDraw_DEFINED

#include "SkMatrix.h"
#include "SkTDArray.h"

class SkCanvas;
class SkPaint;
class SkPicture;

/** \class SkMultiPictureDraw

    The MultiPictureDraw object accepts several picture/canvas pairs and
    then attempts to optimally draw the pictures into the canvases, sharing
    as many resources as possible.
*/
class SK_API SkMultiPictureDraw {
public:
    /**
     *  Create an object to optimize the drawing of multiple pictures.
     *  @param reserve Hint for the number of add calls expected to be issued
     */
    SkMultiPictureDraw(int reserve = 0);
    ~SkMultiPictureDraw() { this->reset(); }

    /**
     *  Add a canvas/picture pair for later rendering.
     *  @param canvas   the canvas in which to draw picture
     *  @param picture  the picture to draw into canvas
     *  @param matrix   if non-NULL, applied to the CTM when drawing
     *  @param paint    if non-NULL, draw picture to a temporary buffer
     *                  and then apply the paint when the result is drawn
     */
    void add(SkCanvas* canvas,
             const SkPicture* picture,
             const SkMatrix* matrix = NULL, 
             const SkPaint* paint = NULL);

    /**
     *  Perform all the previously added draws. This will reset the state
     *  of this object.
     */
    void draw();

    /**
     *  Abandon all buffered draws and reset to the initial state.
     */
    void reset();

private:
    struct DrawData {
        SkCanvas*        canvas;  // reffed
        const SkPicture* picture; // reffed
        SkMatrix         matrix;
        SkPaint*         paint;   // owned
    };

    SkTDArray<DrawData> fDrawData;
};

#endif
