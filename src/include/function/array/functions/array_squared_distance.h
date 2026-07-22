#pragma once

#include "common/exception/runtime.h"
#include "common/vector/value_vector.h"
#ifndef GORGONZOLA_LITE
#include <simsimd.h>
#endif

namespace gorgonzola {
namespace function {

struct ArraySquaredDistance {
    template<std::floating_point T>
    static inline void operation(common::list_entry_t& left, common::list_entry_t& right, T& result,
        common::ValueVector& leftVector, common::ValueVector& rightVector,
        common::ValueVector& /*resultVector*/) {
#ifdef GORGONZOLA_LITE
        (void)left;
        (void)right;
        (void)result;
        (void)leftVector;
        (void)rightVector;
        throw common::RuntimeException("Vector operations are not supported in Gorgonzola Lite.");
#else
        auto leftElements = (T*)common::ListVector::getListValues(&leftVector, left);
        auto rightElements = (T*)common::ListVector::getListValues(&rightVector, right);
        KU_ASSERT(left.size == right.size);
        simsimd_distance_t tmpResult = 0.0;
        static_assert(std::is_same_v<T, float> || std::is_same_v<T, double>);
        if constexpr (std::is_same_v<T, float>) {
            simsimd_l2sq_f32(leftElements, rightElements, left.size, &tmpResult);
        } else {
            simsimd_l2sq_f64(leftElements, rightElements, left.size, &tmpResult);
        }
        result = tmpResult;
#endif
    }
};

} // namespace function
} // namespace gorgonzola
