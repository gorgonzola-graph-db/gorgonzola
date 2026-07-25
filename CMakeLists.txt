cmake_minimum_required(VERSION 3.31)

project(Gorgonzola VERSION 0.11.2.2 LANGUAGES CXX C)

# ── Gorgonzola Lite ─────────────────────────────────────────────────
# A lightweight build for small-to-medium codebases (< 1M LoC).
# Preserves multi-threading and concurrency but caps scalability.
# Extensions are disabled by default (infrastructure remains intact).
# Parquet/Arrow is a compile-time toggle (off by default).
#
# Usage:
#   cmake -B build-lite -DGORGONZOLA_LITE=ON -DCMAKE_BUILD_TYPE=Release
#   cmake --build build-lite --target gorgonzola -j$(nproc)
# ────────────────────────────────────────────────────────────────────
option(GORGONZOLA_LITE "Build Gorgonzola Lite for small-to-medium codebases" ON)

if(GORGONZOLA_LITE)
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

    # Parallelism: keep threads, cap degree of parallelism
    add_compile_definitions(GORGONZOLA_LITE)
    add_compile_definitions(GORGONZOLA_LITE_MAX_WORKERS=4)

    # Extensions: disabled by default via option() declaration
    # Users can opt-in with -DBUILD_EXTENSIONS="json;fts" etc.
    set(GORGONZOLA_LITE_ENABLE_EXTENSIONS FALSE CACHE BOOL "Enable Extension manager" FORCE)
    if(GORGONZOLA_LITE_ENABLE_EXTENSIONS)
        add_compile_definitions(GORGONZOLA_LITE_ENABLE_EXTENSIONS)
    endif()
    
    option(GORGONZOLA_LITE_ENABLE_GDS "Enable Graph Data Science (GDS)" OFF)
    if(GORGONZOLA_LITE_ENABLE_GDS)
        add_compile_definitions(GORGONZOLA_LITE_ENABLE_GDS)
    endif()
    
    set(GORGONZOLA_LITE_ENABLE_JSON FALSE CACHE BOOL "Enable JSON support" FORCE)
    if(GORGONZOLA_LITE_ENABLE_JSON)
        add_compile_definitions(GORGONZOLA_LITE_ENABLE_JSON)
    endif()

    # I/O: Parquet off, CSV only
    set(GORGONZOLA_ENABLE_PARQUET FALSE CACHE BOOL "" FORCE)

    # Vector similarity: not needed for code graphs, unless vector extension is requested
    if (NOT "vector" IN_LIST BUILD_EXTENSIONS)
        set(GORGONZOLA_ENABLE_SIMSIMD FALSE CACHE BOOL "" FORCE)
    endif()

    # CJK text segmentation: not needed
    set(GORGONZOLA_ENABLE_CPPJIEBA FALSE CACHE BOOL "" FORCE)

    # ALP float compression: not needed for code graphs
    set(GORGONZOLA_ENABLE_ALP FALSE CACHE BOOL "" FORCE)

    # NumPy import: not needed
    set(GORGONZOLA_ENABLE_NPY FALSE CACHE BOOL "" FORCE)

    # Language bindings: C/Rust by default, others opt-in
    # Users can enable with -DBUILD_PYTHON=TRUE etc.

    # Tunable parameters for smaller graphs
    set(GORGONZOLA_VECTOR_CAPACITY_LOG2 10 CACHE STRING "" FORCE)
    set(GORGONZOLA_NODE_GROUP_SIZE_LOG2 15 CACHE STRING "" FORCE)
    set(GORGONZOLA_MAX_SEGMENT_SIZE_LOG2 16 CACHE STRING "" FORCE)

    # Default to Release build
    if(NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Release)
    endif()

    message(STATUS "╔══════════════════════════════════════════════╗")
    message(STATUS "║          GORGONZOLA LITE BUILD               ║")
    message(STATUS "║  Parallelism: min(4, HW threads)             ║")
    message(STATUS "║  Extensions:  disabled (infra intact)        ║")
    message(STATUS "║  Bindings:    C/Rust only                    ║")
    message(STATUS "║  Vector cap:  1024  (log2=10)                ║")
    message(STATUS "║  Node group:  32K   (log2=15)                ║")
    message(STATUS "║  Segment:     64KB  (log2=16)                ║")
    message(STATUS "╚══════════════════════════════════════════════╝")
