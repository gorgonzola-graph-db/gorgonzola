#pragma once

// ── Standard Library ──────────────────────────────────────────────
// Sorted alphabetically. These are the most frequently included
// stdlib headers across the 738 .cpp TUs in the project.
#include <algorithm>
#include <atomic>
#include <concepts>
#include <cstdint>
#include <cstring>
#include <format>
#include <functional>
#include <memory>
#include <mutex>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <vector>

// ── Stable project headers ────────────────────────────────────────
// Only headers from common/ that are (a) included by >8% of TUs
// (directly or transitively) and (b) rarely change.
#include "common/api.h"
#include "common/assert.h"
#include "common/cast.h"
#include "common/copy_constructors.h"
#include "common/exception/binder.h"
#include "common/exception/runtime.h"
#include "common/string_format.h"
#include "common/types/type_aliases.h"
#include "common/types/types.h"
