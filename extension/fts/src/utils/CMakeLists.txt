add_library(gorgonzola_fts_utils
        OBJECT
        fts_utils.cpp)

set(FTS_EXTENSION_OBJECT_FILES
        ${FTS_EXTENSION_OBJECT_FILES} $<TARGET_OBJECTS:gorgonzola_fts_utils>
        PARENT_SCOPE)
