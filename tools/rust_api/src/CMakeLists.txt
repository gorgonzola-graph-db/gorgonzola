add_library(gorgonzola_rs STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/gorgonzola_rs.cpp
    ${GORGONZOLA_RS_BINDINGS_DIR}/sources/gorgonzola/src/lib.rs.cc
)

target_include_directories(gorgonzola_rs
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/../include
        ${GORGONZOLA_RS_BINDINGS_DIR}/include
        ${GORGONZOLA_RS_BINDINGS_DIR}/crate
        ${GORGONZOLA_RS_BINDINGS_DIR}/sources
)

target_link_libraries(gorgonzola_rs gorgonzola)
