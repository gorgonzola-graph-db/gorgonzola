set(QUERY_RESULT_FILES materialized_query_result.cpp)
if(NOT GORGONZOLA_LITE)
    list(APPEND QUERY_RESULT_FILES arrow_query_result.cpp)
endif()

add_library(gorgonzola_main_query_result OBJECT ${QUERY_RESULT_FILES})

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:gorgonzola_main_query_result>
        PARENT_SCOPE)
