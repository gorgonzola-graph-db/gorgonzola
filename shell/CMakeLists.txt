include_directories(
        include
        ${PROJECT_SOURCE_DIR}/modules/third_party/taywee_args/include
        ${PROJECT_SOURCE_DIR}/modules/third_party/yyjson/src
        ${PROJECT_SOURCE_DIR}/modules/gorgonzola-db-extensions/json/src/include
)

add_subdirectory(printer)

add_executable(gorgonzola_shell
        ${GORGONZOLA_SHELL_OBJECT_FILES}
        embedded_shell.cpp
        linenoise.cpp
        shell_runner.cpp)

if (NOT MSVC)
    # On Windows, rename the executable to gorgonzola.exe will cause an error
    # "multiple rules generate src/gorgonzola.lib", so we do not set the output
    # name for MSVC.
    set_target_properties(gorgonzola_shell PROPERTIES OUTPUT_NAME gorgonzola)
endif ()


target_link_libraries(gorgonzola_shell gorgonzola)
add_dependencies(gorgonzola_shell gorgonzola_foundation gorgonzola_frontend gorgonzola_api)
add_dependencies(gorgonzola_shell_printer gorgonzola_foundation gorgonzola_execution gorgonzola_api gorgonzola_storage)
if (ENABLE_BACKTRACES)
    target_link_libraries(gorgonzola_shell register_backtrace_signal_handler)
endif ()
if (MSVC)
    target_link_libraries(gorgonzola_shell ws2_32)
endif ()

install(TARGETS gorgonzola_shell)

if(ENABLE_PCH)
    target_precompile_headers(gorgonzola_shell PRIVATE ${PROJECT_SOURCE_DIR}/shell/include/shell_pch.h)
endif()
