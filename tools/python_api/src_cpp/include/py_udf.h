#pragma once

#include <string>

#include "common/types/types.h"
#include "function/function.h"
#include "pybind_include.h"

using gorgonzola::common::LogicalTypeID;
using gorgonzola::function::function_set;

namespace gorgonzola {
namespace main {
class ClientContext;
} // namespace main
} // namespace gorgonzola

class PyUDF {

public:
    static function_set toFunctionSet(const std::string& name, const py::function& udf,
        const py::list& paramTypes, const std::string& resultType, bool defaultNull,
        bool catchExceptions, gorgonzola::main::ClientContext* context);
};
