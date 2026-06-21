include_directories(
        ./include
        ${PROJECT_SOURCE_DIR}/test/include/test_helper)

add_executable(gorgonzola_benchmark
        benchmark.cpp
        benchmark_parser.cpp
        benchmark_runner.cpp
        main.cpp)

target_link_libraries(gorgonzola_benchmark gorgonzola test_helper)