else()
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
endif()

option(SINGLE_THREADED "Single-threaded mode" FALSE)
if(SINGLE_THREADED)
    set(__SINGLE_THREADED__ TRUE)
    add_compile_definitions(__SINGLE_THREADED__)
    message(STATUS "Single-threaded mode is enabled")
else()
    message(STATUS "Multi-threaded mode is enabled: CMAKE_BUILD_PARALLEL_LEVEL=$ENV{CMAKE_BUILD_PARALLEL_LEVEL}")
    find_package(Threads REQUIRED)
endif()

set(CMAKE_CXX_STANDARD 26)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_C_VISIBILITY_PRESET hidden)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)
set(CMAKE_FIND_PACKAGE_RESOLVE_SYMLINKS TRUE)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_VISIBILITY_INLINES_HIDDEN ON)
# On Linux, symbols in executables are not accessible by loaded shared libraries (e.g. via dlopen(3)). However, we need to export public symbols in executables so that extensions can access public symbols. This enables that behaviour.
set(CMAKE_ENABLE_EXPORTS TRUE)

option(ENABLE_WERROR "Treat all warnings as errors" FALSE)
if(ENABLE_WERROR)
    if (CMAKE_VERSION VERSION_GREATER "3.24.0" OR CMAKE_VERSION VERSION_EQUAL "3.24.0")
        set(CMAKE_COMPILE_WARNING_AS_ERROR TRUE)
    elseif (MSVC)
        add_compile_options(\WX)
    else ()
        add_compile_options(-Werror)
    endif()
endif()

option(ENABLE_CPPCHECK "Enable static analysis with Cppcheck during compilation" OFF)
if(ENABLE_CPPCHECK)
    find_program(CPPCHECK_BIN cppcheck)
    if(CPPCHECK_BIN)
        set(CMAKE_CXX_CPPCHECK
            "${CPPCHECK_BIN}"
            "--enable=warning,style,performance,portability"
            "--inline-suppr"
            "--suppressions-list=${CMAKE_SOURCE_DIR}/.cppcheck-suppressions"
            "--quiet"
        )
        message(STATUS "Cppcheck static analysis enabled for compilation")
    else()
        message(WARNING "ENABLE_CPPCHECK set to ON, but cppcheck binary was not found")
    endif()
endif()

find_program(CPPCHECK_BIN cppcheck)
if(CPPCHECK_BIN)
    add_custom_target(cppcheck
        COMMAND ${CPPCHECK_BIN}
            --enable=warning,style,performance,portability
            --inline-suppr
            --suppressions-list=${CMAKE_SOURCE_DIR}/.cppcheck-suppressions
            --quiet
            --std=c++20
            -I ${CMAKE_SOURCE_DIR}/src/include
            -I ${CMAKE_SOURCE_DIR}/include
            -I ${CMAKE_SOURCE_DIR}/shell/include
            ${CMAKE_SOURCE_DIR}/src
            ${CMAKE_SOURCE_DIR}/shell
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running Cppcheck static analysis"
    )
endif()

# Detect OS and architecture, copied from DuckDB
set(OS_NAME "unknown")
set(OS_ARCH "amd64")

string(REGEX MATCH "(arm64|aarch64)" IS_ARM "${CMAKE_SYSTEM_PROCESSOR}")
if(IS_ARM)
  set(OS_ARCH "arm64")
elseif(FORCE_32_BIT)
  set(OS_ARCH "i386")
endif()

if(APPLE)
  set(OS_NAME "osx")
endif()
if(WIN32)
  set(OS_NAME "windows")
endif()
if(UNIX AND NOT APPLE)
  set(OS_NAME "linux") # sorry BSD
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(STATUS "64-bit architecture detected")
    add_compile_definitions(__64BIT__)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 4)
    message(STATUS "32-bit architecture detected")
    add_compile_definitions(__32BIT__)
    set(__32BIT__ TRUE)
