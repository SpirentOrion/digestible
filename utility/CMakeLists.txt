add_executable(utility main.cpp)
target_compile_features(utility PRIVATE cxx_std_17)
target_include_directories(utility PRIVATE ../include)
