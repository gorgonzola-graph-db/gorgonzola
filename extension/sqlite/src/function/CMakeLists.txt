add_library(gorgonzola_sqlite_scan
        OBJECT
        sqlite_scan.cpp)

set(SQLITE_EXTENSION_OBJECT_FILES
        ${SQLITE_EXTENSION_OBJECT_FILES} $<TARGET_OBJECTS:gorgonzola_sqlite_scan>
        PARENT_SCOPE)
