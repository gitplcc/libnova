if(LN_HAVE_IPO_SUPPORT)
    set_directory_properties(
        PROPERTIES
            INTERPROCEDURAL_OPTIMIZATION TRUE
    )
endif(LN_HAVE_IPO_SUPPORT)

add_subdirectory(elp)

set(HEADER_PATH libnova)

set(LIBRARY_PUBLIC_HEADERS
    ${HEADER_PATH}/julian_day.h
    ${HEADER_PATH}/dynamical_time.h
    ${HEADER_PATH}/sidereal_time.h
    ${HEADER_PATH}/transform.h
    ${HEADER_PATH}/nutation.h
    ${HEADER_PATH}/libnova.h
    ${HEADER_PATH}/aberration.h
    ${HEADER_PATH}/apparent_position.h
    ${HEADER_PATH}/solar.h
    ${HEADER_PATH}/precession.h
    ${HEADER_PATH}/proper_motion.h
    ${HEADER_PATH}/mercury.h
    ${HEADER_PATH}/venus.h
    ${HEADER_PATH}/earth.h
    ${HEADER_PATH}/mars.h
    ${HEADER_PATH}/jupiter.h
    ${HEADER_PATH}/saturn.h
    ${HEADER_PATH}/uranus.h
    ${HEADER_PATH}/neptune.h
    ${HEADER_PATH}/pluto.h
    ${HEADER_PATH}/vsop87.h
    ${HEADER_PATH}/lunar.h
    ${HEADER_PATH}/elliptic_motion.h
    ${HEADER_PATH}/asteroid.h
    ${HEADER_PATH}/comet.h
    ${HEADER_PATH}/parabolic_motion.h
    ${HEADER_PATH}/refraction.h
    ${HEADER_PATH}/rise_set.h
    ${HEADER_PATH}/angular_separation.h
    ${HEADER_PATH}/ln_types.h
    ${HEADER_PATH}/utility.h
    ${HEADER_PATH}/hyperbolic_motion.h
    ${HEADER_PATH}/parallax.h
    ${HEADER_PATH}/airmass.h
    ${HEADER_PATH}/heliocentric_time.h
    ${HEADER_PATH}/constellation.h
)

add_library(nova
    ${LIBRARY_PUBLIC_HEADERS}
    julian_day.c
    dynamical_time.c
    sidereal_time.c
    transform.c
    nutation.c
    aberration.c
    apparent_position.c
    solar.c
    precession.c
    proper_motion.c
    mercury.c
    venus.c
    earth.c
    mars.c
    jupiter.c
    saturn.c
    uranus.c
    neptune.c
    pluto.c
    vsop87.c
    lunar.c
    elliptic_motion.c
    asteroid.c
    comet.c
    parabolic_motion.c
    utility.c
    refraction.c
    rise_set.c
    angular_separation.c
    hyperbolic_motion.c
    parallax.c
    airmass.c
    heliocentric_time.c
    constellation.c
    misc.c
    implementation.c
    $<TARGET_OBJECTS:elp>
)
target_compile_features(nova PRIVATE c_std_11)
target_compile_definitions(nova
    PUBLIC
        $<IF:$<BOOL:${BUILD_SHARED_LIBS}>,LIBNOVA_SHARED,LIBNOVA_STATIC>
    PRIVATE
        $<$<BOOL:${WIN32}>:_USE_MATH_DEFINES>
        $<$<BOOL:${WIN32}>:_SCL_SECURE_NO_WARNINGS>
        $<$<BOOL:${WIN32}>:_CRT_SECURE_NO_DEPRECATE>
        $<$<BOOL:${WIN32}>:__WIN32__>
)
target_include_directories(
    nova
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)
if (BUILD_SHARED_LIBS)
    set_target_properties(nova elp
        PROPERTIES
            C_EXTENSIONS ON
            $<$<BOOL:${CMAKE_C_LINK_PIE_SUPPORTED}>:POSITION_INDEPENDENT_CODE TRUE>
            C_VISIBILITY_PRESET hidden
            VERSION "${libnova_VERSION_MAJOR}.${libnova_VERSION_MINOR}.${libnova_VERSION_PATCH}"
            SOVERSION ${libnova_VERSION_MAJOR}
    )
endif (BUILD_SHARED_LIBS)

add_library(Libnova::nova ALIAS nova)

# install stuff
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    install(
        TARGETS nova
        EXPORT  LibnovaTargets
        RUNTIME
            DESTINATION ${CMAKE_INSTALL_BINDIR}
            COMPONENT   runtime
        LIBRARY
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT   development
        ARCHIVE
            DESTINATION ${CMAKE_INSTALL_LIBDIR}
            COMPONENT   development
        INCLUDES
            DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
            COMPONENT   development
    )

    install(
        FILES       ${LIBRARY_PUBLIC_HEADERS}
        DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${HEADER_PATH}"
        COMPONENT   development
    )
endif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
