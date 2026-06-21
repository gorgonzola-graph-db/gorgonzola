#include "planner/planner.h"

using namespace gorgonzola::binder;

namespace gorgonzola {
namespace planner {

LogicalPlan Planner::getNodePropertyScanPlan(const NodeExpression& node) {
    auto properties = getProperties(node);
    auto scanPlan = LogicalPlan();
    if (properties.empty()) {
        return scanPlan;
    }
    appendScanNodeTable(node.getInternalID(), node.getTableIDs(), properties, scanPlan);
    return scanPlan;
}

} // namespace planner
} // namespace gorgonzola
