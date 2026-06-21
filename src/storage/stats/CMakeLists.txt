add_library(gorgonzola_storage_stats
        OBJECT
        column_stats.cpp
        hyperloglog.cpp
        table_stats.cpp)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:gorgonzola_storage_stats>
        PARENT_SCOPE)
