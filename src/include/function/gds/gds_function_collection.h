#pragma once

#include "function/gds/rec_joins.h"

namespace gorgonzola {
namespace function {

struct VarLenJoinsFunction {
    static constexpr const char* name = "VAR_LEN_JOINS";

    static std::unique_ptr<RJAlgorithm> getAlgorithm();
};

#if !defined(GORGONZOLA_LITE) || defined(GORGONZOLA_LITE_ENABLE_GDS)
struct AllSPDestinationsFunction {
    static constexpr const char* name = "ALL_SP_DESTINATIONS";

    static std::unique_ptr<RJAlgorithm> getAlgorithm();
};

struct AllSPPathsFunction {
    static constexpr const char* name = "ALL_SP_PATHS";

    static std::unique_ptr<RJAlgorithm> getAlgorithm();
};
#endif

struct SingleSPDestinationsFunction {
    static constexpr const char* name = "SINGLE_SP_DESTINATIONS";

    static std::unique_ptr<RJAlgorithm> getAlgorithm();
};

struct SingleSPPathsFunction {
    static constexpr const char* name = "SINGLE_SP_PATHS";

    static std::unique_ptr<RJAlgorithm> getAlgorithm();
};

#if !defined(GORGONZOLA_LITE) || defined(GORGONZOLA_LITE_ENABLE_GDS)
struct WeightedSPDestinationsFunction {
    static constexpr const char* name = "WEIGHTED_SP_DESTINATIONS";

    static std::unique_ptr<RJAlgorithm> getAlgorithm();
};

struct WeightedSPPathsFunction {
    static constexpr const char* name = "WEIGHTED_SP_PATHS";

    static std::unique_ptr<RJAlgorithm> getAlgorithm();
};

struct AllWeightedSPPathsFunction {
    static constexpr const char* name = "ALL_WEIGHTED_SP_PATHS";

    static std::unique_ptr<RJAlgorithm> getAlgorithm();
};
#endif

} // namespace function
} // namespace gorgonzola
