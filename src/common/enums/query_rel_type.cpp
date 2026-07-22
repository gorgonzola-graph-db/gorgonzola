#include "common/enums/query_rel_type.h"

#include "common/assert.h"
#include "common/exception/runtime.h"
#include "function/gds/gds_function_collection.h"

using namespace gorgonzola::function;

namespace gorgonzola {
namespace common {

PathSemantic QueryRelTypeUtils::getPathSemantic(QueryRelType queryRelType) {
    switch (queryRelType) {
    case QueryRelType::VARIABLE_LENGTH_WALK:
        return PathSemantic::WALK;
    case QueryRelType::VARIABLE_LENGTH_TRAIL:
        return PathSemantic::TRAIL;
    case QueryRelType::VARIABLE_LENGTH_ACYCLIC:
    case QueryRelType::SHORTEST:
    case QueryRelType::ALL_SHORTEST:
    case QueryRelType::WEIGHTED_SHORTEST:
    case QueryRelType::ALL_WEIGHTED_SHORTEST:
        return PathSemantic::ACYCLIC;
    default:
        KU_UNREACHABLE;
    }
}

std::unique_ptr<function::RJAlgorithm> QueryRelTypeUtils::getFunction(QueryRelType type) {
    switch (type) {
    case QueryRelType::VARIABLE_LENGTH_WALK:
    case QueryRelType::VARIABLE_LENGTH_TRAIL:
    case QueryRelType::VARIABLE_LENGTH_ACYCLIC: {
        return VarLenJoinsFunction::getAlgorithm();
    }
    case QueryRelType::SHORTEST: {
        return SingleSPPathsFunction::getAlgorithm();
    }
    case QueryRelType::ALL_SHORTEST: {
#ifdef GORGONZOLA_LITE
        throw common::RuntimeException("All-shortest-paths is not available in Gorgonzola Lite");
#else
        return AllSPPathsFunction::getAlgorithm();
#endif
    }
    case QueryRelType::WEIGHTED_SHORTEST: {
#ifdef GORGONZOLA_LITE
        throw common::RuntimeException(
            "Weighted shortest path is not available in Gorgonzola Lite");
#else
        return WeightedSPPathsFunction::getAlgorithm();
#endif
    }
    case QueryRelType::ALL_WEIGHTED_SHORTEST: {
#ifdef GORGONZOLA_LITE
        throw common::RuntimeException(
            "All-weighted shortest paths is not available in Gorgonzola Lite");
#else
        return AllWeightedSPPathsFunction::getAlgorithm();
#endif
    }
    default:
        KU_UNREACHABLE;
    }
}

} // namespace common
} // namespace gorgonzola
