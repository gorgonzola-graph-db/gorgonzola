include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Have to pass this down to every subdirectory, which actually adds the files.
# This doesn't affect parent directories.
add_compile_definitions(GORGONZOLA_EXPORTS)
add_compile_definitions(ANTLR4CPP_STATIC)

set(INCLUDES "")
set(LOAD_LINKED_EXTENSION "")

if(NOT GORGONZOLA_LITE OR GORGONZOLA_LITE_ENABLE_EXTENSIONS)
foreach (EXT_NAME IN LISTS STATICALLY_LINKED_EXTENSIONS)
    string(TOLOWER "${EXT_NAME}" EXT_NAME_LOWER)
    string(REPLACE "_" ";" EXT_NAME_SPLIT ${EXT_NAME})
    set(EXT_NAME_CAMELCASE "")
    foreach (EXT_NAME_PART IN LISTS EXT_NAME_SPLIT)
        string(SUBSTRING ${EXT_NAME_PART} 0 1 FIRST_LETTER)
        string(SUBSTRING ${EXT_NAME_PART} 1 -1 REMAINDER)
        string(TOUPPER ${FIRST_LETTER} FIRST_LETTER)
        set(EXT_NAME_CAMELCASE "${EXT_NAME_CAMELCASE}${FIRST_LETTER}${REMAINDER}")
    endforeach ()
    set(LOAD_LINKED_EXTENSION "${LOAD_LINKED_EXTENSION}\
{
        ${EXT_NAME_LOWER}_extension::${EXT_NAME_CAMELCASE}Extension extension{};
        extension.load(context);
        loadedExtensions.push_back(LoadedExtension(${EXT_NAME_LOWER}_extension::${EXT_NAME_CAMELCASE}Extension::EXTENSION_NAME, \" \", ExtensionSource::STATIC_LINKED));
    }\n")
    include_directories(${PROJECT_SOURCE_DIR}/extension/${EXT_NAME}/src/include/main)
    set(INCLUDES "${INCLUDES}#include \"${EXT_NAME_LOWER}_extension.h\"\n")
endforeach ()
endif()

configure_file(
        "extension/generated_extension_loader.h.in"
        "${CMAKE_CURRENT_BINARY_DIR}/codegen/include/generated_extension_loader.h"
        @ONLY
)

configure_file(
        "extension/generated_extension_loader.cpp.in"
        "${CMAKE_CURRENT_BINARY_DIR}/codegen/generated_extension_loader.cpp"
        @ONLY
)

set(GENERATED_CPP_FILE ${CMAKE_CURRENT_BINARY_DIR}/codegen/generated_extension_loader.cpp)

file(GLOB_RECURSE GORGONZOLA_SOURCES CONFIGURE_DEPENDS
    "*.cpp"
)

if(GORGONZOLA_LITE AND NOT GORGONZOLA_LITE_ENABLE_GDS)
    file(GLOB_RECURSE GDS_SOURCES
        "function/gds/asp_destinations.cpp"
        "function/gds/asp_paths.cpp"
        "function/gds/awsp_paths.cpp"
        "function/gds/leiden.cpp"
        "function/gds/wsp_destinations.cpp"
        "function/gds/wsp_paths.cpp"
        "function/gds/algo/*.cpp"
    )
    foreach(GDS_FILE IN LISTS GDS_SOURCES)
        list(REMOVE_ITEM GORGONZOLA_SOURCES ${GDS_FILE})
    endforeach()
endif()

if(GORGONZOLA_LITE AND NOT GORGONZOLA_LITE_ENABLE_JSON)
    file(GLOB_RECURSE JSON_SOURCES
        "function/json/*.cpp"
    )
    foreach(JSON_FILE IN LISTS JSON_SOURCES)
        list(REMOVE_ITEM GORGONZOLA_SOURCES ${JSON_FILE})
    endforeach()
endif()

if(GORGONZOLA_LITE)
    file(GLOB_RECURSE ARROW_SOURCES
        "main/query_result/arrow_query_result.cpp"
        "processor/map/create_arrow_result_collector.cpp"
        "processor/operator/arrow_result_collector.cpp"
    )
    foreach(ARROW_FILE IN LISTS ARROW_SOURCES)
        list(REMOVE_ITEM GORGONZOLA_SOURCES ${ARROW_FILE})
    endforeach()
endif()

add_library(gorgonzola-core STATIC
    ${GORGONZOLA_SOURCES}
    ${GENERATED_CPP_FILE}
)

if(NOT GORGONZOLA_LITE)
    set(GORGONZOLA_LIBRARIES antlr4_cypher antlr4_runtime brotlidec brotlicommon fast_float utf8proc re2 fastpfor parquet snappy thrift yyjson zstd miniz mbedtls lz4 roaring_bitmap simsimd Glob libalp)
else()
    set(GORGONZOLA_LIBRARIES antlr4_cypher antlr4_runtime fast_float utf8proc re2 fastpfor yyjson zstd miniz mbedtls lz4 roaring_bitmap Glob)
endif()
if (NOT __SINGLE_THREADED__)
        set(GORGONZOLA_LIBRARIES ${GORGONZOLA_LIBRARIES} Threads::Threads)
endif()
if(NOT WIN32)
        set(GORGONZOLA_LIBRARIES dl ${GORGONZOLA_LIBRARIES})
endif()
# Seems to be needed for clang on linux only
# for compiling std::atomic<T>::compare_exchange_weak
if ((NOT APPLE AND CMAKE_CXX_COMPILER_ID STREQUAL "Clang") AND NOT __WASM__ AND NOT __SINGLE_THREADED__)
        set(GORGONZOLA_LIBRARIES atomic ${GORGONZOLA_LIBRARIES})
endif()
if (ENABLE_BACKTRACES)
        set(GORGONZOLA_LIBRARIES ${GORGONZOLA_LIBRARIES} cpptrace::cpptrace)
endif()
target_link_libraries(gorgonzola-core PUBLIC ${GORGONZOLA_LIBRARIES})
unset(GORGONZOLA_LIBRARIES)

set(GORGONZOLA_INCLUDES $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include/c_api ${PROJECT_SOURCE_DIR}/include "${CMAKE_CURRENT_BINARY_DIR}/codegen/include/" "${PROJECT_SOURCE_DIR}/third_party/parquet" "${PROJECT_SOURCE_DIR}/third_party/thrift" "${PROJECT_SOURCE_DIR}/third_party/snappy" "${PROJECT_SOURCE_DIR}/third_party/brotli/c/include")
target_include_directories(gorgonzola-core PUBLIC ${GORGONZOLA_INCLUDES})
unset(GORGONZOLA_INCLUDES)

set_property(TARGET gorgonzola-core PROPERTY POSITION_INDEPENDENT_CODE ON)
set_property(TARGET gorgonzola-core PROPERTY OUTPUT_NAME gorgonzola)
add_library(gorgonzola ALIAS gorgonzola-core)
add_library(gorgonzola_shared ALIAS gorgonzola-core)

if(WIN32)
        # Anything linking against the static library must not use dllimport.
        target_compile_definitions(gorgonzola-core INTERFACE GORGONZOLA_STATIC_DEFINE)
endif()

if(ENABLE_PCH)
    target_precompile_headers(gorgonzola-core
        PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h
    )
endif()

install(TARGETS gorgonzola-core)

if(${BUILD_SINGLE_FILE_HEADER})
        # Create a command to generate gorgonzola.hpp, and then create a target that is
        # always built that depends on it. This allows our generator to detect when
        # exactly to build gorgonzola.hpp, while still building the target by default.
        find_package(Python3 3.9...4 REQUIRED)
        add_custom_command(
                OUTPUT gorgonzola.hpp
                COMMAND
                        ${Python3_EXECUTABLE} ${PROJECT_SOURCE_DIR}/scripts/collect-single-file-header.py ${CMAKE_CURRENT_BINARY_DIR}/..
                DEPENDS
                        ${PROJECT_SOURCE_DIR}/scripts/collect-single-file-header.py gorgonzola_shared)
        add_custom_target(single_file_header ALL DEPENDS gorgonzola.hpp)
endif()

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/include/c_api/gorgonzola.h TYPE INCLUDE)

if(${BUILD_SINGLE_FILE_HEADER})
        install(FILES ${CMAKE_CURRENT_BINARY_DIR}/gorgonzola.hpp  TYPE INCLUDE)
endif()
