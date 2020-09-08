add_library(elp OBJECT
  elp1 elp2 elp3 elp4 elp5 elp6 elp7 elp8 elp9 elp10
  elp11 elp12 elp13 elp14 elp15 elp16 elp17 elp18 elp19 elp20
  elp21 elp22 elp23 elp24 elp25 elp26 elp27 elp28 elp29 elp30
  elp31 elp32 elp33 elp34 elp35 elp36
)
target_compile_definitions(elp
    PRIVATE
        $<$<BOOL:${WIN32}>:_USE_MATH_DEFINES>
        $<$<BOOL:${WIN32}>:_SCL_SECURE_NO_WARNINGS>
        $<$<BOOL:${WIN32}>:_CRT_SECURE_NO_DEPRECATE>
        $<$<BOOL:${WIN32}>:__WIN32__>
)
target_include_directories(elp PRIVATE ${PROJECT_SOURCE_DIR}/src)
