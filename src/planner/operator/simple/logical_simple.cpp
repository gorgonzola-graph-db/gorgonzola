#include "planner/operator/simple/logical_simple.h"

namespace gorgonzola {
namespace planner {

void LogicalSimple::computeFlatSchema() {
    createEmptySchema();
}

void LogicalSimple::computeFactorizedSchema() {
    createEmptySchema();
}

} // namespace planner
} // namespace gorgonzola
