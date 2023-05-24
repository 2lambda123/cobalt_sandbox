//===----------------------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

// UNSUPPORTED: c++98, c++03

// <valarray>

// template<class T> class valarray;

// valarray& operator=(valarray&& v);

#include <valarray>
#include <cassert>
#include <cstddef>

#include "test_macros.h"

int main(int, char**)
{
    {
        typedef int T;
        T a[] = {1, 2, 3, 4, 5};
        const unsigned N = sizeof(a)/sizeof(a[0]);
        std::valarray<T> v(a, N);
        std::valarray<T> v2;
        v2 = std::move(v);
        assert(v2.size() == N);
        assert(v.size() == 0);
        for (std::size_t i = 0; i < v2.size(); ++i)
            assert(v2[i] == a[i]);
    }
    {
        typedef double T;
        T a[] = {1, 2.5, 3, 4.25, 5};
        const unsigned N = sizeof(a)/sizeof(a[0]);
        std::valarray<T> v(a, N);
        std::valarray<T> v2;
        v2 = std::move(v);
        assert(v2.size() == N);
        assert(v.size() == 0);
        for (std::size_t i = 0; i < v2.size(); ++i)
            assert(v2[i] == a[i]);
    }
    {
        typedef std::valarray<double> T;
        T a[] = {T(1), T(2), T(3), T(4), T(5)};
        const unsigned N = sizeof(a)/sizeof(a[0]);
        std::valarray<T> v(a, N);
        std::valarray<T> v2(a, N-2);
        v2 = std::move(v);
        assert(v2.size() == N);
        assert(v.size() == 0);
        for (unsigned i = 0; i < N; ++i)
        {
            assert(v2[i].size() == a[i].size());
            for (std::size_t j = 0; j < a[i].size(); ++j)
                assert(v2[i][j] == a[i][j]);
        }
    }

  return 0;
}
