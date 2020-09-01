add_executable(test test)
target_compile_definitions(test
  PRIVATE $<IF:$<BOOL:BUILD_SHARED_LIBS>,LIBNOVA_SHARED,LIBNOVA_STATIC>
)
target_include_directories(test
  PRIVATE "${PROJECT_SOURCE_DIR}/src"
)
target_link_libraries(test nova $<$<PLATFORM_ID:UNIX>:m>)
