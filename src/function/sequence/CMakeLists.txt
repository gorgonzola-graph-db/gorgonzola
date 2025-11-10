add_library(gorgonzola_function_sequence
        OBJECT
        sequence_functions.cpp)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:gorgonzola_function_sequence>
        PARENT_SCOPE)
