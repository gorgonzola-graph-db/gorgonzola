add_library(gorgonzola_api
        OBJECT
        database.cc
        session.cc
        transaction.cc
        query.cc
        result.cc
        version.cc)

target_include_directories(gorgonzola_api PRIVATE
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/src/include
        ${CMAKE_CURRENT_BINARY_DIR}/..)

set(ALL_OBJECT_FILES
        ${ALL_OBJECT_FILES} $<TARGET_OBJECTS:gorgonzola_api>
        PARENT_SCOPE)
