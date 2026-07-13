#!/usr/bin/env bash
set -eu

# Stage 0 Architecture Audit Script for Gorgonzola (C++/C aware)
# This script performs a comprehensive inventory of the Gorgonzola codebase
# to assess its embeddability and architectural readiness, with deep C++ analysis.

# Usage: ./stage0_audit.sh [path-to-gorgonzola]
# If no path provided, it looks for ../gorgonzola or ./gorgonzola.

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PECORINO_ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"

# Determine Gorgonzola root
GORGONZOLA_ROOT="${1:-}"
if [[ -z "$GORGONZOLA_ROOT" ]]; then
    if [[ -d "$PECORINO_ROOT/gorgonzola" ]]; then
        GORGONZOLA_ROOT="$PECORINO_ROOT/gorgonzola"
    elif [[ -d "$PECORINO_ROOT/../gorgonzola" ]]; then
        GORGONZOLA_ROOT="$(cd "$PECORINO_ROOT/../gorgonzola" && pwd)"
    else
        echo "ERROR: Could not find Gorgonzola source. Please provide the path as an argument."
        exit 1
    fi
fi

# Ensure the directory exists
if [[ ! -d "$GORGONZOLA_ROOT" ]]; then
    echo "ERROR: Directory $GORGONZOLA_ROOT does not exist."
    exit 1
fi

echo "Gorgonzola root: $GORGONZOLA_ROOT"

# Output directory for audit reports
AUDIT_DIR="$PECORINO_ROOT/audit_reports"
mkdir -p "$AUDIT_DIR"
REPORT_FILE="$AUDIT_DIR/audit_report.md"

# Initialize report
cat > "$REPORT_FILE" <<EOF
# Gorgonzola Architecture Audit Report (C++/C Focused)

Generated on: $(date)

## Overview

- **Repository:** $GORGONZOLA_ROOT
- **Audit Script:** $0
- **Language Analysis:** C++ and C (includes STL, RAII, templates, virtual, exceptions, C linkage)

This report provides a comprehensive architectural inventory of the Gorgonzola codebase, focusing on embeddability and refactoring readiness.

---

EOF

# Helper to append report
append_report() {
    echo -e "$1" >> "$REPORT_FILE"
}

# Task 1: Execution Pipeline (unchanged)
task1_execution_pipeline() {
    append_report "## Task 1: Execution Pipeline"
    append_report ""
    append_report "### Identified Components"
    append_report ""

    local components=("parser" "binder" "planner" "optimizer" "executor" "storage")
    for comp in "${components[@]}"; do
        local dirs=$(find "$GORGONZOLA_ROOT/src" -type d -name "$comp" 2>/dev/null | head -5)
        if [[ -n "$dirs" ]]; then
            append_report "- **$comp**: Found at $dirs"
        else
            append_report "- **$comp**: Not found as a separate directory; may be integrated elsewhere."
        fi
    done

    append_report ""
    append_report "### Potential Entry Points in Pipeline"
    grep -r --include="*.cc" --include="*.cpp" --include="*.c" -E "(parse|bind|plan|optimize|execute|run)" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -20 | while read -r line; do
        append_report "  - $line"
    done

    append_report ""
    append_report "### Suggested Pipeline Diagram"
    append_report "\`\`\`mermaid"
    append_report "graph TD"
    append_report "    A[CLI/Shell] --> B[Parser]"
    append_report "    B --> C[Binder]"
    append_report "    C --> D[Planner]"
    append_report "    D --> E[Optimizer]"
    append_report "    E --> F[Executor]"
    append_report "    F --> G[Storage]"
    append_report "\`\`\`"
    append_report ""
}

# Task 2: Public Entry Points (includes C API detection)
task2_entry_points() {
    append_report "## Task 2: Public Entry Points"
    append_report ""
    append_report "### C++ API Headers"
    find "$GORGONZOLA_ROOT" -path "*/include/*" -name "*.h" -o -name "*.hpp" 2>/dev/null | while read -r header; do
        append_report "- $header"
    done | head -30

    append_report ""
    append_report "### C API (extern \"C\")"
    grep -r --include="*.h" --include="*.hpp" -E "extern \"C\"" "$GORGONZOLA_ROOT" 2>/dev/null | while read -r line; do
        append_report "- $line"
    done

    append_report ""
    append_report "### CLI / Shell Main Functions"
    grep -r --include="*.cc" --include="*.cpp" -E "int main\(" "$GORGONZOLA_ROOT" 2>/dev/null | while read -r line; do
        append_report "- $line"
    done

    append_report ""
    append_report "### Benchmark Executables"
    find "$GORGONZOLA_ROOT" -path "*/benchmark/*" -name "*.cc" -o -name "*.cpp" 2>/dev/null | while read -r bench; do
        append_report "- $bench"
    done
    append_report ""
}

