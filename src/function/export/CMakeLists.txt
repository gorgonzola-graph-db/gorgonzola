add_library(gorgonzola_function_export
        OBJECT
        export_csv_function.cpp
        $<$<NOT:$<BOOL:${GORGONZOLA_LITE}>>:export_parquet_function.cpp>)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:gorgonzola_function_export>
        PARENT_SCOPE)
