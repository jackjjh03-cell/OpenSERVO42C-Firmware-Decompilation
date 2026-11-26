set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR arm)
set(CMAKE_TRY_COMPILE_TARGET_TYPE STATIC_LIBRARY)
find_program(ARM_CLANG NAMES clang REQUIRED)
find_program(ARM_LLD NAMES ld.lld REQUIRED)
set(CMAKE_C_COMPILER "${ARM_CLANG}")
set(CMAKE_C_COMPILER_TARGET arm-none-eabi)
# Link the freestanding application directly with LLD.
set(CMAKE_LINKER "${ARM_LLD}")
