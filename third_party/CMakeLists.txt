#
# Catch 2
#
add_library(Catch2 catch_main.cpp)
target_include_directories(Catch2 PUBLIC "Catch2/single_include")
target_compile_features(Catch2 PUBLIC cxx_std_17)
add_library(Catch2::Catch2 ALIAS Catch2)