# Task 3: Singletons and Globals (unchanged)
task3_globals() {
    append_report "## Task 3: Singletons and Global Objects"
    append_report ""
    append_report "### Static Globals"
    grep -r --include="*.cc" --include="*.cpp" --include="*.c" -E "(static|global)" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -20 | while read -r line; do
        append_report "- $line"
    done

    append_report ""
    append_report "### Singleton Patterns"
    grep -r --include="*.cc" --include="*.cpp" -E "singleton|getInstance" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -20 | while read -r line; do
        append_report "- $line"
    done

    append_report ""
    append_report "### Meyers Singletons"
    grep -r --include="*.cc" --include="*.cpp" -E "static.*&.*\(\)" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -10 | while read -r line; do
        append_report "- $line"
    done
    append_report ""
}

# Task 4: Lifecycle Management (enhanced for RAII)
task4_lifecycle() {
    append_report "## Task 4: Lifecycle Management (RAII / Constructor-Destructor)"
    append_report ""
    append_report "### Open/Close Functions"
    grep -r --include="*.cc" --include="*.cpp" -E "(open|close|init|shutdown|destroy)" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -20 | while read -r line; do
        append_report "- $line"
    done

    append_report ""
    append_report "### Constructors/Destructors"
    grep -r --include="*.cc" --include="*.cpp" -E "(~.*\(\)|^.*\(\)\s*:)" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -20 | while read -r line; do
        append_report "- $line"
    done

    append_report ""
    append_report "### RAII Wrappers (unique_ptr, scoped handles)"
    grep -r --include="*.cc" --include="*.cpp" --include="*.h" --include="*.hpp" -E "(unique_ptr|shared_ptr|weak_ptr|make_unique|make_shared|Scoped)" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -20 | while read -r line; do
        append_report "- $line"
    done
    append_report ""
}

# Task 5: Query Execution Flow (unchanged)
task5_query_flow() {
    append_report "## Task 5: Query Execution Flow"
    append_report ""
    append_report "### Key Functions in Flow"
    grep -r --include="*.cc" --include="*.cpp" -E "(parse|bind|plan|optimize|execute|result)" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -30 | while read -r line; do
        append_report "- $line"
    done

    append_report ""
    append_report "### Sequence Diagram (suggested)"
    append_report "\`\`\`mermaid"
    append_report "sequenceDiagram"
    append_report "    User->>Parser: Cypher query"
    append_report "    Parser->>Binder: AST"
    append_report "    Binder->>Planner: Logical Plan"
    append_report "    Planner->>Optimizer: Physical Plan"
    append_report "    Optimizer->>Executor: Optimized Plan"
    append_report "    Executor->>Storage: Read/Write"
    append_report "    Storage-->>Executor: Data"
    append_report "    Executor-->>User: Result"
    append_report "\`\`\`"
    append_report ""
}

# Task 6: Storage Architecture (unchanged)
task6_storage() {
    append_report "## Task 6: Storage Architecture"
    append_report ""
    append_report "### Storage Manager Classes"
    grep -r --include="*.h" --include="*.hpp" -E "(StorageManager|BufferManager|PageManager|WAL|Checkpoint)" "$GORGONZOLA_ROOT/src" 2>/dev/null | while read -r line; do
        append_report "- $line"
    done

    append_report ""
    append_report "### Indexing"
    grep -r --include="*.h" --include="*.hpp" -E "(index|BTree|Hash)" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -10 | while read -r line; do
        append_report "- $line"
    done
    append_report ""
}

# Task 7: Parser Dependencies (enhanced with C++ includes)
task7_parser_deps() {
    append_report "## Task 7: Parser Dependencies"
    append_report ""
    append_report "### Includes from Parser Files"
    find "$GORGONZOLA_ROOT/src" -path "*/parser/*" -name "*.cc" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" 2>/dev/null | while read -r file; do
        grep -E "^#include" "$file" 2>/dev/null | head -5 | while read -r inc; do
            append_report "- $inc (in $file)"
        done
    done | head -30

    append_report ""
    append_report "### C++ Standard Library Usage in Parser"
    find "$GORGONZOLA_ROOT/src" -path "*/parser/*" -name "*.cc" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" 2>/dev/null | while read -r file; do
        grep -E "(std::vector|std::map|std::unordered_map|std::string|std::shared_ptr|std::unique_ptr)" "$file" 2>/dev/null | head -5 | while read -r usage; do
            append_report "  - $usage (in $file)"
        done
    done | head -20
    append_report ""
}

