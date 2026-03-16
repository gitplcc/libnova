add_executable(elp82)
target_sources(elp82 PUBLIC elp82.c)
target_compile_features(elp82 PRIVATE c_std_11)
set_target_properties(elp82
    PROPERTIES
        C_EXTENSIONS OFF
        INTERPROCEDURAL_OPTIMIZATION ${LN_HAVE_IPO_SUPPORT}
)
if(MATH_LIBRARY)
    target_link_libraries(elp82 ${MATH_LIBRARY})
endif()
