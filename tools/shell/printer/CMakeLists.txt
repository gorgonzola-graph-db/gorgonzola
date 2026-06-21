include_directories(
    ${PROJECT_SOURCE_DIR}/third_party/yyjson/src
    ${PROJECT_SOURCE_DIR}/extension/json/src/include
)

add_library(gorgonzola_shell_printer
        OBJECT
        json_printer.cpp
        printer_factory.cpp
        ${PROJECT_SOURCE_DIR}/extension/json/src/utils/json_utils.cpp
        ${PROJECT_SOURCE_DIR}/extension/json/src/type/json_type.cpp
)

set(GORGONZOLA_SHELL_OBJECT_FILES
        ${GORGONZOLA_SHELL_OBJECT_FILES} $<TARGET_OBJECTS:gorgonzola_shell_printer>
        PARENT_SCOPE)
