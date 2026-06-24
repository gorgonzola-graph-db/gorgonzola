include_directories(
    ${PROJECT_SOURCE_DIR}/third_party/yyjson/src
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_library(gorgonzola_shell_printer
        OBJECT
        json_printer.cpp
        printer_factory.cpp
        json_utils.cpp
        json_type.cpp
)

set(GORGONZOLA_SHELL_OBJECT_FILES
        ${GORGONZOLA_SHELL_OBJECT_FILES} $<TARGET_OBJECTS:gorgonzola_shell_printer>
        PARENT_SCOPE)
