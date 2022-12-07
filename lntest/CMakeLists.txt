macro(add_unit_test)
    set(options "")
    set(oneValueArgs NAME)
    set(multiValueArgs SOURCES LIBS)
    cmake_parse_arguments(TEST "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    add_executable(${TEST_NAME} ${TEST_SOURCES})
    target_compile_features(${TEST_NAME} PUBLIC c_std_11)
    set_target_properties(
        ${TEST_NAME}
        PROPERTIES
            C_EXTENSIONS OFF
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
    NAME test_julian_day
    SOURCES test_julian_day.c
    LIBS nova unity ${MATH_LIBRARY}
)

add_unit_test(
    NAME test_dynamical_time
    SOURCES test_dynamical_time.c
    LIBS nova unity ${MATH_LIBRARY}
)

add_unit_test(
    NAME test_sidereal_time
    SOURCES test_sidereal_time.c
    LIBS nova unity ${MATH_LIBRARY}
)

add_unit_test(
    NAME test_rise_set
    SOURCES test_rise_set.c
    LIBS nova unity ${MATH_LIBRARY}
)

add_unit_test(
    NAME test_airmass
    SOURCES test_airmass.c
    LIBS nova unity ${MATH_LIBRARY}
)

add_unit_test(
    NAME test_angular_separation
    SOURCES test_angular_separation.c
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

add_unit_test(
    NAME test_constellation
    SOURCES test_constellation.c
    LIBS nova unity ${MATH_LIBRARY}
)

add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND}
    DEPENDS ${TESTNAMES}
)
