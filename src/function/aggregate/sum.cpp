#include "function/aggregate/sum.h"

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