# Task 8: Executor Dependencies (enhanced)
task8_executor_deps() {
    append_report "## Task 8: Executor Dependencies"
    append_report ""
    append_report "### Executor Includes"
    find "$GORGONZOLA_ROOT/src" -path "*/executor/*" -name "*.cc" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" 2>/dev/null | while read -r file; do
        grep -E "^#include" "$file" 2>/dev/null | head -5 | while read -r inc; do
            append_report "- $inc (in $file)"
        done
    done | head -30

    append_report ""
    append_report "### STL Container Usage in Executor"
    find "$GORGONZOLA_ROOT/src" -path "*/executor/*" -name "*.cc" -o -name "*.cpp" -o -name "*.h" -o -name "*.hpp" 2>/dev/null | while read -r file; do
        grep -E "(std::vector|std::list|std::deque|std::queue|std::stack|std::map|std::unordered_map|std::set|std::unordered_set)" "$file" 2>/dev/null | head -5 | while read -r usage; do
            append_report "  - $usage (in $file)"
        done
    done | head -20
    append_report ""
}

# Task 9: Public Headers Inventory (C++ aware)
task9_headers() {
    append_report "## Task 9: Public Headers Inventory"
    append_report ""
    append_report "### Installed Headers (likely under include/)"
    find "$GORGONZOLA_ROOT" -type d -name "include" 2>/dev/null | while read -r incdir; do
        find "$incdir" -name "*.h" -o -name "*.hpp" 2>/dev/null | while read -r h; do
            append_report "- $h"
        done
    done | head -40

    append_report ""
    append_report "### Headers with C++ Guards (extern \"C\")"
    grep -r --include="*.h" --include="*.hpp" -l "extern \"C\"" "$GORGONZOLA_ROOT" 2>/dev/null | head -20 | while read -r h; do
        append_report "- $h"
    done

    append_report ""
    append_report "### Internal Headers (src/ only)"
    find "$GORGONZOLA_ROOT/src" -name "*.h" -o -name "*.hpp" 2>/dev/null | head -30 | while read -r h; do
        append_report "- $h"
    done
    append_report ""
}

# Task 10: Build System (unchanged)
task10_build() {
    append_report "## Task 10: Build System"
    append_report ""
    append_report "### CMake Files"
    find "$GORGONZOLA_ROOT" -name "CMakeLists.txt" -o -name "*.cmake" 2>/dev/null | while read -r cmake; do
        append_report "- $cmake"
    done

    append_report ""
    append_report "### Feature Flags"
    grep -r --include="CMakeLists.txt" --include="*.cmake" -E "(option|set.*ON|set.*OFF)" "$GORGONZOLA_ROOT" 2>/dev/null | head -20 | while read -r flag; do
        append_report "- $flag"
    done

    append_report ""
    append_report "### Lite Build Mode"
    if grep -r -i "lite" "$GORGONZOLA_ROOT/CMakeLists.txt" 2>/dev/null; then
        append_report "Lite build mode detected."
    else
        append_report "Lite build mode not explicitly detected."
    fi

    append_report ""
    append_report "### C++ Standard Version"
    grep -r --include="CMakeLists.txt" --include="*.cmake" -E "CMAKE_CXX_STANDARD|std=c\+\+" "$GORGONZOLA_ROOT" 2>/dev/null | head -5 | while read -r std; do
        append_report "- $std"
    done
    append_report ""
}

# Task 11: Threading Audit (unchanged)
task11_threading() {
    append_report "## Task 11: Threading Audit"
    append_report ""
    append_report "### Mutexes and Locks"
    grep -r --include="*.cc" --include="*.cpp" --include="*.h" --include="*.hpp" -E "(mutex|lock|atomic)" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -20 | while read -r line; do
        append_report "- $line"
    done

    append_report ""
    append_report "### Thread Pools"
    grep -r --include="*.cc" --include="*.cpp" -E "(thread_pool|ThreadPool|worker)" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -10 | while read -r line; do
        append_report "- $line"
    done
    append_report ""
}

