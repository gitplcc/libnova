add_executable(elp82 elp82.c)
target_include_directories(elp82 PRIVATE "${PROJECT_SOURCE_DIR}/src")
target_link_libraries(elp82 $<$<PLATFORM_ID:UNIX>:m>)
