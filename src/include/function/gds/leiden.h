#pragma once
#include "function/table/bind_data.h"
#include "function/table/bind_input.h"
#include "function/table/table_function.h"

namespace gorgonzola {
namespace function {

struct LeidenFunction {
    static constexpr const char* name = "leiden";

    static function_set getFunctionSet();
};

} // namespace function
} // namespace gorgonzola