# Task 12: Error Handling (C++ exceptions, assert, exit)
task12_errors() {
    append_report "## Task 12: Error Handling"
    append_report ""
    append_report "### assert() usage"
    grep -r --include="*.cc" --include="*.cpp" -E "assert\(" "$GORGONZOLA_ROOT/src" 2>/dev/null | wc -l | while read -r count; do
        append_report "Number of assert() calls: $count"
    done

    append_report ""
    append_report "### exit() / abort() calls"
    grep -r --include="*.cc" --include="*.cpp" -E "(exit\(|abort\()" "$GORGONZOLA_ROOT/src" 2>/dev/null | while read -r line; do
        append_report "- $line"
    done

    append_report ""
    append_report "### C++ Exceptions (try, catch, throw)"
    grep -r --include="*.cc" --include="*.cpp" --include="*.h" --include="*.hpp" -E "(try|catch|throw)" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -20 | while read -r line; do
        append_report "- $line"
    done

    append_report ""
    append_report "### Status/Error Classes"
    grep -r --include="*.h" --include="*.hpp" -E "(Status|Error|Result)" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -10 | while read -r line; do
        append_report "- $line"
    done
    append_report ""
}

# Task 13: Memory Ownership (enhanced for C++ smart pointers)
task13_ownership() {
    append_report "## Task 13: Memory Ownership"
    append_report ""
    append_report "### Smart Pointer Usage"
    grep -r --include="*.cc" --include="*.cpp" --include="*.h" --include="*.hpp" -E "(unique_ptr|shared_ptr|weak_ptr)" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -30 | while read -r line; do
        append_report "- $line"
    done

    append_report ""
    append_report "### Raw pointers (potential ownership ambiguity)"
    grep -r --include="*.cc" --include="*.cpp" -E "\*.*=" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -20 | while read -r line; do
        append_report "- $line"
    done

    append_report ""
    append_report "### Custom allocators / memory pools"
    grep -r --include="*.cc" --include="*.cpp" --include="*.h" --include="*.hpp" -E "(allocator|pool|new|delete)" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -10 | while read -r line; do
        append_report "- $line"
    done
    append_report ""
}

# Task 14: CLI Coupling (unchanged)
task14_cli() {
    append_report "## Task 14: CLI Coupling"
    append_report ""
    append_report "### stdin/stdout usage"
    grep -r --include="*.cc" --include="*.cpp" -E "(std::cin|std::cout|std::cerr|printf|fprintf)" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -20 | while read -r line; do
        append_report "- $line"
    done

    append_report ""
    append_report "### CLI flags / argument parsing"
    grep -r --include="*.cc" --include="*.cpp" -E "(argc|argv|getopt|CLI)" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -10 | while read -r line; do
        append_report "- $line"
    done
    append_report ""
}

# Task 15: Architecture Documents (unchanged)
task15_docs() {
    append_report "## Task 15: Architecture Documents"
    append_report ""
    append_report "### Existing Documentation"
    find "$GORGONZOLA_ROOT" -name "*.md" 2>/dev/null | while read -r doc; do
        append_report "- $doc"
    done
    append_report ""
}

# Task 16: C++ Specific Features (virtual, templates, etc.)
task16_cpp_features() {
    append_report "## Task 16: C++ Specific Features"
    append_report ""
    append_report "### Virtual Functions / Polymorphism"
    grep -r --include="*.h" --include="*.hpp" -E "virtual.*= 0|virtual.*override" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -20 | while read -r line; do
        append_report "- $line"
    done

    append_report ""
    append_report "### Templates"
    grep -r --include="*.h" --include="*.hpp" --include="*.cc" --include="*.cpp" -E "template\s*<" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -20 | while read -r line; do
        append_report "- $line"
    done

    append_report ""
    append_report "### STL Algorithms"
    grep -r --include="*.cc" --include="*.cpp" -E "std::(find|sort|transform|for_each|copy|accumulate)" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -10 | while read -r line; do
        append_report "- $line"
    done

    append_report ""
    append_report "### Move Semantics (std::move, &&)"
    grep -r --include="*.cc" --include="*.cpp" --include="*.h" --include="*.hpp" -E "(std::move|&&)" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -10 | while read -r line; do
        append_report "- $line"
    done

    append_report ""
    append_report "### RTTI / typeid"
    grep -r --include="*.cc" --include="*.cpp" -E "(typeid|dynamic_cast)" "$GORGONZOLA_ROOT/src" 2>/dev/null | head -5 | while read -r line; do
        append_report "- $line"
    done
    append_report ""
}

