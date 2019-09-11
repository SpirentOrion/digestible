
add_executable(test_digestible tests.cpp)
target_include_directories(test_digestible PRIVATE ../include)

target_compile_features(test_digestible PRIVATE cxx_std_17)

add_test(NAME digestible_test COMMAND test_digestible)
