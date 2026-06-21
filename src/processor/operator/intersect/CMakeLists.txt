add_library(gorgonzola_processor_operator_intersect
        OBJECT
        intersect.cpp)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:gorgonzola_processor_operator_intersect>
        PARENT_SCOPE)
