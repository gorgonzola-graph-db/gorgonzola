#include "function/aggregate/sum.h"

#include "common/types/logical_type_utils.h"

namespace gorgonzola {
namespace function {

using namespace gorgonzola::common;

function_set AggregateSumFunction::getFunctionSet() {
    function_set result;
    for (auto typeID : LogicalTypeUtils::getNumericalLogicalTypeIDs()) {
        AggregateFunctionUtils::appendSumOrAvgFuncs<SumFunction>(name, typeID, result);
    }
    return result;
}

} // namespace function
} // namespace gorgonzola
