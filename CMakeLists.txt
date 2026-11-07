cmake_minimum_required(VERSION 3.25)
project(scan VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
# clang-tidy reads this.
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if(MSVC)
    # The sources hold Cyrillic comments: without /utf-8 cl.exe reads them in the system code page.
    add_compile_options(/utf-8)
endif()

function(warn_as_errors target)
    if(NOT MSVC)
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    endif()
endfunction()

add_library(scan INTERFACE)
target_include_directories(scan INTERFACE include/)

include(CTest)

add_executable(scan_tests tests/main.cpp)
target_link_libraries(scan_tests PRIVATE scan)
warn_as_errors(scan_tests)
add_test(NAME scan_tests COMMAND scan_tests)

# tests/true_*.cpp must compile and run.
file(GLOB true_sources "tests/true_*.cpp")
foreach(source ${true_sources})
    get_filename_component(name ${source} NAME_WE)
    add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE scan)
    warn_as_errors(${name})
    add_test(NAME ${name} COMMAND ${name})
endforeach()

# tests/fail_*.cpp must be refused, and each entry here says how many times and for what: the text of the
# line that throws. A refusal alone proves nothing, because a missing header refuses every one of them.
set(refusal_fail_no_numer_for_placefolder 1 "This is not a number")
set(refusal_fail_parse_input_empty_incorrect_int 2 "This number has unother symbols")
set(refusal_fail_parse_input_empty_int 1 "Input is empty. Parsing error")
set(refusal_fail_parse_input_not_int 2 "This is not a number")
set(refusal_fail_parse_input_unknown_type 1 "Unsupported type")
set(refusal_fail_scan_count_not_equal 1 "The count of types are not the same as count of placeholders")
# The format string's error travels in std::expected, so what throws is value(), inside the standard library.
set(refusal_fail_unknown_placeholder 1 "bad_expected_access")

file(GLOB fail_sources "tests/fail_*.cpp")
if(MSVC)
    # cl.exe words its diagnostics differently, so this branch only sees that the compiler refused.
    add_custom_target(check-compile-fails)
endif()
foreach(source ${fail_sources})
    get_filename_component(name ${source} NAME_WE)
    if(MSVC)
        add_custom_target(${name}
            COMMAND ${CMAKE_COMMAND}
                    "-DEXPECTED_FAILURE_COMPILER=${CMAKE_CXX_COMPILER}"
                    "-DEXPECTED_FAILURE_SOURCE=${source}"
                    "-DEXPECTED_FAILURE_INCLUDE_DIR=${CMAKE_SOURCE_DIR}/include"
                    "-DEXPECTED_FAILURE_OBJECT=${CMAKE_BINARY_DIR}/expected-failures/${name}.obj"
                    -P "${CMAKE_SOURCE_DIR}/cmake/expect_compile_failure.cmake"
            COMMENT "Checking expected failure: ${source}"
            VERBATIM
        )
        add_dependencies(check-compile-fails ${name})
    else()
        if(NOT DEFINED refusal_${name})
            message(FATAL_ERROR "${name} has no refusal_${name}: nothing would say why it must fail.")
        endif()
        list(GET refusal_${name} 0 refusals)
        list(GET refusal_${name} 1 reason)
        add_test(
            NAME ${name}
            COMMAND ${CMAKE_COMMAND}
                    "-DCOMPILER=${CMAKE_CXX_COMPILER}"
                    "-DSOURCE=${source}"
                    "-DINCLUDE_DIR=${CMAKE_SOURCE_DIR}/include"
                    "-DOBJECT=${CMAKE_BINARY_DIR}/expected-refusals/${name}.o"
                    "-DREFUSALS=${refusals}"
                    "-DREASON=${reason}"
                    -P "${CMAKE_SOURCE_DIR}/cmake/expect_compile_refusal.cmake"
        )
    endif()
endforeach()

# The manual list is judged where every other file is judged (section 9).
add_test(
    NAME manual_list
    COMMAND python -m pytest tests/test_manual_list.py -q -p no:cacheprovider
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
