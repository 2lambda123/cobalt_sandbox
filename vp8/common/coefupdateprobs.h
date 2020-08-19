/*
 *  Copyright (c) 2010 The WebM project authors. All Rights Reserved.
 *
 *  Use of this source code is governed by a BSD-style license
 *  that can be found in the LICENSE file in the root of the source
 *  tree. An additional intellectual property rights grant can be found
 *  in the file PATENTS.  All contributing project authors may
 *  be found in the AUTHORS file in the root of the source tree.
 */

#ifndef VPX_VP8_COMMON_COEFUPDATEPROBS_H_
#define VPX_VP8_COMMON_COEFUPDATEPROBS_H_

#ifdef __cplusplus
extern "C" {
#endif

/* Update probabilities for the nodes in the token entropy tree.
   Generated file included by entropy.c */

const vp8_prob vp8_coef_update_probs
    [BLOCK_TYPES][COEF_BANDS][PREV_COEF_CONTEXTS][ENTROPY_NODES] = {
      {
          {
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 176, 246, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 223, 241, 252, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 249, 253, 253, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 244, 252, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 234, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 246, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 239, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 254, 255, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 248, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 251, 255, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 251, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 254, 255, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 254, 253, 255, 254, 255, 255, 255, 255, 255, 255 },
              { 250, 255, 254, 255, 254, 255, 255, 255, 255, 255, 255 },
              { 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
      },
      {
          {
              { 217, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 225, 252, 241, 253, 255, 255, 254, 255, 255, 255, 255 },
              { 234, 250, 241, 250, 253, 255, 253, 254, 255, 255, 255 },
          },
          {
              { 255, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 223, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 238, 253, 254, 254, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 248, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 249, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 253, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 247, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 254, 253, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 250, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
      },
      {
          {
              { 186, 251, 250, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 234, 251, 244, 254, 255, 255, 255, 255, 255, 255, 255 },
              { 251, 251, 243, 253, 254, 255, 254, 255, 255, 255, 255 },
          },
          {
              { 255, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 236, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 251, 253, 253, 254, 254, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
      },
      {
          {
              { 248, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 250, 254, 252, 254, 255, 255, 255, 255, 255, 255, 255 },
              { 248, 254, 249, 253, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 253, 253, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 246, 253, 253, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 252, 254, 251, 254, 254, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 254, 252, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 248, 254, 253, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 253, 255, 254, 254, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 251, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 245, 251, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 253, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 251, 253, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 252, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 249, 255, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 254, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 255, 253, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 250, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
          {
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
              { 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 },
          },
      },
    };

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // VPX_VP8_COMMON_COEFUPDATEPROBS_H_
