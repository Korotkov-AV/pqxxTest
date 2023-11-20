cmake_minimum_required(VERSION 3.20)

set(PostgreSQL_ADDITIONAL_VERSIONS "16")

project (pqxxTest)

add_subdirectory("D:\\work\\libpqxx-7.8.1" libpqxx-build)

add_executable(pqxxTest main.cpp)

target_compile_features(pqxxTest PRIVATE cxx_std_17)

target_link_libraries(pqxxTest pqxx)