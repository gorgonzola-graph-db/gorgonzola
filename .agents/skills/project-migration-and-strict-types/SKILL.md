---
name: project-migration-and-strict-types
description: Handles complex C++ project renames, namespace migrations, and strict explicit type constructor conversions.
---

# C++ Project Rename and Strict Type Migration Skill

This skill guides the agent in handling large-scale codebase renaming (e.g., from a legacy name to a new fork name) and resolving cascading compilation failures caused by strict typing (such as marking constructors as `explicit`).

## 1. Project Renaming (The "Fork" Scenario)

When a project is renamed, renaming the main source files is often not enough. Submodules and test suites often retain old references that break compilation and linking. You must actively fix:
- **Namespaces**: Replace all `namespace old_name` with `namespace new_name` across tests, headers, and implementation files.
- **Macros and Constants**: Be mindful of case sensitivity. Replace `OLD_NAME_CONSTANT` with `NEW_NAME_CONSTANT` (e.g., `KUZU_PAGE_SIZE` to `GORGONZOLA_PAGE_SIZE`), and TitleCase variants.
- **Build Systems**: Update `CMakeLists.txt` (or equivalent) to link against the newly named targets/libraries (e.g., `-lold_name` to `-lnew_name` or `target_link_libraries(... new_name)`).

**Automation Strategy**: Use Python scripts with `write_to_file` and `run_command` tools to recursively search and replace terms across directories to avoid manual, tedious fixes. Target `.cpp`, `.h`, `.test`, and `CMakeLists.txt` files specifically.

## 2. Strict Type Conversions (`explicit` Constructors)

When a codebase enforces strict typing (e.g., changing constructors for custom primitive types like `int128_t` or `uint128_t` to be `explicit`), implicit conversions from standard integers will break. This often surfaces in test suites that were written under looser constraints.

Watch out for these common failure patterns:
- **Variable Initialization**: `uint128_t value = 42;` -> **Fix**: `uint128_t value(42);`
- **Return Statements**: Returning `0` from a function returning `uint128_t` -> **Fix**: `return uint128_t(0);`
- **Comparisons (e.g. GTest)**: `if (value > 0)` or `EXPECT_EQ(15, value)` -> **Fix**: `if (value > uint128_t(0))` and `EXPECT_EQ(uint128_t(15), value)`
- **Compound Assignment**: `value += 7;` -> **Fix**: `value += uint128_t(7);`

**Automation Strategy**: If a test suite has dozens of implicit casts, write a regex-based Python script to automate wrapping integers in explicit type casts rather than fixing them one-by-one.

## 3. Resolving "Most Vexing Parse" Warnings

When explicitly calling constructors (especially around C-style casting), avoid triggering C++'s "most vexing parse".
- **Bad**: `int128_t expected(double(val));` (The compiler thinks this is a function declaration)
- **Good**: `int128_t expected{double(val)};` (Use brace initialization to guarantee variable instantiation)

## 4. Non-Trivial Types in Unions and Storage Engine Impact

When changing primitive-like structs (e.g., `int128_t`) to use `explicit` constructors, C++ may reclassify them as "non-trivial" types.
If these types are used inside `union`s (like `StorageValue` in a database storage engine), the compiler will implicitly delete the union's default constructor.

- **Fix**: Explicitly define the union's default constructor to initialize one of its members (e.g., `StorageValue() : signedInt(0) {}`).
- **Graph Impact**: This initialization only zero-pads the memory to satisfy C++ compilation requirements. It has no negative impact on actual graph data or schemas, as the database engine will overwrite the union with valid storage/compression metadata before writing anything to disk. The runtime graph storage remains completely intact.
