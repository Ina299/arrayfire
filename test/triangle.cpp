/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <gtest/gtest.h>
#include <arrayfire.h>
#include <af/dim4.hpp>
#include <af/defines.h>
#include <af/traits.hpp>
#include <af/data.h>
#include <vector>
#include <iostream>
#include <complex>
#include <string>
#include <testHelpers.hpp>

using std::vector;
using std::string;
using std::cout;
using std::endl;
using af::cfloat;
using af::cdouble;
using af::dim4;

template<typename T>
class Triangle : public ::testing::Test { };

typedef ::testing::Types<float, af::cfloat, double, af::cdouble, int, unsigned, char, uchar, uintl, intl> TestTypes;
TYPED_TEST_CASE(Triangle, TestTypes);

template<typename T>
void triangleTester(const dim4 dims, bool is_upper)
{
    if (noDoubleTests<T>()) return;
#if 1
    af::array in = cpu_randu<T>(dims);
#else
    af::array in = af::randu(dims, (af::dtype)af::dtype_traits<T>::af_type);
#endif

    T *h_in = in.host<T>();
    af::array out = is_upper ?  upper(in) : lower(in);
    T *h_out = out.host<T>();

    int m = dims[0];
    int n = dims[1];

    for (int z = 0; z < (int)(dims[2] * dims[3]); z++) {
        int z_off = z * m * n;

        for (int y = 0; y < n; y++) {
            int y_off = z_off + y * m;

            for (int x = 0; x < m; x++) {
                T val = T(0);
                if (((y <= x) && !is_upper) ||
                    ((y >= x) &&  is_upper)) {
                    val = h_in[y_off + x];
                }

                ASSERT_EQ(h_out[y_off + x], val) << "at (" << x << ", " << y << ")";
            }
        }
    }

    delete[] h_in;
    delete[] h_out;
}

TYPED_TEST(Triangle, Lower2DRect0)
{
    triangleTester<TypeParam>(dim4(500, 600), false);
}

TYPED_TEST(Triangle, Lower2DRect1)
{
    triangleTester<TypeParam>(dim4(2003, 1775), false);
}

TYPED_TEST(Triangle, Lower2DSquare)
{
    triangleTester<TypeParam>(dim4(2048, 2048), false);
}

TYPED_TEST(Triangle, Lower3D)
{
    triangleTester<TypeParam>(dim4(1000, 1000, 5), false);
}

TYPED_TEST(Triangle, Lower4D)
{
    triangleTester<TypeParam>(dim4(600, 900, 5, 2), false);
}

TYPED_TEST(Triangle, Upper2DRect0)
{
    triangleTester<TypeParam>(dim4(500, 600), true);
}

TYPED_TEST(Triangle, Upper2DRect1)
{
    triangleTester<TypeParam>(dim4(2003, 1775), true);
}

TYPED_TEST(Triangle, Upper2DSquare)
{
    triangleTester<TypeParam>(dim4(2048, 2048), true);
}

TYPED_TEST(Triangle, Upper3D)
{
    triangleTester<TypeParam>(dim4(1000, 1000, 5), true);
}

TYPED_TEST(Triangle, Upper4D)
{
    triangleTester<TypeParam>(dim4(600, 900, 5, 2), true);
}
