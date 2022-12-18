add_executable(elp82 elp82.c)
target_compile_features(elp82 PUBLIC c_std_11)
set_target_properties(elp82 PROPERTIES C_EXTENSIONS OFF)
target_link_libraries(elp82 $<$<BOOL:${MATH_LIBRARY}>:${MATH_LIBRARY}>)