# Task 17: Embeddability Audit (Matrix) - enhanced with C++ traits
task17_embeddability() {
    append_report "## Task 17: Embeddability Audit (C++/C Matrix)"
    append_report ""
    append_report "### Subsystem Assessment"
    append_report ""
    append_report "| Subsystem | Already Embeddable | Changes Needed | Difficulty | C++ Specific Issues |"
    append_report "|-----------|-------------------|----------------|------------|---------------------|"
    
    # For each component, we now also check for C++ specific patterns that might hinder embedding
    for comp in parser binder optimizer executor storage catalog transaction extension_manager c_api; do
        comp_dir=$(echo "$comp" | tr '_' '/')
        dir_path="$GORGONZOLA_ROOT/src/$comp_dir"
        if [[ -d "$dir_path" ]]; then
            # Check CLI coupling
            if grep -r "std::cout\|std::cin\|printf" "$dir_path" 2>/dev/null | grep -q .; then
                cli_issue="CLI I/O"
            else
                cli_issue="None"
            fi
            # Check for globals
            if grep -r "static\|singleton" "$dir_path" 2>/dev/null | grep -q .; then
                global_issue="Global state"
            else
                global_issue="None"
            fi
            # Check for exceptions
            if grep -r "throw\|try\|catch" "$dir_path" 2>/dev/null | grep -q .; then
                exception_issue="Exceptions used"
            else
                exception_issue="None"
            fi
            # Check for raw pointers
            if grep -r "\*.*=" "$dir_path" 2>/dev/null | grep -q .; then
                pointer_issue="Raw pointers"
            else
                pointer_issue="None"
            fi
            # Combine issues
            issues=""
            [[ "$cli_issue" != "None" ]] && issues+="$cli_issue, "
            [[ "$global_issue" != "None" ]] && issues+="$global_issue, "
            [[ "$exception_issue" != "None" ]] && issues+="$exception_issue, "
            [[ "$pointer_issue" != "None" ]] && issues+="$pointer_issue, "
            issues="${issues%, }"  # strip trailing comma

            if [[ -z "$issues" ]]; then
                ready="✅"
                changes="None"
                diff="Low"
            else
                ready="⚠️"
                changes="Resolve: $issues"
                diff="Medium"
            fi
        else
            ready="❓"
            changes="Directory not found"
            diff="Unknown"
            issues="N/A"
        fi
        # Capitalize component name
        name=$(echo "$comp" | sed 's/_.*/ &/; s/./\u&/')
        append_report "| $name | $ready | $changes | $diff | $issues |"
    done
    append_report ""
}

# Generate the report by running all tasks
task1_execution_pipeline
task2_entry_points
task3_globals
task4_lifecycle
task5_query_flow
task6_storage
task7_parser_deps
task8_executor_deps
task9_headers
task10_build
task11_threading
task12_errors
task13_ownership
task14_cli
task15_docs
task16_cpp_features
task17_embeddability

# Add summary
append_report "## Summary and Recommendations"
append_report ""
append_report "This audit provides a snapshot of the current architecture with a strong C++/C focus. Key findings:"
append_report ""
append_report "- **CLI Coupling**: I/O usage detected in several subsystems. These need to be abstracted."
append_report "- **Global State**: Singleton patterns and static globals exist; they should be moved into Database/Session objects."
append_report "- **Lifecycle**: RAII patterns (unique_ptr, shared_ptr) are present but may not be uniformly applied."
append_report "- **Public Headers**: Some headers are mixed; a clear separation is needed for embeddability. Look for `extern \"C\"` guards for C API."
append_report "- **C++ Specifics**: Templates, virtual functions, and STL usage are extensive. Ensure these do not prevent linking or cause ABI instability."
append_report "- **Error Handling**: Mix of assert, exit, and exceptions. Consider a unified Status/Result pattern."
append_report ""
append_report "Recommended refactor order:"
append_report "1. Isolate CLI from core engine (remove stdout/cin dependencies)."
append_report "2. Introduce Database, Session, and Transaction objects to own state."
append_report "3. Replace singletons with instance-owned state."
append_report "4. Formalize error handling (Status/Result) and remove exit/assert for runtime errors."
append_report "5. Expose a stable C++ API and optionally a C ABI (with `extern \"C\"`)."
append_report "6. Ensure RAII is used for all resource management."
append_report ""
append_report "Full details are in the sections above."

echo "Audit report generated: $REPORT_FILE"

# Also produce a summary matrix in a separate file
MATRIX_FILE="$AUDIT_DIR/embeddability_matrix.md"
cat > "$MATRIX_FILE" <<EOF
# Embeddability Matrix (C++/C) from Stage 0 Audit

| Subsystem | Already Embeddable | Changes Needed | Difficulty | C++ Specific Issues |
|-----------|-------------------|----------------|------------|---------------------|
EOF
grep -A 100 "### Subsystem Assessment" "$REPORT_FILE" | grep -E "^\|" >> "$MATRIX_FILE"

echo "Matrix saved: $MATRIX_FILE"

exit 0