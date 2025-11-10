add_library(gorgonzola_binder_bind_copy
        OBJECT
        bind_copy_to.cpp
        bind_copy_from.cpp)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:gorgonzola_binder_bind_copy>
        PARENT_SCOPE)
