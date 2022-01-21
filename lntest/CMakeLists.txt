macro(add_unit_test)
    set(options "")
    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES LIBS)
    cmake_parse_arguments(TEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    add_executable(${TEST_NAME} ${TEST_SOURCES})
    set_target_properties(
        ${TEST_NAME}
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}/bin"
        )
    target_link_libraries(${TEST_NAME} ${TEST_LIBS})
    add_test(
        NAME ${TEST_NAME}
        WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/bin"
        COMMAND ${TEST_NAME}
    )
    list(APPEND TESTNAMES ${TEST_NAME})
endmacro()

add_unit_test(
    NAME test_lntest
    SOURCES test.c
    LIBS nova unity ${MATH_LIBRARY}
)

add_unit_test(
    NAME test_ticket9
    SOURCES test_ticket9.c
    LIBS nova unity ${MATH_LIBRARY}
)

add_unit_test(
    NAME test_utility
    SOURCES test_utility.c
    LIBS nova unity ${MATH_LIBRARY}
)

add_unit_test(
    NAME test_nutation
    SOURCES test_nutation.c
    LIBS nova unity ${MATH_LIBRARY}
)

add_unit_test(
    NAME test_transform
    SOURCES test_transform.c
    LIBS nova unity ${MATH_LIBRARY}
)

add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND}
    DEPENDS ${TESTNAMES}
)
