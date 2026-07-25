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

## 5. Template Functions with Implicit Literals (THE SILENT KILLER)

The regex-based Python scripts from Section 2 only catch lines where `int128_t` or `uint128_t` appear **literally** (e.g., `int128_t result = 0;`). They **completely miss** template functions where the type is a parameter `T` that gets instantiated with a 128-bit type at compile time.

These are the hardest to find because the type name never appears on the offending line:
- **`result = 0;`** inside a `template<typename T>` function → **Fix**: `result = T(0);`
- **`result = 1;`** inside a `template<typename T>` function → **Fix**: `result = T(1);`
- **`T step = 1;`** → **Fix**: `T step = T(1);`
- **`T start = 0;`** → **Fix**: `T start = T(0);`
- **`if (step == 0)`** → **Fix**: `if (step == T(0))`

**Discovery Strategy**: You cannot grep for these. You must compile and read the full template instantiation chain in the error output. The compiler will show you lines like:
```
required from '... [with T = gorgonzola::common::uint128_t]'
```
Follow that chain to the actual offending line.

**Known files with this pattern**:
- `src/function/list/list_agg_function.cpp` — `ListSum::operation` and `ListProduct::operation`
- `src/function/list/list_range_function.cpp` — `Range::operation` (multiple overloads)

## 6. Aggregate Functions (SUM / AVG)

Aggregate functions accumulate smaller input types (e.g., `int64_t`) into larger result types (e.g., `int128_t`). With explicit constructors, the compiler refuses to implicitly promote the input value.

- **Pattern**: `Add::operation(state->sum, val, state->sum);` where `val` is `INPUT_TYPE` but `sum` is `RESULT_TYPE`
- **Fix**: Cast the input value before passing it: `RESULT_TYPE castedVal = static_cast<RESULT_TYPE>(val);` then use `castedVal` in both the initial assignment and the `Add::operation` call.

**Known files**:
- `src/include/function/aggregate/sum.h` — `SumFunction::updateSingleValue`
- `src/include/function/aggregate/avg.h` — `AvgFunction::updateSingleValue`

### AVG-Specific: Count is Not a 128-bit Type

In `AvgState::finalize()`, the row count (`uint64_t count`) was being passed through `ResultType::cast<long double>(count)`, which expects a 128-bit argument. Since `count` is just a plain `uint64_t`, use `static_cast<long double>(count)` instead.

## 7. Numeric Limits and Bounds Checking

`NumericLimits<T>::isInBounds(V val)` compares `minimum()` (type `T`) against `val` (type `V`). When `T` is `int64_t` and `V` is `int128_t` (or vice versa), the comparison fails because neither side can implicitly convert to the other.

- **Fix**: Cast the bounds to the value's type: `static_cast<V>(minimum()) <= val && val <= static_cast<V>(maximum())`
- **File**: `src/include/function/cast/functions/numeric_limits.h`

## 8. Arithmetic with Float Literals

128-bit types cannot participate in implicit floating-point arithmetic. The pattern `(end - start) * 1.0 / step` fails because `* 1.0` tries to implicitly convert a 128-bit result to `double`.

- **Fix**: Use explicit casts: `static_cast<double>(end - start) / static_cast<double>(step)`
- **File**: `src/function/list/list_range_function.cpp`

Similarly, `res.result += 1` inside template cast functions fails when `T` is `int128_t`:
- **Fix**: `res.result += T(1);`
- **File**: `src/include/function/cast/functions/cast_string_non_nested_functions.h`

## 9. Function Signature Mismatches (Collateral Discovery)

During full rebuilds triggered by header changes, previously-hidden signature mismatches may surface. For example, the `.cpp` definition using `const std::string name` (by value) while the `.h` declaration uses `const std::string& name` (by reference).

- **Fix**: Make the `.cpp` definition match the `.h` declaration exactly.
- **Known file**: `src/function/built_in_function_utils.cpp` — `getFunctionMatchFailureMsg`

## 10. Automation Limitations & Strategy

### What regex scripts CAN fix
- Lines with explicit type names: `int128_t x = 0;` → `int128_t x(0);`
- Run across `src/` AND `modules/test/` directories (not just tests!)

### What regex scripts CANNOT fix
- Template code using `T` as the type parameter
- Comparison operators between mixed types (`int64_t <= int128_t`)
- Arithmetic with float literals (`* 1.0`)
- Aggregate function type promotion patterns
- Function signature mismatches

### Recommended workflow
1. Run the regex script on BOTH `src/` and `modules/test/` first
2. Build and collect ALL errors (don't stop at the first one)
3. Fix template/operator errors manually in batches grouped by pattern
4. Rebuild only the affected translation unit when possible: `make -j$(nproc) <target>` not the whole project
