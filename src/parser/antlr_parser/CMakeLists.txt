add_library(gorgonzola_parser_antlr_parser
        OBJECT
        gorgonzola_cypher_parser.cpp
        parser_error_listener.cpp
        parser_error_strategy.cpp)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:gorgonzola_parser_antlr_parser>
        PARENT_SCOPE)
