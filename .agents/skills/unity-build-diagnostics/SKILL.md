---
name: unity-build-diagnostics
description: Identifies, refactors, and resolves C++ Unity Build compilation and linker errors including ODR violations, regex pitfall mistakes, and dead-code elimination breakage.
---

# Unity Build Diagnostics and Refactoring

When enabling `CMAKE_UNITY_BUILD=ON`, C++ projects that compile fine as individual object files often encounter a wave of compilation and linker errors. This skill provides guidelines for diagnosing and resolving the four primary failure modes introduced by Unity Builds.

## 1. One Definition Rule (ODR) Violations

### Problem
Global `static` functions (e.g., `bindFunc`, `execFunc`) that share names across different `.cpp` files will collide when CMake merges those files into a single `.cxx` translation unit, causing "redefinition" errors.

### Solution
Prefix global static functions with their filename to isolate them.
- `static void execFunc(...)` -> `static void listselect_execFunc(...)`

**Warning**: Do not rely on anonymous namespaces (`namespace { }`) alone, as they do not always prevent collisions within the same Unity translation unit unless properly structured by the build system.

## 2. Regex Over-Correction Pitfalls

### Problem
Naive search-and-replace scripts used to fix ODR violations (e.g., blindly replacing `execFunc` with `PREFIX_execFunc`) often accidentally rename variables that were *not* global static functions, leading to "unused variable" or "not declared in this scope" compiler errors.

### Watch Out For:
1. **Local Variables**: `auto execFunc = ...;` inside a class method.
2. **Struct Member Assignments**: `exportFunc->bind = bindFunc;`
3. **Class Member Functions**: `std::unique_ptr<BindData> MyClass::bindFunc(...)`

### Solution
Always manually review compiler output following bulk renames. If you see an `unused variable` warning right next to a `not declared in this scope` error, you likely replaced the usage of a local variable but not its definition (or vice versa). Revert the local variable back to its original name.

## 3. Missing Header Guards

### Problem
If a header is missing `#pragma once` (or `#ifndef` guards), and multiple `.cpp` files in the same Unity chunk `#include` it, the compiler will see the class definition multiple times in the same translation unit, throwing a "redefinition of class X" error.

### Solution
Immediately append `#pragma once` to the top of the offending header.

## 4. Dead-Code Elimination Breakage (Linker Errors)

### Problem
In standard builds, if a feature (e.g., Parquet support) is disabled via CMake, but its `.cpp` source files are still accidentally globbed, it often compiles fine and links fine because the resulting `.o` files are never referenced, allowing the linker (`ld`) to silently drop them.

In Unity builds, however, that disabled `.cpp` file is combined into a `.cxx` file alongside *active* code. Because the active code in the `.cxx` file is referenced, the linker must load the entire `.cxx.o` object file. This forces the linker to resolve symbols for the disabled code as well, leading to massive chains of "undefined reference" errors (e.g., missing Thrift, Snappy, or Parquet symbols).

### Solution
Audit `CMakeLists.txt` for `GLOB_RECURSE` commands. Ensure that disabled modules are explicitly removed from the globbed lists rather than relying on linker garbage collection.

```cmake
# Example Fix:
if(GORGONZOLA_LITE)
    file(GLOB_RECURSE PARQUET_SOURCES
        "processor/operator/persistent/reader/parquet/*.cpp"
        "function/export/export_parquet_function.cpp"
    )
    foreach(PARQUET_FILE IN LISTS PARQUET_SOURCES)
        list(REMOVE_ITEM EXECUTION_SOURCES ${PARQUET_FILE})
    endforeach()
endif()
```
