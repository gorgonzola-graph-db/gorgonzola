#include "function/aggregate/avg.h"

#include "common/types/logical_type_utils.h"

namespace gorgonzola {
namespace function {

using namespace gorgonzola::common;

function_set AggregateAvgFunction::getFunctionSet() {
    function_set result;
    for (auto typeID : LogicalTypeUtils::getNumericalLogicalTypeIDs()) {
        AggregateFunctionUtils::appendSumOrAvgFuncs<AvgFunction>(name, typeID, result);
    }
    return result;
}

} // namespace function
} // namespace gorgonzola
