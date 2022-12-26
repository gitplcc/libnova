cmake_minimum_required(VERSION 3.13...3.23)

project(libnova VERSION 0.16.1 LANGUAGES C)

include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckSymbolExists)
include(CheckStructHasMember)
include(CheckCCompilerFlag)
include(CheckIPOSupported)
include(CMakeDependentOption)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CTest)

    # Packaging support
    set(CPACK_PACKAGE_VENDOR "Vendor name")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Celestial Mechanics, Astrometry and Astrodynamics Library")
    set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
    set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
    set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/COPYING")
    set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README")
    include(CPack)

    ###########################################################################
    # Snippet from https://blog.kitware.com/cmake-and-the-default-build-type/
    # Set a default build type if none was specified
    set(DEFAULT_BUILD_TYPE "Release")
    if(EXISTS "${CMAKE_SOURCE_DIR}/.git")
        set(DEFAULT_BUILD_TYPE "Debug")
    endif()

    if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        message(STATUS "Setting build type to '${DEFAULT_BUILD_TYPE}' as none was specified.")
        set(CMAKE_BUILD_TYPE "${DEFAULT_BUILD_TYPE}" CACHE
            STRING "Choose the type of build." FORCE)
        # Set the possible values of build type for cmake-gui
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
          "MinSizeRel" "RelWithDebInfo")
    endif()
    # Snippet from https://blog.kitware.com/cmake-and-the-default-build-type/
    ###########################################################################

    find_package(Doxygen)
    find_program(CCACHE_PROGRAM ccache)
    mark_as_advanced(CCACHE_PROGRAM)

    CMAKE_DEPENDENT_OPTION(BUILD_USING_CCACHE
        "Enable using ccache for speed up compilation." ON
        "CCACHE_PROGRAM" OFF
    )
    if(BUILD_USING_CCACHE)
        set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
    endif(BUILD_USING_CCACHE)

    CMAKE_DEPENDENT_OPTION(BUILD_DOCS
        "Enable building documents." OFF
        "DOXYGEN_FOUND" OFF
    )
    option(BUILD_EXAMPLES "Enable building examples." OFF)
    option(BUILD_SHARED_LIBS "Build libnova as a shared library." ON)
    option(BUILD_TOOLS "Enable building tools." OFF)
    option(BUILD_REGISTER_PKG "Enable registering package in CMake registry." OFF)

    ###########################################################################
    # Snippet from https://cliutils.gitlab.io/modern-cmake
    find_package(Git QUIET)
    if(GIT_FOUND AND EXISTS "${PROJECT_SOURCE_DIR}/.git")
    # Update submodules as needed
        option(GIT_SUBMODULE "Check submodules during build" ON)
        if(GIT_SUBMODULE)
            message(STATUS "Submodule update")
            execute_process(
                COMMAND ${GIT_EXECUTABLE} submodule update --init --recursive
                WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                RESULT_VARIABLE GIT_SUBMOD_RESULT
            )
            if(NOT GIT_SUBMOD_RESULT EQUAL "0")
                message(FATAL_ERROR "git submodule update --init failed with ${GIT_SUBMOD_RESULT}, please checkout submodules")
            endif()
        endif()
    endif()
    # Snippet from https://cliutils.gitlab.io/modern-cmake
    ###########################################################################

