cmake_minimum_required(VERSION 3.25)
project(scan VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(MSVC)
    # Исходники и сообщения об ошибках содержат кириллицу. Без /utf-8 cl.exe
    # читает их в кодовой странице системы (на этой машине CP1251) и ломается
    # на первом же комментарии. На GCC ничего не меняется: ветка только MSVC.
    add_compile_options(/utf-8)
endif()

set(target scan)

add_library(${target} INTERFACE)

target_include_directories(${target} INTERFACE include/)

set(test_target scan_tests)

add_executable(${test_target} tests/main.cpp)
target_link_libraries(${test_target} PRIVATE ${target})


file(GLOB ALL_TEST_SOURCES "tests/*.cpp")

include(CTest)

file(GLOB ALL_TEST_SOURCES "tests/*.cpp")

add_custom_target(check-compile-fails)

foreach(TEST_SOURCE ${ALL_TEST_SOURCES})
    get_filename_component(TEST_NAME ${TEST_SOURCE} NAME_WE)

    # Тесты, которые должны УСПЕШНО компилироваться и запускаться (с true_)
    if(TEST_NAME MATCHES "^true_")
        set(TEST_TARGET ${TEST_NAME})
        add_executable(${TEST_TARGET} ${TEST_SOURCE})
        target_link_libraries(${TEST_TARGET} PRIVATE ${target})
        add_test(NAME ${TEST_TARGET} COMMAND $<TARGET_FILE:${TEST_TARGET}>)

    # Тесты, которые должны ПРОВАЛИВАТЬ компиляцию ( с fail_)
    elseif(TEST_NAME MATCHES "^fail_")
        set(TEST_TARGET ${TEST_NAME})
        if(MSVC)
            # У cl.exe другой синтаксис ключей, а в cmd.exe нет ни `!`, ни
            # /dev/null, поэтому инверсию результата делает отдельный
            # cmake -P скрипт. Ветка для sh ниже остаётся нетронутой.
            add_custom_target(${TEST_TARGET}
                COMMAND ${CMAKE_COMMAND}
                        "-DEXPECTED_FAILURE_COMPILER=${CMAKE_CXX_COMPILER}"
                        "-DEXPECTED_FAILURE_SOURCE=${TEST_SOURCE}"
                        "-DEXPECTED_FAILURE_INCLUDE_DIR=${CMAKE_SOURCE_DIR}/include"
                        "-DEXPECTED_FAILURE_OBJECT=${CMAKE_BINARY_DIR}/expected-failures/${TEST_NAME}.obj"
                        -P "${CMAKE_SOURCE_DIR}/cmake/expect_compile_failure.cmake"
                COMMENT "Checking expected failure: ${TEST_SOURCE}"
                VERBATIM
            )
        else()
            add_custom_target(${TEST_TARGET}
                COMMAND sh -c "! ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_FLAGS} -I${CMAKE_SOURCE_DIR}/include -std=c++23 -c ${TEST_SOURCE} -o /dev/null"
                COMMENT "Checking expected failure: ${TEST_SOURCE}"
                VERBATIM
            )
        endif()
        add_dependencies(check-compile-fails ${TEST_TARGET})
    endif()
endforeach()