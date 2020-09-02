cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(libnova VERSION 0.16.0 LANGUAGES C)

include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckSymbolExists)
include(CheckStructHasMember)
include(CMakeDependentOption)
include(GNUInstallDirs)

find_package(Doxygen)
find_program(CCACHE_PROGRAM ccache)

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

CMAKE_DEPENDENT_OPTION(BUILD_USING_CCACHE
    "Enable using ccache for speed up compilation." OFF
    "CCACHE_PROGRAM" OFF
)
if(CCACHE_PROGRAM)
    set(CMAKE_C_COMPILER_LAUNCHER "${CCACHE_PROGRAM}")
endif()
CMAKE_DEPENDENT_OPTION(BUILD_DOCS
    "Enable building documents." OFF
    "DOXYGEN_FOUND" OFF
)
option(BUILD_EXAMPLES "Enable building examples." OFF)
option(BUILD_SHARED_LIBS "Set to ON to build libnova as a shared library." ON)
option(BUILD_TESTS "Build tests and run." OFF)
option(BUILD_TOOLS "Enable building tools." OFF)

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
file(MAKE_DIRECTORY bin)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# define a macro to setup executable program which are linked with libnova
macro(SETUP_EXECUTABLE EXECUTABLE_NAME)
    add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_NAME}.c)
    target_compile_definitions(${EXECUTABLE_NAME}
        PRIVATE
            $<IF:$<BOOL:BUILD_SHARED_LIBS>,LIBNOVA_SHARED,LIBNOVA_STATIC>
            $<$<BOOL:WIN32>:_USE_MATH_DEFINES>
            $<$<BOOL:WIN32>:_SCL_SECURE_NO_WARNINGS>
            $<$<BOOL:WIN32>:_CRT_SECURE_NO_DEPRECATE>
            $<$<BOOL:WIN32>:__WIN32__>
    )
    target_link_libraries(${EXECUTABLE_NAME} nova $<$<PLATFORM_ID:UNIX>:m>)
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
    install(TARGETS ${EXECUTABLE_NAME} RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
endmacro(SETUP_EXECUTABLE EXECUTABLE_NAME)

# add include directory to all projects
include_directories(
    ${libnova_SOURCE_DIR}/src
    ${libnova_BINARY_DIR}
)

# traverse the sub directories
add_subdirectory(src)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif(BUILD_EXAMPLES)

if(BUILD_TESTS)
    add_subdirectory(lntest)
endif(BUILD_TESTS)

if(BUILD_TOOLS)
    add_subdirectory(tools)
endif(BUILD_TOOLS)

if(BUILD_DOCS)
    add_subdirectory(doc)
endif(BUILD_DOCS)
