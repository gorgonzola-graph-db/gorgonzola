#pragma once
#include "gorgonzola.h"
#include "common/system_config.h"
#include "main/client_config.h"

// Define KUZU_PAGE_SIZE alias inside gorgonzola::common
namespace gorgonzola {
namespace common {
    constexpr uint64_t KUZU_PAGE_SIZE = GORGONZOLA_PAGE_SIZE;
}
}

// Define real namespace kuzu and sub-namespaces with using directives
namespace kuzu {
    using namespace gorgonzola;
    namespace common {
        using namespace gorgonzola::common;
        using Value = ::gorgonzola::common::Value;
        using LogicalType = ::gorgonzola::common::LogicalType;
        using Exception = ::gorgonzola::common::Exception;
        using InternalException = ::gorgonzola::common::InternalException;
        using ValueVector = ::gorgonzola::common::ValueVector;
        using DataChunk = ::gorgonzola::common::DataChunk;
        using DataChunkState = ::gorgonzola::common::DataChunkState;
        using HashIndexConstants = ::gorgonzola::common::HashIndexConstants;
    }
    namespace planner {
        using namespace gorgonzola::planner;
        using LogicalPlan = ::gorgonzola::planner::LogicalPlan;
    }
    namespace processor {
        using namespace gorgonzola::processor;
        using FlatTuple = ::gorgonzola::processor::FlatTuple;
    }
    namespace main {
        using namespace gorgonzola::main;
        using ClientConfig = ::gorgonzola::main::ClientConfig;
    }
    namespace testing {
        // Empty to avoid namespace ambiguity for TestHelper, DBTest, etc.
    }
}

#ifndef KUZU_ROOT_DIRECTORY
#define KUZU_ROOT_DIRECTORY GORGONZOLA_ROOT_DIRECTORY
#endif

#ifndef KUZU_VERSION
#define KUZU_VERSION ::gorgonzola::main::Version::getVersion()
#endif
