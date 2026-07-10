# Gorgonzola Architecture Modernization & Build Optimization

Validated implementation plan based on thorough codebase research. Your original plan is solid and well-reasoned — this document refines it with codebase-specific findings, corrections, and concrete execution steps.

## Codebase Facts (Research Findings)

| Metric | Value |
|---|---|
| CPU | i5-6500T, 4 cores @ 2.50GHz |
| Source files | 738 `.cpp`, 800 `.h` |
| C++ standard | C++20 |
| Build system | Already using **Ninja** ✅ |
| ccache | Installed (v4.11.2), detected by CMake via `RULE_LAUNCH_COMPILE` ✅ |
| ccache hit rate | 47.3% (needs tuning) |
| ccache config | **Default** (no custom config — `compiler_check=mtime`, no `base_dir`, no `sloppiness`) |
| Grammar | 915-line ANTLR4 Cypher grammar → 19,374-line generated `cypher_parser.cpp` |
| Parser output | `std::vector<std::shared_ptr<Statement>>` — flows to **Binder**, not Planner directly |
| Heavy headers | `types.h` (679 lines, included by **151** files) / `value.h` (899 lines, included by **40** files) |
| Existing C API | [gorgonzola.h](file:///media/lechibang/work/projects/gorgonzola/src/include/c_api/gorgonzola.h) — 1,599-line flat C header (good precedent) |

---

## User Review Required

> [!IMPORTANT]
> **Phase 1 is already partially done.** Ninja and ccache are installed and active. The build is already generating Ninja files. The main remaining Phase 1 work is **ccache tuning** — your hit rate is 47% when it should be 90%+.

> [!WARNING]
> **Your plan says the parser feeds the Planner — it actually feeds the Binder.** The pipeline is: `Parser → Binder → Planner → Optimizer → Processor`. The Rust parser ABI (Phase 3-4) needs to produce `parser::Statement` objects that the **Binder** consumes, not the Planner. This changes the surface area of the C ABI and the shim layer. The translation layer maps C AST → `parser::Statement` subclasses (e.g., `RegularQuery`, `Copy`, DDL statements), not Planner types.

> [!WARNING]
> **LALRPOP won't work for Cypher.** Your plan suggests LALRPOP, but Cypher has significant syntactic ambiguity (e.g., `MATCH` vs `CREATE`, expression contexts). The existing grammar is ANTLR4 (LL parser), and LALRPOP is LALR(1). A more viable Rust path would be `tree-sitter` (GLR) or a hand-tuned recursive-descent parser using `chumsky`, or — most practically — keeping ANTLR4 and using `antlr4-rust` bindings. The 915-line grammar is the real asset here.

---

## Open Questions

> [!IMPORTANT]
> 1. **Phase 1 scope**: Since Ninja + ccache are already working, do you want me to **just apply ccache tuning** (15 min task), or do you also want me to do a `cmake -B build -G Ninja` clean reconfigure?
>
> 2. **Phase 2 priority**: The `types.h` header (151 includers, 99 of which are other headers) is the real kill-chain. `value.h` is bad but only 40 direct includers. Should I focus exclusively on `types.h` first, or do both in parallel?
>
> 3. **Phase 3-4 timeline**: The Rust parser port is a multi-month effort. Do you want Phases 3-4 fully planned now, or should I deliver Phases 1-2 first and revisit 3-4 after measuring the build-time improvement?

---

## Proposed Changes

### [DONE] Phase 1: ccache Tuning (Immediate — no code changes)

The build is already using Ninja + ccache, but **ccache is misconfigured**, explaining the 47% hit rate.

#### [NEW] [ccache.conf](file:///home/lechibang/.config/ccache/ccache.conf)

Create `~/.config/ccache/ccache.conf` with:

```ini
# Gorgonzola ccache configuration
base_dir = /media/lechibang/work/projects/gorgonzola
compiler_check = none
sloppiness = include_file_mtime,include_file_ctime,time_macros,pch_defines
max_size = 10G
```

**Rationale:**
- `compiler_check = none` — Avoids re-hashing the compiler binary on every call. Safe when you're not switching compiler versions mid-session.
- `base_dir` — Critical for cache portability. Without it, absolute paths in debug info cause cache misses when the source tree moves.
- `sloppiness = include_file_mtime,...` — Prevents cache misses from filesystem timestamp jitter (common with git operations).
- `max_size = 10G` — The current 5G limit with 0% usage suggests aggressive eviction. 10G gives breathing room for a 2000+ TU project.

**After applying:** run `ccache -z` (zero stats) then do a full rebuild to prime the cache. Subsequent incremental builds should show 95%+ hit rate.

---

### [PARTIAL] Phase 2: Header Decoupling (Short-Term)

#### Step 2a: Audit & Triage

Before touching any headers, generate a dependency graph to confirm the blast radius:

```bash
# Generate include-what-you-use report for types.h specifically
# (NOT a blind IWYU run — targeted only)
ninja -C build/release -t deps | grep 'types.h' | sort -u | wc -l
```

#### [DONE] Step 2b: Forward-Declare in `types.h`

The [types.h](file:///media/lechibang/work/projects/gorgonzola/src/include/common/types/types.h) header currently defines everything inline — type aliases, `internalID_t`, `LogicalType` (a 120-line class), `ExtraTypeInfo`, `StructField`, and all the type helper structs. Key splits:

##### [NEW] `src/include/common/types/type_aliases.h`
- Extract all `using` type aliases (lines 30-89) and constants into a lightweight header that most files can include instead of the full `types.h`.

##### [MODIFY] [types.h](file:///media/lechibang/work/projects/gorgonzola/src/include/common/types/types.h)
- Replace inline type aliases with `#include "common/types/type_aliases.h"`
- Move `LogicalTypeUtils`, `PhysicalTypeUtils`, `DecimalType`, `ListType`, `ArrayType`, `StructType`, `MapType`, `UnionType` helper struct **implementations** to a new `.cpp` file (they're currently all declared in the header with implementations in separate `.cpp` files — this is fine, the header just needs forward declarations pruned).

##### [MODIFY] Files that only need `table_id_t`, `offset_t`, `column_id_t`, etc.
- Change `#include "common/types/types.h"` → `#include "common/types/type_aliases.h"` in the ~50-80 files that don't actually use `LogicalType`.

#### Step 2c: Forward-Declare `Value` Class

##### [MODIFY] [value.h](file:///media/lechibang/work/projects/gorgonzola/src/include/common/types/value/value.h)
- Audit the 40 includers. For headers that only take `Value*` or `Value&`, replace with forward declaration.
- Move any inline method implementations larger than 3 lines into `value.cpp`.

#### [SKIPPED - C++ limitations] Step 2d: Targeted ANTLR Header Isolation

The [transformer.h](file:///media/lechibang/work/projects/gorgonzola/src/include/parser/transformer.h) directly `#include "cypher_parser.h"` — the 19,374-line generated file. This is a compilation bomb for anything that transitively includes transformer.h.

##### [MODIFY] [transformer.h](file:///media/lechibang/work/projects/gorgonzola/src/include/parser/transformer.h)
- The `Transformer` class holds a reference to `CypherParser::Ku_StatementsContext&` and all its methods take `CypherParser::*Context&` parameters. These are currently **public** methods but are only called from within `transformer.cpp` and the `transform/` subdirectory.
- **Plan**: Make all `CypherParser`-dependent methods **private**. The public interface only needs `transform()` and `transformStatement()`. Then move the `#include "cypher_parser.h"` from the header into the `.cpp` file using a forward declaration of `CypherParser::Ku_StatementsContext`.
- **Risk**: Medium — needs careful verification that no external code calls the internal transform methods.

---

### Phase 3: C ABI Definition (Medium-Term)

> [!NOTE]
> Your design for `parser_abi.h` is correct in principle. Key corrections based on the actual codebase:

#### [NEW] `src/include/c_api/parser_abi.h`
- Define flat C structs mirroring the `parser::Statement` hierarchy, **not** the Planner types.
- The parser produces 15+ statement types: `RegularQuery`, `Copy`, `CreateNodeTable`, `CreateRelGroup`, `AlterTable`, `Drop`, `Transaction`, `StandaloneCall`, `ExplainStatement`, `AttachDatabase`, `DetachDatabase`, `UseDatabase`, `ExtensionStatement`, `CreateMacro`, `PortDB`.
- The ABI should represent the **parsed AST** — the tree of `ParsedExpression` nodes, pattern elements, reading/updating clauses.
- Use the existing [gorgonzola.h](file:///media/lechibang/work/projects/gorgonzola/src/include/c_api/gorgonzola.h) (1,599 lines) as the style guide for the C ABI.

#### [NEW] `src/parser/rust_ast_translator.cpp`
- Translate `parser_abi.h` C structs → `parser::Statement` subclasses.
- The `static_assert` strategy from your plan is correct and essential.

#### Ownership Model
Your `free_ast` approach is sound. One refinement: the existing C API already uses an opaque-handle pattern (e.g., `kuzu_database`, `kuzu_connection`). Follow the same convention — return a `gorgonzola_parsed_result*` handle.

---

### Phase 4: Rust Parser (Long-Term)

#### [NEW] `rust_parser/` Cargo workspace

> [!IMPORTANT]
> **Grammar strategy recommendation:** Instead of LALRPOP, use one of:
> 1. **`antlr4-rust`** — Direct port of the existing 915-line `Cypher.g4`. Lowest risk, highest fidelity.
> 2. **`tree-sitter`** — Write a tree-sitter grammar from the existing ANTLR grammar. Battle-tested GLR parser, great incremental parsing for IDE support.
> 3. **`chumsky`** — Hand-rolled recursive-descent in Rust. Most control, but highest effort.
>
> Option 1 is the safest choice for a production database.

#### Build Integration
- Link `librust_parser.a` via CMake's `ExternalProject_Add` or `FetchContent` + `corrosion` (the standard CMake/Cargo bridge).
- Add a `BUILD_RUST_PARSER` CMake option (default `OFF`) with a `GORGONZOLA_USE_RUST_PARSER` compile definition.
- Keep old C++ parser behind `#ifndef GORGONZOLA_USE_RUST_PARSER`.

---

## Verification Plan

### Phase 1 Verification
```bash
# After ccache tuning:
ccache -z                          # Reset stats
cmake --build build/release        # Full rebuild to prime cache
touch src/main/client_context.cpp  # Dirty one file
time cmake --build build/release   # Should be <10s
ccache -s                          # Verify >90% hit rate
```

### Phase 2 Verification
```bash
# After header refactoring:
cmake --build build/release        # Full clean rebuild — must compile with 0 errors
touch src/include/common/types/types.h
time cmake --build build/release   # Measure how many TUs recompile (target: <50% of current)
```

### Phase 3-4 Verification
- C++ test suite must pass 100% with both old parser and new Rust parser
- Valgrind memcheck on FFI boundary
- `static_assert` coverage for all ABI enum mappings
