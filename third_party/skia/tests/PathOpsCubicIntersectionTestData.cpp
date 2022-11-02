/*
 * Copyright 2012 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "PathOpsCubicIntersectionTestData.h"

#include <limits>

static const double D = FLT_EPSILON / 2;
static const double G = FLT_EPSILON / 3;
static const double N = -FLT_EPSILON / 2;
static const double M = -FLT_EPSILON / 3;

const SkDCubic pointDegenerates[] = {
    {{{0, 0}, {0, 0}, {0, 0}, {0, 0}}},
    {{{1, 1}, {1, 1}, {1, 1}, {1, 1}}},
    {{{1 + FLT_EPSILON_HALF, 1}, {1, 1 + FLT_EPSILON_HALF}, {1, 1}, {1, 1}}},
    {{{1 + D, 1}, {1 - D, 1}, {1, 1}, {1, 1}}},
    {{{0, 0}, {0, 0}, {1, 0}, {0, 0}}},
    {{{0, 0}, {1, 0}, {0, 0}, {0, 0}}},
    {{{0, 0}, {0, 0}, {0, 1}, {0, 0}}},
    {{{0, 0}, {0, 1}, {0, 0}, {0, 0}}},
    {{{0, 0}, {0, 0}, {1, 1}, {0, 0}}},
    {{{0, 0}, {1, 1}, {0, 0}, {0, 0}}},
    {{{0, 0}, {1, 1}, {2, 2}, {0, 0}}},
    {{{1, 1}, {2, 2}, {2, 2}, {1, 1}}},
    {{{0, 0}, {0, D}, {1, 0}, {0, 0}}},
    {{{0, 0}, {1, 0}, {0, D}, {0, 0}}},
    {{{0, 0}, {D, 0}, {0, 1}, {0, 0}}},
    {{{0, 0}, {0, 1}, {D, 0}, {0, 0}}},
    {{{1, 1}, {2, 2}, {2, 2+D}, {1, 1}}},
    {{{0, 0}, {0, N}, {1, 0}, {0, 0}}},
    {{{0, 0}, {1, 0}, {0, N}, {0, 0}}},
    {{{0, 0}, {N, 0}, {0, 1}, {0, 0}}},
    {{{0, 0}, {0, 1}, {N, 0}, {0, 0}}},
    {{{0, 0}, {1, 1}, {N, 0}, {0, 0}}},
    {{{0, 0}, {D, 0}, {1, 1}, {0, 0}}},
    {{{0, 0}, {1, 1}, {D, 0}, {0, 0}}},
    {{{0, 0}, {N, 0}, {1, 1}, {0, 0}}},
    {{{1, 1}, {2, 2}, {2, 2+N}, {1, 1}}},
};

const size_t pointDegenerates_count = SK_ARRAY_COUNT(pointDegenerates);

const SkDCubic notPointDegenerates[] = {
    {{{1 + FLT_EPSILON * 2, 1}, {1, FLT_EPSILON * 2}, {1, 1}, {1, 1}}},
    {{{1 + FLT_EPSILON * 2, 1}, {1 - FLT_EPSILON * 2, 1}, {1, 1}, {1, 1}}}
};

const size_t notPointDegenerates_count =
        SK_ARRAY_COUNT(notPointDegenerates);

// from http://www.truetex.com/bezint.htm
const SkDCubic tests[][2] = {
    {  // intersects in one place (data gives bezier clip fits
     {{{0, 45},
      {6.0094158284751593, 51.610357411322688},
      {12.741093228940867, 55.981703949474607},
      {20.021417396476362, 58.652245509710262}}},
     {{{2.2070737699246674, 52.703494107327209},
      {31.591482272629477, 23.811002295222025},
      {76.824588616426425, 44.049473790502674},
      {119.25488947221436, 55.599248272955073}}}
    }, {  // intersects in three places
        {{{0, 45}, {50, 100}, {150,   0}, {200, 55}}},
        {{{0, 55}, {50,   0}, {150, 100}, {200, 45}}}
    }, {  // intersects in one place, cross over is nearly parallel
        {{{0,   0}, {0, 100}, {200,   0}, {200, 100}}},
        {{{0, 100}, {0,   0}, {200, 100}, {200,   0}}}
    }, {  // intersects in two places
        {{{0,   0}, {0, 100}, {200, 100}, {200,   0}}},
        {{{0, 100}, {0,   0}, {200,   0}, {200, 100}}}
    }, {
        {{{150, 100}, {150 + 0.1, 150}, {150, 200}, {150, 250}}},
        {{{250, 150}, {200, 150 + 0.1}, {150, 150}, {100, 150}}}
    }, {  // single intersection around 168,185
        {{{200, 100}, {150, 100}, {150, 150}, {200, 150}}},
        {{{250, 150}, {250, 100}, {100, 100}, {100, 150}}}
    }, {
        {{{1.0, 1.5}, {15.5, 0.5}, {-8.0, 3.5}, {5.0, 1.5}}},
        {{{4.0, 0.5}, {5.0, 15.0}, {2.0, -8.5}, {4.0, 4.5}}}
    }, {
        {{{664.00168, 0},       {726.11545, 124.22757}, {736.89069, 267.89743},
                {694.0017, 400.0002}}},
        {{{850.66843, 115.55563}, {728.515, 115.55563}, {725.21347, 275.15309},
                {694.0017, 400.0002}}}
    }, {
        {{{1, 1},   {12.5, 6.5}, {-4, 6.5}, {7.5, 1}}},
        {{{1, 6.5}, {12.5, 1},   {-4, 1},   {.5, 6}}}
    }, {
        {{{315.748, 312.84}, {312.644, 318.134}, {305.836, 319.909}, {300.542, 316.804}}},
        {{{317.122, 309.05}, {316.112, 315.102}, {310.385, 319.19},  {304.332, 318.179}}}
    }, {
        {{{1046.604051, 172.937967},  {1046.604051, 178.9763059}, {1041.76745,  183.9279165}, {1035.703842, 184.0432409}}},
        {{{1046.452235, 174.7640504}, {1045.544872, 180.1973817}, {1040.837966, 184.0469882}, {1035.505925, 184.0469882}}}
    }, {
        {{{125.79356, 199.57382}, {51.16556, 128.93575}, {87.494,  16.67848}, {167.29361, 16.67848}}},
        {{{167.29361, 55.81876}, {100.36128, 55.81876}, {68.64099, 145.4755}, {125.7942, 199.57309}}}
    }, {
        {{{104.11546583642826, 370.21352558595504}, {122.96968232592344, 404.54489231839295},
          {169.90881005384728, 425.00067000000007}, {221.33045999999999, 425.00067000000001}}},
        {{{116.32365976159625, 381.71048540582598}, {103.86096590870899, 381.71048540581626},
          {91.394188003200725, 377.17917781762833}, {82.622283093355179, 368.11683661930334}}}
    }
};

const size_t tests_count = SK_ARRAY_COUNT(tests);

const SkDCubic lines[] = {
    {{{0, 0}, {0, 0}, {0, 0}, {1, 0}}},  // 0: horizontal
    {{{1, 0}, {0, 0}, {0, 0}, {0, 0}}},
    {{{1, 0}, {2, 0}, {3, 0}, {4, 0}}},
    {{{0, 0}, {0, 0}, {0, 0}, {0, 1}}},  // 5: vertical
    {{{0, 1}, {0, 0}, {0, 0}, {0, 0}}},
    {{{0, 1}, {0, 2}, {0, 3}, {0, 4}}},
    {{{0, 0}, {0, 0}, {0, 0}, {1, 1}}},  // 10: 3 coincident
    {{{1, 1}, {0, 0}, {0, 0}, {0, 0}}},
    {{{0, 0}, {0, 0}, {1, 1}, {2, 2}}},  // 14: 2 coincident
    {{{0, 0}, {1, 1}, {0, 0}, {2, 2}}},
    {{{1, 1}, {0, 0}, {0, 0}, {2, 2}}},  // 17:
    {{{1, 1}, {0, 0}, {2, 2}, {0, 0}}},
    {{{1, 1}, {2, 2}, {0, 0}, {0, 0}}},
    {{{1, 1}, {2, 2}, {3, 3}, {2, 2}}},  // middle-last coincident
    {{{1, 1}, {2, 2}, {3, 3}, {3, 3}}},  // middle-last coincident
    {{{1, 1}, {1, 1}, {2, 2}, {2, 2}}},  // 2 pairs coincident
    {{{1, 1}, {2, 2}, {1, 1}, {2, 2}}},
    {{{1, 1}, {1, 1}, {3, 3}, {3, 3}}},  // first-middle middle-last coincident
    {{{1, 1}, {2, 2}, {3, 3}, {4, 4}}},  // no coincident
    {{{1, 1}, {3, 3}, {2, 2}, {4, 4}}},
    {{{1, 1}, {2, 2}, {4, 4}, {3, 3}}},
    {{{1, 1}, {3, 3}, {4, 4}, {2, 2}}},
    {{{1, 1}, {4, 4}, {2, 2}, {3, 3}}},
    {{{1, 1}, {4, 4}, {3, 3}, {2, 2}}},
    {{{2, 2}, {1, 1}, {3, 3}, {4, 4}}},
    {{{2, 2}, {1, 1}, {4, 4}, {3, 3}}},
    {{{2, 2}, {3, 3}, {1, 1}, {4, 4}}},
    {{{2, 2}, {3, 3}, {4, 4}, {1, 1}}},
    {{{2, 2}, {4, 4}, {1, 1}, {3, 3}}},
    {{{2, 2}, {4, 4}, {3, 3}, {1, 1}}},
};

const size_t lines_count = SK_ARRAY_COUNT(lines);

// 'not a line' tries to fool the line detection code
const SkDCubic notLines[] = {
    {{{0, 0}, {0, 0}, {0, 1}, {1, 0}}},
    {{{0, 0}, {0, 1}, {0, 0}, {1, 0}}},
    {{{0, 0}, {0, 1}, {1, 0}, {0, 0}}},
    {{{0, 1}, {0, 0}, {0, 0}, {1, 0}}},
    {{{0, 1}, {0, 0}, {1, 0}, {0, 0}}},
    {{{0, 1}, {1, 0}, {0, 0}, {0, 0}}},
};

const size_t notLines_count = SK_ARRAY_COUNT(notLines);

static const double E = FLT_EPSILON * 2;
static const double F = FLT_EPSILON * 3;

const SkDCubic modEpsilonLines[] = {
    {{{0, E}, {0, 0}, {0, 0}, {1, 0}}},  // horizontal
    {{{0, 0}, {0, E}, {1, 0}, {0, 0}}},
    {{{0, 0}, {1, 0}, {0, E}, {0, 0}}},
    {{{1, 0}, {0, 0}, {0, 0}, {0, E}}},
    {{{1, E}, {2, 0}, {3, 0}, {4, 0}}},
    {{{E, 0}, {0, 0}, {0, 0}, {0, 1}}},  // vertical
    {{{0, 0}, {E, 0}, {0, 1}, {0, 0}}},
    {{{0, 0}, {0, 1}, {E, 0}, {0, 0}}},
    {{{0, 1}, {0, 0}, {0, 0}, {E, 0}}},
    {{{E, 1}, {0, 2}, {0, 3}, {0, 4}}},
    {{{E, 0}, {0, 0}, {0, 0}, {1, 1}}},  // 3 coincident
    {{{0, 0}, {E, 0}, {1, 1}, {0, 0}}},
    {{{0, 0}, {1, 1}, {E, 0}, {0, 0}}},
    {{{1, 1}, {0, 0}, {0, 0}, {E, 0}}},
    {{{0, E}, {0, 0}, {1, 1}, {2, 2}}},  // 2 coincident
    {{{0, 0}, {1, 1}, {0, E}, {2, 2}}},
    {{{0, 0}, {1, 1}, {2, 2}, {0, E}}},
    {{{1, 1}, {0, E}, {0, 0}, {2, 2}}},
    {{{1, 1}, {0, E}, {2, 2}, {0, 0}}},
    {{{1, 1}, {2, 2}, {E, 0}, {0, 0}}},
    {{{1, 1}, {2, 2+E}, {3, 3}, {2, 2}}},  // middle-last coincident
    {{{1, 1}, {2+E, 2}, {3, 3}, {3, 3}}},  // middle-last coincident
    {{{1, 1}, {1, 1}, {2, 2}, {2+E, 2}}},  // 2 pairs coincident
    {{{1, 1}, {2, 2}, {1, 1}, {2+E, 2}}},
    {{{1, 1}, {2, 2}, {2, 2+E}, {1, 1}}},
    {{{1, 1}, {1, 1+E}, {3, 3}, {3, 3}}},  // first-middle middle-last coincident
    {{{1, 1}, {2+E, 2}, {3, 3}, {4, 4}}},  // no coincident
    {{{1, 1}, {3, 3}, {2, 2}, {4, 4+F}}},  // INVESTIGATE: why the epsilon is bigger
    {{{1, 1+F}, {2, 2}, {4, 4}, {3, 3}}},  // INVESTIGATE: why the epsilon is bigger
    {{{1, 1}, {3, 3}, {4, 4+E}, {2, 2}}},
    {{{1, 1}, {4, 4}, {2, 2}, {3, 3+E}}},
    {{{1, 1}, {4, 4}, {3, 3}, {2+E, 2}}},
    {{{2, 2}, {1, 1}, {3+E, 3}, {4, 4}}},
    {{{2, 2}, {1+E, 1}, {4, 4}, {3, 3}}},
    {{{2, 2+E}, {3, 3}, {1, 1}, {4, 4}}},
    {{{2+E, 2}, {3, 3}, {4, 4}, {1, 1}}},
    {{{2, 2}, {4+E, 4}, {1, 1}, {3, 3}}},
    {{{2, 2}, {4, 4}, {3, 3}, {1, 1+E}}},
};

const size_t modEpsilonLines_count = SK_ARRAY_COUNT(modEpsilonLines);

const SkDCubic lessEpsilonLines[] = {
    {{{0, D}, {0, 0}, {0, 0}, {1, 0}}},  // horizontal
    {{{1, 0}, {0, 0}, {0, 0}, {0, D}}},
    {{{1, D}, {2, 0}, {3, 0}, {4, 0}}},
    {{{D, 0}, {0, 0}, {0, 0}, {0, 1}}},  // vertical
    {{{0, 1}, {0, 0}, {0, 0}, {D, 0}}},
    {{{D, 1}, {0, 2}, {0, 3}, {0, 4}}},
    {{{D, 0}, {0, 0}, {0, 0}, {1, 1}}},  // 3 coincident
    {{{1, 1}, {0, 0}, {0, 0}, {D, 0}}},
    {{{0, D}, {0, 0}, {1, 1}, {2, 2}}},  // 2 coincident
    {{{0, 0}, {1, 1}, {0, D}, {2, 2}}},
    {{{0, 0}, {1, 1}, {2, 2}, {1, 1+D}}},
    {{{1, 1}, {0, D}, {0, 0}, {2, 2}}},
    {{{1, 1}, {0, D}, {2, 2}, {0, 0}}},
    {{{1, 1}, {2, 2}, {D, 0}, {0, 0}}},
    {{{1, 1}, {2, 2+D}, {3, 3}, {2, 2}}},  // middle-last coincident
    {{{1, 1}, {2+D, 2}, {3, 3}, {3, 3}}},  // middle-last coincident
    {{{1, 1}, {1, 1}, {2, 2}, {2+D, 2}}},  // 2 pairs coincident
    {{{1, 1}, {2, 2}, {1, 1}, {2+D, 2}}},
    {{{1, 1}, {1, 1+D}, {3, 3}, {3, 3}}},  // first-middle middle-last coincident
    {{{1, 1}, {2+D/2, 2}, {3, 3}, {4, 4}}},  // no coincident (FIXME: N as opposed to N/2 failed)
    {{{1, 1}, {3, 3}, {2, 2}, {4, 4+D}}},
    {{{1, 1+D}, {2, 2}, {4, 4}, {3, 3}}},
    {{{1, 1}, {3, 3}, {4, 4+D}, {2, 2}}},
    {{{1, 1}, {4, 4}, {2, 2}, {3, 3+D}}},
    {{{1, 1}, {4, 4}, {3, 3}, {2+G, 2}}},  // INVESTIGATE: why the epsilon is smaller
    {{{2, 2}, {1, 1}, {3+D, 3}, {4, 4}}},
    {{{2, 2}, {1+D, 1}, {4, 4}, {3, 3}}},
    {{{2, 2+D}, {3, 3}, {1, 1}, {4, 4}}},
    {{{2+G, 2}, {3, 3}, {4, 4}, {1, 1}}},  // INVESTIGATE: why the epsilon is smaller
    {{{2, 2}, {4+D, 4}, {1, 1}, {3, 3}}},
    {{{2, 2}, {4, 4}, {3, 3}, {1, 1+D}}},
};

const size_t lessEpsilonLines_count = SK_ARRAY_COUNT(lessEpsilonLines);

const SkDCubic negEpsilonLines[] = {
    {{{0, N}, {0, 0}, {0, 0}, {1, 0}}},  // horizontal
    {{{1, 0}, {0, 0}, {0, 0}, {0, N}}},
    {{{1, N}, {2, 0}, {3, 0}, {4, 0}}},
    {{{N, 0}, {0, 0}, {0, 0}, {0, 1}}},  // vertical
    {{{0, 1}, {0, 0}, {0, 0}, {N, 0}}},
    {{{N, 1}, {0, 2}, {0, 3}, {0, 4}}},
    {{{N, 0}, {0, 0}, {0, 0}, {1, 1}}},  // 3 coincident
    {{{1, 1}, {0, 0}, {0, 0}, {N, 0}}},
    {{{0, N}, {0, 0}, {1, 1}, {2, 2}}},  // 2 coincident
    {{{0, 0}, {1, 1}, {0, N}, {2, 2}}},
    {{{0, 0}, {1, 1}, {2, 2}, {1, 1+N}}},
    {{{1, 1}, {0, N}, {0, 0}, {2, 2}}},
    {{{1, 1}, {0, N}, {2, 2}, {0, 0}}},
    {{{1, 1}, {2, 2}, {N, 0}, {0, 0}}},
    {{{1, 1}, {2, 2+N}, {3, 3}, {2, 2}}},  // middle-last coincident
    {{{1, 1}, {2+N, 2}, {3, 3}, {3, 3}}},  // middle-last coincident
    {{{1, 1}, {1, 1}, {2, 2}, {2+N, 2}}},  // 2 pairs coincident
    {{{1, 1}, {2, 2}, {1, 1}, {2+N, 2}}},
    {{{1, 1}, {1, 1+N}, {3, 3}, {3, 3}}},  // first-middle middle-last coincident
    {{{1, 1}, {2+N/2, 2}, {3, 3}, {4, 4}}},  // no coincident (FIXME: N as opposed to N/2 failed)
    {{{1, 1}, {3, 3}, {2, 2}, {4, 4+N}}},
    {{{1, 1+N}, {2, 2}, {4, 4}, {3, 3}}},
    {{{1, 1}, {3, 3}, {4, 4+N}, {2, 2}}},
    {{{1, 1}, {4, 4}, {2, 2}, {3, 3+N}}},
    {{{1, 1}, {4, 4}, {3, 3}, {2+M, 2}}},  // INVESTIGATE: why the epsilon is smaller
    {{{2, 2}, {1, 1}, {3+N, 3}, {4, 4}}},
    {{{2, 2}, {1+N, 1}, {4, 4}, {3, 3}}},
    {{{2, 2+N}, {3, 3}, {1, 1}, {4, 4}}},
    {{{2+M, 2}, {3, 3}, {4, 4}, {1, 1}}},  // INVESTIGATE: why the epsilon is smaller
    {{{2, 2}, {4+N, 4}, {1, 1}, {3, 3}}},
    {{{2, 2}, {4, 4}, {3, 3}, {1, 1+N}}},
};

const size_t negEpsilonLines_count = SK_ARRAY_COUNT(negEpsilonLines);
