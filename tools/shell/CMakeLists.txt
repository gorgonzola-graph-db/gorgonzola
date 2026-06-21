include_directories(
        include
        ${PROJECT_SOURCE_DIR}/third_party/taywee_args/include
        ${PROJECT_SOURCE_DIR}/third_party/yyjson/src
        ${PROJECT_SOURCE_DIR}/extension/json/src/include
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
if (ENABLE_BACKTRACES)
    target_link_libraries(gorgonzola_shell register_backtrace_signal_handler)
endif ()
if (MSVC)
    target_link_libraries(gorgonzola_shell ws2_32)
endif ()

install(TARGETS gorgonzola_shell)
