cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(LIBNOVA VERSION 0.16.0 LANGUAGES C)

include(CheckIncludeFile)
include(CheckIncludeFiles)
include(CheckSymbolExists)
include(CheckStructHasMember)

CHECK_INCLUDE_FILE(malloc.h HAVE_MALLOC_H)
CHECK_INCLUDE_FILE(alloca.h HAVE_ALLOCA_H)
check_symbol_exists(alloca alloca.h HAVE_ALLOCA)

CHECK_INCLUDE_FILE(sys/time.h HAVE_SYS_TIME_H)
CHECK_INCLUDE_FILES("time.h;sys/time.h" TIME_WITH_SYS_TIME)
CHECK_STRUCT_HAS_MEMBER(tm tm_year sys/time.h TM_IN_SYS_TIME LANGUAGE C)
check_symbol_exists(gettimeofday sys/time.h HAVE_GETTIMEOFDAY)
check_symbol_exists(tzset time.h HAVE_TZSET)

CHECK_INCLUDE_FILE(string.h HAVE_STRING_H)
CHECK_INCLUDE_FILE(strings.h HAVE_STRINGS_H)
check_symbol_exists(strchr string.h HAVE_STRCHR)
check_symbol_exists(strpbrk string.h HAVE_STRPBRK)
check_symbol_exists(strtol stdlib.h HAVE_STRTOL)
check_symbol_exists(strtok_r string.h HAVE_STRTOK_R)

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

CHECK_INCLUDE_FILE(sys/stat.h HAVE_SYS_STAT_H)
CHECK_INCLUDE_FILE(dlfcn.h HAVE_DLFCN_H)

configure_file(config.h.in.cmake config.h)
 
 # define the name of the library
set(LIBRARY_NAME libnova)

option(BUILD_SHARED_LIBRARY "Set to ON to build libnova as a shared library." ON)
if(BUILD_SHARED_LIBRARY)
    set(BUILD_SHARED_LIBS TRUE)
    add_definitions(-DLIBNOVA_SHARED)
else(BUILD_SHARED_LIBRARY)
    set(BUILD_SHARED_LIBS FALSE)
    add_definitions(-DLIBNOVA_STATIC)
endif(BUILD_SHARED_LIBRARY)

# add some definitions
if(WIN32)
    add_definitions(-D_USE_MATH_DEFINES)
    add_definitions(-D_SCL_SECURE_NO_WARNINGS)
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D__WIN32__)
endif(WIN32)

# add more optimization flags to the visual studio release compilation
if(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "/MD /Ox /Ob2 /D NDEBUG")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:SSE2 /GS-")
endif(MSVC)


# set the debug postfix
set(CMAKE_DEBUG_POSTFIX "d" CACHE STRING "add a postfix, usually d on windows")

# set LIB_POSTFIX
if(UNIX AND NOT WIN32 AND NOT APPLE)
  if(CMAKE_SIZEOF_VOID_P MATCHES "8")
      set(LIB_POSTFIX "64" CACHE STRING "suffix for 32/64 dir placement")
      mark_as_advanced(LIB_POSTFIX)
  endif(CMAKE_SIZEOF_VOID_P MATCHES "8")
endif(UNIX AND NOT WIN32 AND NOT APPLE)
if(NOT DEFINED LIB_POSTFIX)
    set(LIB_POSTFIX "")
endif(NOT DEFINED LIB_POSTFIX)


# set and create the EXECUTABLE_OUTPUT_PATH
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)
file(MAKE_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})

# set and create the LIBRARY_OUTPUT_PATH
set(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
file(MAKE_DIRECTORY ${LIBRARY_OUTPUT_PATH})


# define a macro to setup executable program which are linked with libnova
macro(SETUP_EXECUTABLE EXECUTABLE_NAME)
    add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_NAME}.c)
    target_link_libraries(${EXECUTABLE_NAME} libnova)
    set_target_properties(${EXECUTABLE_NAME} PROPERTIES DEBUG_POSTFIX "${CMAKE_DEBUG_POSTFIX}")
    if(MSVC)
        set_target_properties(${EXECUTABLE_NAME} PROPERTIES PREFIX "../")
    endif(MSVC)
    INSTALL(TARGETS ${EXECUTABLE_NAME} RUNTIME DESTINATION bin)
endmacro(SETUP_EXECUTABLE EXECUTABLE_NAME)

# add include directory to all projects
include_directories(
  ${LIBNOVA_SOURCE_DIR}/src
  ${LIBNOVA_BINARY_DIR}
)

# traverse the sub directories
add_subdirectory(src)
add_subdirectory(lntest)
add_subdirectory(examples)
