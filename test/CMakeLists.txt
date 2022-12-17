cmake_minimum_required(VERSION 3.15...3.23)

project(CmakeConfigPackageTests LANGUAGES CXX)

# ---- Test as standalone project the exported config package ----

if(PROJECT_IS_TOP_LEVEL)
  enable_testing()

  find_package(adventofcode22 CONFIG REQUIRED) # for intro, project_options, ...

  if(NOT TARGET adventofcode22::project_options)
    message(FATAL_ERROR "Required config package not found!")
    return() # be strictly paranoid for Template Janitor github action! CK
  endif()
endif()

# ---- Dependencies ----

find_package(Catch2 CONFIG REQUIRED)
include(Catch)

add_executable(tests 
    tests.cpp
)
target_link_libraries(tests 
    PRIVATE 
        adventofcode22::project_options 
        adventofcode22::project_warnings
        Catch2::Catch2
        Catch2::Catch2WithMain
)
catch_discover_tests(tests)

# Add a file containing a set of constexpr tests
add_executable(constexpr_tests 
    constexpr_tests.cpp
)
target_link_libraries(constexpr_tests 
    PRIVATE 
        adventofcode22::project_options 
        adventofcode22::project_warnings
        Catch2::Catch2
        Catch2::Catch2WithMain
)

catch_discover_tests(constexpr_tests)

# Disable the constexpr portion of the test, and build again this allows us to have an executable that we can debug when
# things go wrong with the constexpr testing
add_executable(relaxed_constexpr_tests 
    constexpr_tests.cpp
)
target_link_libraries(relaxed_constexpr_tests 
    PRIVATE 
        adventofcode22::project_options 
        adventofcode22::project_warnings
        Catch2::Catch2
        Catch2::Catch2WithMain
)
target_compile_definitions(relaxed_constexpr_tests 
    PRIVATE 
        -DCATCH_CONFIG_RUNTIME_STATIC_REQUIRE
)
catch_discover_tests(relaxed_constexpr_tests)
