# Инвертирует результат одной компиляции: скрипт завершается успешно только
# тогда, когда компилятор ОТКАЗАЛСЯ собирать файл.
#
# Зачем отдельный файл: на Linux то же самое делает `sh -c "! g++ ..."`, но в
# cmd.exe нет оператора `!`, нет `/dev/null` и нет предсказуемого разбора
# кавычек внутри `add_custom_target`. Прогон через `cmake -P` не зависит от
# оболочки вообще, поэтому одна и та же логика работает под MSVC.
#
# Вызывается так:
#   cmake -DEXPECTED_FAILURE_COMPILER=<cl.exe>
#         -DEXPECTED_FAILURE_SOURCE=<...\fail_xxx.cpp>
#         -DEXPECTED_FAILURE_INCLUDE_DIR=<...\include>
#         -DEXPECTED_FAILURE_OBJECT=<...\fail_xxx.obj>
#         -P cmake/expect_compile_failure.cmake

cmake_minimum_required(VERSION 3.25)

foreach(required_variable
        EXPECTED_FAILURE_COMPILER
        EXPECTED_FAILURE_SOURCE
        EXPECTED_FAILURE_INCLUDE_DIR
        EXPECTED_FAILURE_OBJECT)
    if(NOT DEFINED ${required_variable})
        message(FATAL_ERROR "expect_compile_failure.cmake: не задан -D${required_variable}=...")
    endif()
endforeach()

get_filename_component(expected_failure_object_directory "${EXPECTED_FAILURE_OBJECT}" DIRECTORY)
file(MAKE_DIRECTORY "${expected_failure_object_directory}")

# /std:c++latest, а не /std:c++23: MSVC 19.4x ещё не принимает `c++23` как
# значение переключателя, хотя нужные возможности C++23 в нём уже есть.
# /utf-8 обязателен — в исходниках проекта есть комментарии на кириллице.
execute_process(
    COMMAND "${EXPECTED_FAILURE_COMPILER}"
            /nologo /std:c++latest /utf-8 /EHsc
            "/I${EXPECTED_FAILURE_INCLUDE_DIR}"
            /c "${EXPECTED_FAILURE_SOURCE}"
            "/Fo${EXPECTED_FAILURE_OBJECT}"
    RESULT_VARIABLE expected_failure_exit_code
    OUTPUT_VARIABLE expected_failure_stdout
    ERROR_VARIABLE expected_failure_stderr
)

if(expected_failure_exit_code EQUAL 0)
    message(FATAL_ERROR
        "Файл ${EXPECTED_FAILURE_SOURCE} должен был НЕ компилироваться, "
        "но компилятор собрал его без ошибок.")
endif()

message(STATUS "OK: ${EXPECTED_FAILURE_SOURCE} ожидаемо не компилируется.")