else(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    set(BUILD_DOCS OFF)
    set(BUILD_EXAMPLES OFF)
    if(NOT DEFINED BUILD_SHARED_LIBS)
        set(BUILD_SHARED_LIBS ON)
    endif(NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_TOOLS OFF)
    set(BUILD_REGISTER_PKG OFF)

endif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

find_library(MATH_LIBRARY m)
mark_as_advanced(MATH_LIBRARY)

CHECK_INCLUDE_FILE(malloc.h HAVE_MALLOC_H)
CHECK_INCLUDE_FILE(alloca.h HAVE_ALLOCA_H)
check_symbol_exists(alloca alloca.h HAVE_ALLOCA)

CHECK_INCLUDE_FILE(sys/time.h HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILES("time.h;sys/time.h" TIME_WITH_SYS_TIME)
CHECK_INCLUDE_FILE(synchapi.h HAVE_SYNCHAPI_H)
CHECK_STRUCT_HAS_MEMBER(tm tm_year sys/time.h TM_IN_SYS_TIME LANGUAGE C)
check_symbol_exists(_tzset time.h HAVE_TZSET)
check_symbol_exists(clock_gettime time.h HAVE_CLOCK_GETTIME)
check_symbol_exists(timespec_get time.h HAVE_TIMESPEC_GET)
check_symbol_exists(gmtime_r time.h HAVE_GMTIME_R)
check_symbol_exists(thrd_sleep threads.h HAVE_THRD_SLEEP)
check_symbol_exists(nanosleep time.h HAVE_NANOSLEEP)
if(HAVE_SYNCHAPI_H)
    check_symbol_exists(Sleep synchapi.h HAVE_SLEEP)
else(HAVE_SYNCHAPI_H)
    check_symbol_exists(Sleep Windows.h HAVE_SLEEP)
endif(HAVE_SYNCHAPI_H)

CHECK_INCLUDE_FILE(stdbool.h HAVE_STDBOOL_H)

CHECK_INCLUDE_FILE(string.h HAVE_STRING_H)
CHECK_INCLUDE_FILE(strings.h HAVE_STRINGS_H)
check_symbol_exists(strchr string.h HAVE_STRCHR)
check_symbol_exists(strpbrk string.h HAVE_STRPBRK)
check_symbol_exists(strtol stdlib.h HAVE_STRTOL)
check_symbol_exists(strtok_r string.h HAVE_STRTOK_R)
check_symbol_exists(isblank ctype.h HAVE_ISBLANK)

if(MATH_LIBRARY)
    set(CMAKE_REQUIRED_LIBRARIES ${MATH_LIBRARY})
endif(MATH_LIBRARY)
check_symbol_exists(modf math.h HAVE_MODF)
check_symbol_exists(pow math.h HAVE_POW)
check_symbol_exists(sqrt math.h HAVE_SQRT)
check_symbol_exists(cbrt math.h HAVE_CBRT)
check_symbol_exists(round math.h HAVE_ROUND)
check_symbol_exists(cosl math.h HAVE_COSL)
check_symbol_exists(sinl math.h HAVE_SINL)
check_symbol_exists(acosl math.h HAVE_ACOSL)
check_symbol_exists(asinl math.h HAVE_ASINL)
check_symbol_exists(atan2l math.h HAVE_ATAN2L)
check_symbol_exists(nan math.h HAVE_NAN)
check_symbol_exists(fmodl math.h HAVE_FMODL)

check_symbol_exists(setenv stdlib.h HAVE_SETENV)
check_symbol_exists(_putenv_s stdlib.h HAVE_PUTENV_S)

CHECK_INCLUDE_FILE(sys/stat.h HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE(dlfcn.h HAVE_DLFCN_H)

configure_file(config.h.in.cmake config.h)

check_ipo_supported(RESULT LN_HAVE_IPO_SUPPORT OUTPUT LN_IPO_ERRORINFO)
if(NOT LN_HAVE_IPO_SUPPORT)
    message(WARNING "IPO unsupported: ${LN_IPO_ERRORINFO}")
endif(NOT LN_HAVE_IPO_SUPPORT)

# Compiler scpecific flags
check_c_compiler_flag(/GS- HAVE_BUFSECCHK_FLAG)
if(HAVE_BUFSECCHK_FLAG)
    add_compile_options(/GS-)
endif(HAVE_BUFSECCHK_FLAG)

# Compiler specific optimization flags
check_c_compiler_flag(-march=native HAVE_ARCH_FLAG)
if(HAVE_ARCH_FLAG)
    add_compile_options(-march=native)
endif(HAVE_ARCH_FLAG)
check_c_compiler_flag(/arch:AVX HAVE_AVX_FLAG)
check_c_compiler_flag(/arch:AVX2 HAVE_AVX2_FLAG)
if(HAVE_AVX2_FLAG)
    add_compile_options(/arch:AVX2)
elseif(HAVE_AVX2_FLAG)
    add_compile_options(/arch:AVX)
endif()

# set the debug postfix
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")

if (WIN32 OR CYGWIN)
    # Allow in-tree target execution for PE/COFF systems
    if (NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
        set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
    endif (NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
endif (WIN32 OR CYGWIN)

# define a macro to setup executable program which are linked with libnova
macro(SETUP_EXECUTABLE EXECUTABLE_NAME)
    add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_NAME}.c)
    target_compile_definitions(${EXECUTABLE_NAME}
        PRIVATE
            $<$<BOOL:${WIN32}>:_USE_MATH_DEFINES>
            $<$<BOOL:${WIN32}>:_SCL_SECURE_NO_WARNINGS>
            $<$<BOOL:${WIN32}>:_CRT_SECURE_NO_DEPRECATE>
            $<$<BOOL:${WIN32}>:__WIN32__>
    )
    target_compile_features(${EXECUTABLE_NAME} PUBLIC c_std_11)
    set_target_properties(${EXECUTABLE_NAME}
        PROPERTIES
            C_EXTENSIONS ON
            $<$<BOOL:${LN_HAVE_IPO_SUPPORT}>:INTERPROCEDURAL_OPTIMIZATION TRUE>
    )
    target_link_libraries(
        ${EXECUTABLE_NAME}
        nova
        $<$<BOOL:${MATH_LIBRARY}>:${MATH_LIBRARY}>
    )
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
    if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
        install(TARGETS ${EXECUTABLE_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
    endif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
endmacro(SETUP_EXECUTABLE EXECUTABLE_NAME)

# add include directory for all projects
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# traverse the sub directories
add_subdirectory(src)

# Export stuff
write_basic_package_version_file(
    LibnovaConfigVersion.cmake
    VERSION ${libnova_VERSION}
    COMPATIBILITY AnyNewerVersion
)

export(
    EXPORT LibnovaTargets
    FILE "${CMAKE_CURRENT_BINARY_DIR}/LibnovaTargets.cmake"
    NAMESPACE Libnova::
)

configure_file(
    cmake/LibnovaConfig.cmake
    "${CMAKE_CURRENT_BINARY_DIR}/LibnovaConfig.cmake"
    COPYONLY
)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    if(BUILD_EXAMPLES)
        add_subdirectory(examples)
    endif(BUILD_EXAMPLES)

    if(BUILD_TESTING)
        add_subdirectory(extern/Unity EXCLUDE_FROM_ALL)
        target_compile_definitions(unity
            PUBLIC
                UNITY_INCLUDE_DOUBLE
                UNITY_INCLUDE_PRINT_FORMATTED
                UNITY_OUTPUT_COLOR
        )
        add_subdirectory(lntest)
    endif(BUILD_TESTING)

    if(BUILD_TOOLS)
        add_subdirectory(tools)
    endif(BUILD_TOOLS)

    if(BUILD_DOCS)
        add_subdirectory(doc)
    endif(BUILD_DOCS)

    install(EXPORT LibnovaTargets
        FILE
            LibnovaTargets.cmake
        NAMESPACE
            Libnova::
        DESTINATION
            "${CMAKE_INSTALL_LIBDIR}/cmake/libnova"
        COMPONENT
            development
    )

    install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/LibnovaConfig.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/LibnovaConfigVersion.cmake"
        DESTINATION
            "${CMAKE_INSTALL_LIBDIR}/cmake/libnova"
        COMPONENT
            development
    )

    cpack_add_component(
        runtime
        DISPLAY_NAME "Runtime files"
        DESCRIPTION "Files needed at runtime."
        REQUIRED
    )

    cpack_add_component(
        development
        DISPLAY_NAME "Development files"
        DESCRIPTION "Files needed to link against libnova."
        DEPENDS runtime
    )

    if(BUILD_REGISTER_PKG)
        set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
        export(PACKAGE Libnova)
    endif(BUILD_REGISTER_PKG)

endif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
