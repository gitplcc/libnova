add_executable(lntest test)
target_compile_definitions(lntest
    PRIVATE
        $<$<BOOL:${WIN32}>:_USE_MATH_DEFINES>
        $<$<BOOL:${WIN32}>:_SCL_SECURE_NO_WARNINGS>
        $<$<BOOL:${WIN32}>:_CRT_SECURE_NO_DEPRECATE>
        $<$<BOOL:${WIN32}>:__WIN32__>
)
target_link_libraries(
    lntest
    nova
    $<$<BOOL:${MATH_LIBRARY}>:${MATH_LIBRARY}>
)

add_test(
    NAME fulltest
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/bin"
    COMMAND lntest
)

add_executable(test_ticket9 test_ticket9)
target_compile_definitions(test_ticket9
    PRIVATE
        $<$<BOOL:${WIN32}>:_USE_MATH_DEFINES>
        $<$<BOOL:${WIN32}>:_SCL_SECURE_NO_WARNINGS>
        $<$<BOOL:${WIN32}>:_CRT_SECURE_NO_DEPRECATE>
        $<$<BOOL:${WIN32}>:__WIN32__>
)
target_link_libraries(
    test_ticket9
    nova
    unity
    $<$<BOOL:${MATH_LIBRARY}>:${MATH_LIBRARY}>
)

add_test(
    NAME ticket9
    WORKING_DIRECTORY "${PROJECT_BINARY_DIR}/bin"
    COMMAND test_ticket9
)
