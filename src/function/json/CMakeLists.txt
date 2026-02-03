add_library(gorgonzola_function_json
        OBJECT
        buffered_json_reader.cpp
        json_common.cpp
        json_type.cpp
        json_utils.cpp
        cast_functions/json_cast.cpp
        creation_functions/json_array.cpp
        creation_functions/json_merge_patch.cpp
        creation_functions/json_object.cpp
        creation_functions/to_json.cpp
        export_functions/json_export.cpp
        extract_functions/json_extract.cpp
        scalar_functions/json_array_length.cpp
        scalar_functions/json_contains.cpp
        scalar_functions/json_keys.cpp
        scalar_functions/json_minify.cpp
        scalar_functions/json_structure.cpp
        scalar_functions/json_valid.cpp
        table_functions/json_scan.cpp
)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:gorgonzola_function_json>
        PARENT_SCOPE)

target_include_directories(gorgonzola_function_json PRIVATE ${CMAKE_SOURCE_DIR}/third_party/yyjson/src)
