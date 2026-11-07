# Passes only when g++ refuses one file, as many times as it should and for the reason it should.
#
# A refusal alone proves nothing: a missing header refuses every tests/fail_*.cpp. So the reason is read
# from the diagnostics. g++ reports a throw reached in constant evaluation as "expression
# '<throw-expression>' is not a constant expression" and echoes the throwing line under it. The match
# stands on that echo, which starts with the throw, so a test's own comment quoting the text cannot pass.
#
#   cmake -DCOMPILER=<g++> -DSOURCE=<tests/fail_x.cpp> -DINCLUDE_DIR=<include> -DOBJECT=<fail_x.o>
#         -DREFUSALS=<count> -DREASON=<text of the throwing line> -P cmake/expect_compile_refusal.cmake

cmake_minimum_required(VERSION 3.25)

foreach(required COMPILER SOURCE INCLUDE_DIR OBJECT REFUSALS REASON)
    if(NOT DEFINED ${required})
        message(FATAL_ERROR "expect_compile_refusal.cmake: -D${required}=... is missing.")
    endif()
endforeach()

get_filename_component(object_directory "${OBJECT}" DIRECTORY)
file(MAKE_DIRECTORY "${object_directory}")

execute_process(
    COMMAND "${COMPILER}" -std=c++23 -fdiagnostics-color=never "-I${INCLUDE_DIR}" -c "${SOURCE}" -o "${OBJECT}"
    RESULT_VARIABLE exit_code
    OUTPUT_QUIET
    ERROR_VARIABLE diagnostics
)

if(exit_code EQUAL 0)
    message(FATAL_ERROR "${SOURCE} compiled, and it must be refused.")
endif()

string(REGEX MATCHALL "error: expression '<throw-expression>' is not a constant expression" found "${diagnostics}")
list(LENGTH found refusal_count)
if(NOT refusal_count EQUAL REFUSALS)
    message(FATAL_ERROR "${SOURCE}: ${refusal_count} refusal(s) by a throw, where ${REFUSALS} are expected:\n${diagnostics}")
endif()

# g++ echoes a line once, so two refusals by one throw show one echo. Every echo must be the expected throw.
string(REGEX MATCHALL "\\| +(throw|_GLIBCXX_THROW_OR_ABORT)[ (][^\n;]*" thrown "${diagnostics}")
if(NOT thrown)
    message(FATAL_ERROR "${SOURCE}: no throwing line in the diagnostics:\n${diagnostics}")
endif()
foreach(line IN LISTS thrown)
    string(FIND "${line}" "${REASON}" position)
    if(position EQUAL -1)
        message(FATAL_ERROR "${SOURCE}: refused by \"${line}\", where the reason must hold \"${REASON}\".")
    endif()
endforeach()

message(STATUS "${SOURCE}: refused ${refusal_count} time(s), for \"${REASON}\".")
