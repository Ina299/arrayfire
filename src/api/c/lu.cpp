/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <af/array.h>
#include <af/lapack.h>
#include <af/defines.h>
#include <err_common.hpp>
#include <handle.hpp>
#include <backend.hpp>
#include <ArrayInfo.hpp>
#include <lu.hpp>

using af::dim4;
using namespace detail;

template<typename T>
static inline void lu(af_array *lower, af_array *upper, af_array *pivot,
                      const af_array in)
{
    Array<T> lowerArray = createEmptyArray<T>(af::dim4());
    Array<T> upperArray = createEmptyArray<T>(af::dim4());
    Array<int> pivotArray = createEmptyArray<int>(af::dim4());

    lu<T>(lowerArray, upperArray, pivotArray, getArray<T>(in));

    *lower = getHandle(lowerArray);
    *upper = getHandle(upperArray);
    *pivot = getHandle(pivotArray);
}

template<typename T>
static inline af_array lu_inplace(af_array in)
{
    return getHandle(lu_inplace<T>(getWritableArray<T>(in)));
}

af_err af_lu(af_array *lower, af_array *upper, af_array *pivot, const af_array in)
{
    try {
        ArrayInfo i_info = getInfo(in);

        af_dtype type = i_info.getType();

        ARG_ASSERT(3, i_info.isFloating());                       // Only floating and complex types

        switch(type) {
            case f32: lu<float  >(lower, upper, pivot, in);  break;
            case f64: lu<double >(lower, upper, pivot, in);  break;
            case c32: lu<cfloat >(lower, upper, pivot, in);  break;
            case c64: lu<cdouble>(lower, upper, pivot, in);  break;
            default:  TYPE_ERROR(1, type);
        }
    }
    CATCHALL;

    return AF_SUCCESS;
}

af_err af_lu_inplace(af_array *pivot, af_array in)
{
    try {
        ArrayInfo i_info = getInfo(in);
        af_dtype type = i_info.getType();

        ARG_ASSERT(1, i_info.isFloating()); // Only floating and complex types

        af_array out;

        switch(type) {
            case f32: out = lu_inplace<float  >(in);  break;
            case f64: out = lu_inplace<double >(in);  break;
            case c32: out = lu_inplace<cfloat >(in);  break;
            case c64: out = lu_inplace<cdouble>(in);  break;
            default:  TYPE_ERROR(1, type);
        }
        if(pivot != NULL)
            std::swap(*pivot, out);
    }
    CATCHALL;

    return AF_SUCCESS;
}
