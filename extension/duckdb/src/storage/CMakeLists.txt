add_library(gorgonzola_duckdb_storage
        OBJECT
        duckdb_storage.cpp)

set(DUCKDB_EXTENSION_OBJECT_FILES
        ${DUCKDB_EXTENSION_OBJECT_FILES} $<TARGET_OBJECTS:gorgonzola_duckdb_storage>
        PARENT_SCOPE)
