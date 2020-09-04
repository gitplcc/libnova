cmake_minimum_required(VERSION 3.9...3.18)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

project(libnova VERSION 0.16.0 LANGUAGES C)

include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckSymbolExists)
include(CheckStructHasMember)
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    include(CMakeDependentOption)
    include(CTest)

    find_package(Doxygen)
    find_program(CCACHE_PROGRAM ccache)

    CMAKE_DEPENDENT_OPTION(BUILD_USING_CCACHE
        "Enable using ccache for speed up compilation." OFF
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

else(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

    set(BUILD_DOCS OFF)
    set(BUILD_EXAMPLES OFF)
    if(NOT DEFINED BUILD_SHARED_LIBS)
        set(BUILD_SHARED_LIBS ON)
    endif(NOT DEFINED BUILD_SHARED_LIBS)
    set(BUILD_TOOLS OFF)
    set(BUILD_REGISTER_PKG OFF)

endif(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)

CHECK_INCLUDE_FILE(malloc.h HAVE_MALLOC_H)
CHECK_INCLUDE_FILE(alloca.h HAVE_ALLOCA_H)
check_symbol_exists(alloca alloca.h HAVE_ALLOCA)

CHECK_INCLUDE_FILE(sys/time.h HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILES("time.h;sys/time.h" TIME_WITH_SYS_TIME)
CHECK_STRUCT_HAS_MEMBER(tm tm_year sys/time.h TM_IN_SYS_TIME LANGUAGE C)
check_symbol_exists(gettimeofday sys/time.h HAVE_GETTIMEOFDAY)
check_symbol_exists(tzset time.h HAVE_TZSET)
check_symbol_exists(gmtime_r time.h HAVE_GMTIME_R)
check_symbol_exists(nanosleep time.h HAVE_NANOSLEEP)
check_symbol_exists(usleep time.h HAVE_USLEEP)
check_symbol_exists(Sleep Windows.h HAVE_SLEEP)

CHECK_INCLUDE_FILE(stdbool.h HAVE_STDBOOL_H)

CHECK_INCLUDE_FILE(string.h HAVE_STRING_H)
CHECK_INCLUDE_FILE(strings.h HAVE_STRINGS_H)
check_symbol_exists(strchr string.h HAVE_STRCHR)
check_symbol_exists(strpbrk string.h HAVE_STRPBRK)
check_symbol_exists(strtol stdlib.h HAVE_STRTOL)
check_symbol_exists(strtok_r string.h HAVE_STRTOK_R)
check_symbol_exists(isblank ctype.h HAVE_ISBLANK)

if(UNIX)
    set(CMAKE_REQUIRED_LIBRARIES m)
endif(UNIX)
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

CHECK_INCLUDE_FILE(sys/stat.h HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE(dlfcn.h HAVE_DLFCN_H)

configure_file(config.h.in.cmake config.h)

# add more global optimization flags to the visual studio release compilation
if(MSVC)
    add_compile_options(
        /arch:AVX
        /GS-
        $<$<CONFIG:Release>:/MD>
        $<$<CONFIG:Release>:/Ox>
        $<$<CONFIG:Release>:/Ob2>
    )
endif(MSVC)

# set the debug postfix
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")

# Allow in-tree target execution for PE/COFF systems
if (NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)
    file(MAKE_DIRECTORY bin)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
endif (NOT DEFINED CMAKE_RUNTIME_OUTPUT_DIRECTORY)

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
    target_link_libraries(${EXECUTABLE_NAME} nova $<$<BOOL:${UNIX}>:m>)
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
    install(TARGETS ${EXECUTABLE_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endmacro(SETUP_EXECUTABLE EXECUTABLE_NAME)

# add include directory for all projects
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# traverse the sub directories
add_subdirectory(src)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(BUILD_EXAMPLES)

if(BUILD_TESTING)
    add_subdirectory(lntest)
endif(BUILD_TESTING)

if(BUILD_TOOLS)
    add_subdirectory(tools)
endif(BUILD_TOOLS)

if(BUILD_DOCS)
    add_subdirectory(doc)
endif(BUILD_DOCS)

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

install(EXPORT LibnovaTargets
    FILE
        LibnovaTargets.cmake
    NAMESPACE
        Libnova::
    DESTINATION
        "${CMAKE_INSTALL_LIBDIR}/cmake/libnova"
)
install(
    FILES
        "${CMAKE_CURRENT_BINARY_DIR}/LibnovaConfig.cmake"
        "${CMAKE_CURRENT_BINARY_DIR}/LibnovaConfigVersion.cmake"
    DESTINATION
        "${CMAKE_INSTALL_LIBDIR}/cmake/libnova"
)

if(BUILD_REGISTER_PKG)
    set(CMAKE_EXPORT_PACKAGE_REGISTRY ON)
    export(PACKAGE Libnova)
endif(BUILD_REGISTER_PKG)
