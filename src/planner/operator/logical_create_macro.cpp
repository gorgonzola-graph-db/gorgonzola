#include "planner/operator/logical_create_macro.h"

namespace gorgonzola {
namespace planner {

std::string LogicalCreateMacroPrintInfo::toString() const {
    std::string result = "Macro: ";
    result += macroName;
    return result;
}

void LogicalCreateMacro::computeFlatSchema() {
    createEmptySchema();
}

void LogicalCreateMacro::computeFactorizedSchema() {
    createEmptySchema();
}

} // namespace planner
} // namespace gorgonzola
