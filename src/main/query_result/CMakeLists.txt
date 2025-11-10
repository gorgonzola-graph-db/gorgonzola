add_library(gorgonzola_main_query_result
        OBJECT
        arrow_query_result.cpp
        materialized_query_result.cpp)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:gorgonzola_main_query_result>
        PARENT_SCOPE)
