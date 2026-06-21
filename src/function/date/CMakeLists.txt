add_library(gorgonzola_function_date
        OBJECT
        date_functions.cpp)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:gorgonzola_function_date>
        PARENT_SCOPE)
