#pragma once

#include "common/profiler.h"

namespace gorgonzola {
namespace main {
class ClientContext;
}
namespace processor {

struct GORGONZOLA_API ExecutionContext {
    uint64_t queryID;
    common::Profiler* profiler;
    main::ClientContext* clientContext;

    ExecutionContext(common::Profiler* profiler, main::ClientContext* clientContext,
        uint64_t queryID)
        : queryID{queryID}, profiler{profiler}, clientContext{clientContext} {}
};

} // namespace processor
} // namespace gorgonzola
