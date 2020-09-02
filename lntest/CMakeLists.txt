add_executable(lntest test)
target_compile_definitions(lntest
    PRIVATE
        $<IF:$<BOOL:BUILD_SHARED_LIBS>,LIBNOVA_SHARED,LIBNOVA_STATIC>
        $<$<BOOL:WIN32>:_USE_MATH_DEFINES>
        $<$<BOOL:WIN32>:_SCL_SECURE_NO_WARNINGS>
        $<$<BOOL:WIN32>:_CRT_SECURE_NO_DEPRECATE>
        $<$<BOOL:WIN32>:__WIN32__>
)
target_include_directories(lntest
  PRIVATE "${PROJECT_SOURCE_DIR}/src"
)
target_link_libraries(lntest nova $<$<BOOL:UNIX>:m>)