endif()

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

if(DEFINED ENV{PYBIND11_PYTHON_VERSION})
    set(PYBIND11_PYTHON_VERSION $ENV{PYBIND11_PYTHON_VERSION})
endif()

if(DEFINED ENV{PYTHON_EXECUTABLE})
    set(PYTHON_EXECUTABLE $ENV{PYTHON_EXECUTABLE})
endif()

find_program(CCACHE_PROGRAM ccache)
if (CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
else ()
    find_program(CCACHE_PROGRAM sccache)
    if (CCACHE_PROGRAM)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    endif ()
endif ()

set(INSTALL_LIB_DIR
        lib
        CACHE PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR
        bin
        CACHE PATH "Installation directory for executables")
set(INSTALL_INCLUDE_DIR
        include
        CACHE PATH "Installation directory for header files")
set(INSTALL_CMAKE_DIR
        ${DEF_INSTALL_CMAKE_DIR}
        CACHE PATH "Installation directory for CMake files")

option(ENABLE_ADDRESS_SANITIZER "Enable address sanitizer." FALSE)
option(ENABLE_THREAD_SANITIZER "Enable thread sanitizer." FALSE)
option(ENABLE_UBSAN "Enable undefined behavior sanitizer." FALSE)
option(ENABLE_RUNTIME_CHECKS "Enable runtime coherency checks (e.g. asserts)" FALSE)
option(ENABLE_LTO "Enable Link-Time Optimization" FALSE)
option(ENABLE_MALLOC_BUFFER_MANAGER "Enable Buffer manager using malloc. Default option for webassembly" OFF)

option(GORGONZOLA_DEFAULT_REL_STORAGE_DIRECTION "Only store fwd direction in rel tables by default." BOTH)
if(NOT GORGONZOLA_DEFAULT_REL_STORAGE_DIRECTION)
    set(GORGONZOLA_DEFAULT_REL_STORAGE_DIRECTION BOTH)
endif()
set(GORGONZOLA_DEFAULT_REL_STORAGE_DIRECTION ${GORGONZOLA_DEFAULT_REL_STORAGE_DIRECTION}_REL_STORAGE)
option(GORGONZOLA_PAGE_SIZE_LOG2 "Log2 of the page size." 12)
if(NOT GORGONZOLA_PAGE_SIZE_LOG2)
    set(GORGONZOLA_PAGE_SIZE_LOG2 12)
endif()
message(STATUS "GORGONZOLA_PAGE_SIZE_LOG2: ${GORGONZOLA_PAGE_SIZE_LOG2}")
option(GORGONZOLA_VECTOR_CAPACITY_LOG2 "Log2 of the vector capacity." 11)
if(NOT GORGONZOLA_VECTOR_CAPACITY_LOG2)
    set(GORGONZOLA_VECTOR_CAPACITY_LOG2 11)
endif()
message(STATUS "GORGONZOLA_VECTOR_CAPACITY_LOG2: ${GORGONZOLA_VECTOR_CAPACITY_LOG2}")

# 64 * 2048 nodes per group
option(GORGONZOLA_NODE_GROUP_SIZE_LOG2 "Log2 of the vector capacity." 17)
if(NOT GORGONZOLA_NODE_GROUP_SIZE_LOG2)
    set(GORGONZOLA_NODE_GROUP_SIZE_LOG2 17)
endif()
message(STATUS "GORGONZOLA_NODE_GROUP_SIZE_LOG2: ${GORGONZOLA_NODE_GROUP_SIZE_LOG2}")

option(GORGONZOLA_MAX_SEGMENT_SIZE_LOG2 "Log2 of the maximum segment size in bytes." 18)
if(NOT GORGONZOLA_MAX_SEGMENT_SIZE_LOG2)
    set(GORGONZOLA_MAX_SEGMENT_SIZE_LOG2 18)
endif()
message(STATUS "GORGONZOLA_MAX_SEGMENT_SIZE_LOG2: ${GORGONZOLA_MAX_SEGMENT_SIZE_LOG2}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/templates/system_config.h.in ${CMAKE_CURRENT_BINARY_DIR}/src/include/common/system_config.h @ONLY)

include(CheckCXXSymbolExists)
check_cxx_symbol_exists(F_FULLFSYNC "fcntl.h" HAS_FULLFSYNC)
check_cxx_symbol_exists(fdatasync "unistd.h" HAS_FDATASYNC)
if(HAS_FULLFSYNC)
    message(STATUS "✓ F_FULLFSYNC will be used on this platform")
    add_compile_definitions(HAS_FULLFSYNC)
else()
    message(STATUS "✗ F_FULLFSYNC not available")
endif()

if(HAS_FDATASYNC)
    message(STATUS "✓ fdatasync will be used on this platform")
    add_compile_definitions(HAS_FDATASYNC)
else()
    message(STATUS "✗ fdatasync not available, using fsync fallback")
endif()

if(MSVC)
    # Required for M_PI on Windows
    add_compile_definitions(_USE_MATH_DEFINES)
    add_compile_definitions(NOMINMAX)
    add_compile_definitions(SERD_STATIC)
    # This is a workaround for regex oom issue on windows in gtest.
    add_compile_definitions(_REGEX_MAX_STACK_COUNT=0)
    add_compile_definitions(_REGEX_MAX_COMPLEXITY_COUNT=0)
    # Disable constexpr mutex constructor to avoid compatibility issues with
    # older versions of the MSVC runtime library
    # See: https://github.com/microsoft/STL/wiki/Changelog#vs-2022-1710
    add_compile_definitions(_DISABLE_CONSTEXPR_MUTEX_CONSTRUCTOR)
    # TODO (bmwinger): Figure out if this can be set automatically by cmake,
    # or at least better integrated with user-specified options
    # For now, hardcode _AMD64_
    # CMAKE_GENERATOR_PLATFORM can be used for visual studio builds, but not for ninja
    add_compile_definitions(_AMD64_)
    # Non-english windows system may use other encodings other than utf-8 (e.g. Chinese use GBK).
    add_compile_options("/utf-8")
    # Enables support for custom hardware exception handling
    add_compile_options("/EHa")
    # Reduces the size of the static library by roughly 1/2
    add_compile_options("/Zc:inline")
    # Disable type conversion warnings
    add_compile_options(/wd4244 /wd4267)
    # Remove the default to avoid warnings
    STRING(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    STRING(REPLACE "/EHs" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    # Store all libraries and binaries in the same directory so that gorgonzola_shared.dll is found at runtime
    set(LIBRARY_OUTPUT_PATH "${CMAKE_BINARY_DIR}/src")
    set(EXECUTABLE_OUTPUT_PATH "${CMAKE_BINARY_DIR}/src")
    # This is a workaround for regex stackoverflow issue on windows in gtest.
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /STACK:8388608")

    string(REGEX REPLACE "/W[3|4]" "/w" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
    add_compile_options($<$<CONFIG:Release>:/W0>)
else()
    add_compile_options(-Wall -Wextra)
    # Disable warnings for unknown pragmas, which is used by several third-party libraries
    add_compile_options(-Wno-unknown-pragmas)
endif()

if(${BUILD_WASM})
    if(NOT __SINGLE_THREADED__)
        add_compile_options(-pthread)
        add_link_options(-pthread)
        add_link_options(-sPTHREAD_POOL_SIZE=8)
    endif()
    add_compile_options(-s DISABLE_EXCEPTION_CATCHING=0)
    add_link_options(-sSTACK_SIZE=4MB)
    add_link_options(-sASSERTIONS=1)
    add_link_options(-lembind)
    add_link_options(-sWASM_BIGINT)

    if(BUILD_TESTS OR BUILD_EXTENSION_TESTS)
        add_link_options(-sINITIAL_MEMORY=3892MB)
        add_link_options(-sNODERAWFS=1)
    elseif(WASM_NODEFS)
        add_link_options(-sNODERAWFS=1)
        add_link_options(-sALLOW_MEMORY_GROWTH=1)
        add_link_options(-sMODULARIZE=1)
        add_link_options(-sEXPORTED_RUNTIME_METHODS=FS,wasmMemory)
        add_link_options(-sEXPORT_NAME=gorgonzola)
        add_link_options(-sMAXIMUM_MEMORY=4GB)
    else()
        add_link_options(-sSINGLE_FILE=1)
        add_link_options(-sALLOW_MEMORY_GROWTH=1)
        add_link_options(-sMODULARIZE=1)
        add_link_options(-sEXPORTED_RUNTIME_METHODS=FS,wasmMemory)
        add_link_options(-lidbfs.js)
        add_link_options(-lworkerfs.js)
        add_link_options(-sEXPORT_NAME=gorgonzola)
        add_link_options(-sMAXIMUM_MEMORY=4GB)
    endif()
    set(__WASM__ TRUE)
    add_compile_options(-fexceptions)
    add_link_options(-s DISABLE_EXCEPTION_CATCHING=0)
    add_link_options(-fexceptions)
    add_compile_definitions(__WASM__)
    set(ENABLE_MALLOC_BUFFER_MANAGER ON)
endif()

if(${BUILD_SWIFT})
    add_compile_definitions(__SWIFT__)
    set(ENABLE_MALLOC_BUFFER_MANAGER ON)
endif()

if (${ENABLE_MALLOC_BUFFER_MANAGER})
    add_compile_definitions(BM_MALLOC)
endif()

if(ANDROID_ABI)
    message(STATUS "Android ABI detected: ${ANDROID_ABI}")
    add_compile_definitions(__ANDROID__)
    set(__ANDROID__ TRUE)
endif()

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-restrict) # no restrict until https://gcc.gnu.org/bugzilla/show_bug.cgi?id=105651 is fixed
endif()

if(${ENABLE_THREAD_SANITIZER} AND (NOT __SINGLE_THREADED__))
    if(MSVC)
        message(FATAL_ERROR "Thread sanitizer is not supported on MSVC")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=thread -fno-omit-frame-pointer")
    endif()
endif()
if(${ENABLE_ADDRESS_SANITIZER})
    if(MSVC)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /fsanitize=address")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -fno-omit-frame-pointer")
    endif()
endif()
if(${ENABLE_UBSAN})
    if(MSVC)
        message(FATAL_ERROR "Undefined behavior sanitizer is not supported on MSVC")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=undefined -fno-omit-frame-pointer")
    endif()
endif()

if(${ENABLE_RUNTIME_CHECKS})
    add_compile_definitions(GORGONZOLA_RUNTIME_CHECKS)
endif()

if (${ENABLE_DESER_DEBUG})
    add_compile_definitions(GORGONZOLA_DESER_DEBUG)
endif()

if(${ENABLE_LTO})
    set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

option(AUTO_UPDATE_GRAMMAR "Automatically regenerate C++ grammar files on change." FALSE)
option(BUILD_BENCHMARK "Build benchmarks." FALSE)
option(BUILD_EXTENSIONS "Semicolon-separated list of extensions to build." "")
option(BUILD_EXAMPLES "Build examples." FALSE)
option(BUILD_JAVA "Build Java API." FALSE)
option(BUILD_NODEJS "Build NodeJS API." FALSE)
option(BUILD_PYTHON "Build Python API." FALSE)
option(BUILD_SHELL "Build Interactive Shell" TRUE)
option(BUILD_SINGLE_FILE_HEADER "Build single file header. Requires Python >= 3.9." TRUE)
option(BUILD_TESTS "Build C++ tests." FALSE)
option(BUILD_EXTENSION_TESTS "Build C++ extension tests." FALSE)
option(BUILD_GORGONZOLA "Build Gorgonzola." TRUE)
option(ENABLE_BACKTRACES "Enable backtrace printing for exceptions and segfaults" FALSE)
option(USE_STD_FORMAT "Use std::format instead of a custom formatter." FALSE)
option(PREFER_SYSTEM_DEPS "Only download certain deps if not found on the system" TRUE)

option(BUILD_LCOV "Build coverage report." FALSE)
option(ENABLE_DESER_DEBUG "Enable deserialization debug info." FALSE)

option(ENABLE_PCH "Enable precompiled headers (zoned)" OFF)
option(ENABLE_ZONED_PCH "Split PCH per build zone" OFF)
option(GORGONZOLA_PCH_INCLUDE_PROJECT_HEADERS "Include common/ project headers in zone PCHs (off for CI)" OFF)
if(ENABLE_PCH AND GORGONZOLA_PCH_INCLUDE_PROJECT_HEADERS)
    add_compile_definitions(GORGONZOLA_PCH_INCLUDE_PROJECT_HEADERS)
endif()

add_definitions(-DGORGONZOLA_EXTENSION_VERSION="0.11.1")


if (ENABLE_BACKTRACES)
    set(DOWNLOAD_CPPTRACE TRUE)
    if(${PREFER_SYSTEM_DEPS})
        find_package(cpptrace QUIET)
        if(cpptrace_FOUND)
            message(STATUS "Using system cpptrace")
            set(DOWNLOAD_CPPTRACE FALSE)
        endif()
    endif()
    if(${DOWNLOAD_CPPTRACE})
        message(STATUS "Fetching cpptrace from GitHub...")
        include(FetchContent)
        FetchContent_Declare(
            cpptrace
            GIT_REPOSITORY https://github.com/jeremy-rifkin/cpptrace.git
            GIT_TAG        v0.8.3
            GIT_SHALLOW    TRUE
        )
        FetchContent_MakeAvailable(cpptrace)
    endif()
    add_compile_definitions(GORGONZOLA_BACKTRACE)
endif()

if (USE_STD_FORMAT)
    add_compile_definitions(USE_STD_FORMAT)
endif()

if (BUILD_EXTENSION_TESTS)
    add_compile_definitions(__ALLOW_UNSIGNED_EXTENSION__)
else()
    add_compile_definitions(__ALLOW_UNSIGNED_EXTENSION__)
endif()

function(add_gorgonzola_test TEST_NAME)
    set(SRCS ${ARGN})
    add_executable(${TEST_NAME} ${SRCS})
    target_link_libraries(${TEST_NAME} PRIVATE test_helper test_runner graph_test)
    if (ENABLE_BACKTRACES)
        target_link_libraries(${TEST_NAME} PRIVATE register_backtrace_signal_handler zstd)
    endif()
    target_include_directories(${TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/test/include)
    include(GoogleTest)

    if (TEST_NAME STREQUAL "e2e_test")
        gtest_discover_tests(${TEST_NAME}
            DISCOVERY_TIMEOUT 600
            DISCOVERY_MODE PRE_TEST
            TEST_PREFIX e2e_test_
        )
    else()
        gtest_discover_tests(${TEST_NAME}
            DISCOVERY_TIMEOUT 600
            DISCOVERY_MODE PRE_TEST
        )
    endif()
endfunction()

function(add_gorgonzola_api_test TEST_NAME)
    set(SRCS ${ARGN})
    add_executable(${TEST_NAME} ${SRCS})
    target_link_libraries(${TEST_NAME} PRIVATE api_graph_test api_test_helper)
    if (ENABLE_BACKTRACES)
        target_link_libraries(${TEST_NAME} PRIVATE register_backtrace_signal_handler zstd)
    endif()
    target_include_directories(${TEST_NAME} PRIVATE ${PROJECT_SOURCE_DIR}/test/include)
    include(GoogleTest)
    gtest_discover_tests(${TEST_NAME})
endfunction()

macro(add_kuzu_test TEST_NAME)
    add_gorgonzola_test(${TEST_NAME} ${ARGN})
endmacro()

macro(add_kuzu_api_test TEST_NAME)
    add_gorgonzola_api_test(${TEST_NAME} ${ARGN})
endmacro()

# Windows doesn't support dynamic lookup, so we have to link extensions against gorgonzola.
if (MSVC AND (NOT BUILD_EXTENSIONS EQUAL ""))
    set(BUILD_GORGONZOLA TRUE)
endif ()

include_directories(modules/third_party/antlr4_cypher/include)
include_directories(modules/third_party/antlr4_runtime/src)
if(NOT DEFINED GORGONZOLA_ENABLE_PARQUET OR GORGONZOLA_ENABLE_PARQUET)
    include_directories(modules/third_party/brotli/c/include)
endif()
include_directories(modules/third_party/fast_float/include)
include_directories(modules/third_party/mbedtls/include)
if(NOT DEFINED GORGONZOLA_ENABLE_PARQUET OR GORGONZOLA_ENABLE_PARQUET)
    include_directories(modules/third_party/parquet)
    include_directories(modules/third_party/snappy)
    include_directories(modules/third_party/thrift)
endif()
include_directories(modules/third_party/miniz)
include_directories(modules/third_party/yyjson/src)
include_directories(modules/third_party/nlohmann_json)
include_directories(modules/third_party/pybind11/include)
include_directories(modules/third_party/pyparse)
include_directories(modules/third_party/re2/include)
include_directories(modules/third_party/alp/include)
if (${BUILD_TESTS} OR ${BUILD_EXTENSION_TESTS})
    include_directories(modules/third_party/spdlog)
elseif (${BUILD_BENCHMARK})
    include_directories(modules/third_party/spdlog)
endif ()
include_directories(modules/third_party/utf8proc/include)
include_directories(modules/third_party/zstd/include)
include_directories(modules/third_party/httplib)
include_directories(modules/third_party/pcg)
include_directories(modules/third_party/lz4)
include_directories(modules/third_party/roaring_bitmap)
include_directories(modules/third_party/glob)
# Use SYSTEM to suppress warnings from simsimd
if(NOT DEFINED GORGONZOLA_ENABLE_SIMSIMD OR GORGONZOLA_ENABLE_SIMSIMD)
    include_directories(SYSTEM modules/third_party/simsimd/include)
endif()

add_subdirectory(modules/third_party)

add_definitions(-DGORGONZOLA_ROOT_DIRECTORY="${PROJECT_SOURCE_DIR}")
add_definitions(-DGORGONZOLA_CMAKE_VERSION="${CMAKE_PROJECT_VERSION}")
add_definitions(-DGORGONZOLA_EXTENSION_VERSION="0.11.1")

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/modules/gorgonzola-db-extensions/CMakeLists.txt")
    add_subdirectory(modules/gorgonzola-db-extensions)
endif()

if (EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/extension/CMakeLists.txt")
    add_subdirectory(extension)
endif ()

if(BUILD_GORGONZOLA)

include_directories(
    src/include
    ${CMAKE_CURRENT_BINARY_DIR}/src/include
)

if(EXISTS "${CMAKE_SOURCE_DIR}/cmake/generated/build_optimization.cmake")
    include("${CMAKE_SOURCE_DIR}/cmake/generated/build_optimization.cmake")
endif()

add_subdirectory(src)

# Link extensions which require static linking.
foreach(ext IN LISTS STATICALLY_LINKED_EXTENSIONS)
    if (${BUILD_EXTENSION_TESTS})
        add_compile_definitions(__STATIC_LINK_EXTENSION_TEST__)
    endif ()
    target_link_libraries(gorgonzola PRIVATE "gorgonzola_${ext}_static_extension")
    target_link_libraries(gorgonzola_shared PRIVATE "gorgonzola_${ext}_static_extension")
endforeach()

if (${BUILD_TESTS} OR ${BUILD_EXTENSION_TESTS})
    add_subdirectory(modules/test test)

elseif (${BUILD_BENCHMARK})
    add_subdirectory(modules/test/test_helper test/test_helper)
endif ()
if(${BUILD_SHELL})
    add_subdirectory(shell)
endif()
if(${BUILD_BENCHMARK})
    add_subdirectory(modules/benchmark/cpp)
endif()
endif ()

if (${BUILD_EXAMPLES})
    add_subdirectory(modules/examples/c)
    add_subdirectory(modules/examples/cpp)
endif()
