add_library(gorgonzola_function_uuid
        OBJECT
        gen_random_uuid.cpp)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:gorgonzola_function_uuid>
        PARENT_SCOPE)
