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
    target_link_libraries(
        ${TEST_NAME}
        ${TEST_LIBS}
        nova unity
        $<$<BOOL:${MATH_LIBRARY}>:${MATH_LIBRARY}>
    )
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
)

add_unit_test(
    NAME test_heliocentric_time
    SOURCES test_heliocentric_time.c
)

add_unit_test(
    NAME test_julian_day
    SOURCES test_julian_day.c
)

add_unit_test(
    NAME test_dynamical_time
    SOURCES test_dynamical_time.c
)

add_unit_test(
    NAME test_sidereal_time
    SOURCES test_sidereal_time.c
)

add_unit_test(
    NAME test_parallax
    SOURCES test_parallax.c
)

add_unit_test(
    NAME test_rise_set
    SOURCES test_rise_set.c
)

add_unit_test(
    NAME test_airmass
    SOURCES test_airmass.c
)

add_unit_test(
    NAME test_angular_separation
    SOURCES test_angular_separation.c
)

add_unit_test(
    NAME test_nutation
    SOURCES test_nutation.c
)

add_unit_test(
    NAME test_precession
    SOURCES test_precession.c
)

add_unit_test(
    NAME test_aberration
    SOURCES test_aberration.c
)

add_unit_test(
    NAME test_aber_prec_nut
    SOURCES test_aber_prec_nut.c
)

add_unit_test(
    NAME test_apparent_position
    SOURCES test_apparent_position.c
)

add_unit_test(
    NAME test_vsop87
    SOURCES test_vsop87.c
)

add_unit_test(
    NAME test_solar
    SOURCES test_solar.c
)

add_unit_test(
    NAME test_ticket9
    SOURCES test_ticket9.c
)

add_unit_test(
    NAME test_utility
    SOURCES test_utility.c
)

add_unit_test(
    NAME test_transform
    SOURCES test_transform.c
)

add_unit_test(
    NAME test_elliptic_motion
    SOURCES test_elliptic_motion.c
)

add_unit_test(
    NAME test_parabolic_motion
    SOURCES test_parabolic_motion.c
)

add_unit_test(
    NAME test_hyperbolic_motion
    SOURCES test_hyperbolic_motion.c
)

add_unit_test(
    NAME test_lunar
    SOURCES test_lunar.c
)

add_unit_test(
    NAME test_constellation
    SOURCES test_constellation.c
)

add_custom_target(check
    COMMAND ${CMAKE_CTEST_COMMAND}
    DEPENDS ${TESTNAMES}
)
