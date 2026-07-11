add_subdirectory(csv)
if(NOT GORGONZOLA_LITE)
    add_subdirectory(npy)
    add_subdirectory(parquet)
endif()
add_library(gorgonzola_processor_operator_persistent_reader
        OBJECT
        copy_from_error.cpp
        file_error_handler.cpp
        reader_bind_utils.cpp)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:gorgonzola_processor_operator_persistent_reader>
        PARENT_SCOPE)
