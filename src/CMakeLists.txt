include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Have to pass this down to every subdirectory, which actually adds the files.
# This doesn't affect parent directories.
add_compile_definitions(GORGONZOLA_EXPORTS)
add_compile_definitions(ANTLR4CPP_STATIC)
add_subdirectory(binder)
add_subdirectory(c_api)
add_subdirectory(catalog)
add_subdirectory(common)
add_subdirectory(expression_evaluator)
add_subdirectory(function)
add_subdirectory(graph)
add_subdirectory(main)
add_subdirectory(optimizer)
add_subdirectory(parser)
add_subdirectory(planner)
add_subdirectory(processor)
add_subdirectory(storage)
add_subdirectory(transaction)
add_subdirectory(extension)
# ── Precompiled Header ────────────────────────────────────────────
# PCH must be applied to the OBJECT libraries where compilation occurs,
# not to the final STATIC/SHARED targets (which have no source files).
option(ENABLE_PCH "Enable precompiled headers to speed up compilation" ON)
if(ENABLE_PCH)
        # Create a dedicated target to own the PCH compilation.
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/pch_dummy.cpp "")
        add_library(gorgonzola_pch OBJECT ${CMAKE_CURRENT_BINARY_DIR}/pch_dummy.cpp)
        target_precompile_headers(gorgonzola_pch PUBLIC
                ${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h)

        # Recursively collect all OBJECT library targets defined under src/.
        function(gorgonzola_collect_object_targets DIR OUT_VAR)
                get_property(_targets DIRECTORY ${DIR} PROPERTY BUILDSYSTEM_TARGETS)
                set(_result)
                foreach(_t IN LISTS _targets)
                        get_target_property(_type ${_t} TYPE)
                        if(_type STREQUAL "OBJECT_LIBRARY" AND NOT _t STREQUAL "gorgonzola_pch")
                                list(APPEND _result ${_t})
                        endif()
                endforeach()
                get_property(_subdirs DIRECTORY ${DIR} PROPERTY SUBDIRECTORIES)
                foreach(_sd IN LISTS _subdirs)
                        gorgonzola_collect_object_targets(${_sd} _sub_result)
                        list(APPEND _result ${_sub_result})
                endforeach()
                set(${OUT_VAR} ${_result} PARENT_SCOPE)
        endfunction()

        gorgonzola_collect_object_targets(${CMAKE_CURRENT_SOURCE_DIR} _ALL_OBJECT_TARGETS)
        foreach(_target IN LISTS _ALL_OBJECT_TARGETS)
                target_precompile_headers(${_target} REUSE_FROM gorgonzola_pch)
        endforeach()

        message(STATUS "PCH enabled for ${CMAKE_CURRENT_SOURCE_DIR}/include/pch.h")
        list(LENGTH _ALL_OBJECT_TARGETS _pch_count)
        message(STATUS "PCH applied to ${_pch_count} OBJECT libraries")
endif()

add_library(gorgonzola STATIC ${ALL_OBJECT_FILES})
add_library(gorgonzola_shared SHARED ${ALL_OBJECT_FILES})

set(GORGONZOLA_LIBRARIES antlr4_cypher antlr4_runtime brotlidec brotlicommon fast_float utf8proc re2 fastpfor parquet snappy thrift yyjson zstd miniz mbedtls lz4 roaring_bitmap simsimd)
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
target_link_libraries(gorgonzola PUBLIC ${GORGONZOLA_LIBRARIES})
target_link_libraries(gorgonzola_shared PUBLIC ${GORGONZOLA_LIBRARIES})
unset(GORGONZOLA_LIBRARIES)

set(GORGONZOLA_INCLUDES $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include> $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}> ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include/c_api)
target_include_directories(gorgonzola PUBLIC ${GORGONZOLA_INCLUDES})
target_include_directories(gorgonzola_shared PUBLIC ${GORGONZOLA_INCLUDES})
unset(GORGONZOLA_INCLUDES)

if(WIN32)
        # Anything linking against the static library must not use dllimport.
        target_compile_definitions(gorgonzola INTERFACE GORGONZOLA_STATIC_DEFINE)
endif()

if(NOT WIN32)
        set_target_properties(gorgonzola_shared PROPERTIES OUTPUT_NAME gorgonzola)
endif()

install(TARGETS gorgonzola gorgonzola_shared)

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
