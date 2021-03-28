add_library(elp OBJECT
  elp1.c  elp2.c  elp3.c  elp4.c  elp5.c  elp6.c  elp7.c  elp8.c  elp9.c  elp10.c
  elp11.c elp12.c elp13.c elp14.c elp15.c elp16.c elp17.c elp18.c elp19.c elp20.c
  elp21.c elp22.c elp23.c elp24.c elp25.c elp26.c elp27.c elp28.c elp29.c elp30.c
  elp31.c elp32.c elp33.c elp34.c elp35.c elp36.c
)
target_compile_definitions(elp
    PRIVATE
        $<$<BOOL:${WIN32}>:_USE_MATH_DEFINES>
        $<$<BOOL:${WIN32}>:_SCL_SECURE_NO_WARNINGS>
        $<$<BOOL:${WIN32}>:_CRT_SECURE_NO_DEPRECATE>
        $<$<BOOL:${WIN32}>:__WIN32__>
)
target_include_directories(elp PRIVATE ${PROJECT_SOURCE_DIR}/src)
