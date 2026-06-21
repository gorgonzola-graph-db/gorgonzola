#pragma once

namespace gorgonzola {
namespace common {
struct DatabaseLifeCycleManager {
    bool isDatabaseClosed = false;
    void checkDatabaseClosedOrThrow() const;
};
} // namespace common
} // namespace gorgonzola
