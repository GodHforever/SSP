# set(TOOLCHAIN_PREFIX riscv64-unknown-linux-gnu-)

# set(TOOLCHAIN_PREFIX riscv64-unknown-elf-)

if(LINUX)
    set(TOOLCHAIN_PREFIX riscv64-unknown-linux-gnu-)
elseif(RTOS)
    set(TOOLCHAIN_PREFIX riscv64-unknown-elf-)
endif()

set(CMAKE_C_COMPILER ${TOOLCHAIN_PREFIX}gcc)
set(CMAKE_ASM_COMPILER ${CMAKE_C_COMPILER})
set(CMAKE_CXX_COMPILER ${TOOLCHAIN_PREFIX}g++)

set(CMAKE_OBJCOPY ${TOOLCHAIN_PREFIX}objcopy)
set(CMAKE_SIZE_UTIL ${TOOLCHAIN_PREFIX}size)

set(CMAKE_FIND_ROOT_PATH ${BINUTILS_PATH})
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

if(C906)
    add_compile_options(-march=rv64gcv0p7_zfh_xtheadc)
    add_compile_options(-mabi=lp64d)
    add_compile_options(-mcmodel=medany)
elseif(E907)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,--export-all-symbols") 
    add_compile_options(-march=rv32imafcpzpsfoperand_xtheade)
    add_compile_options(-mabi=ilp32f)
    add_compile_options(-mtune=e907)
    # 后续可能需要与C906配置一致
    # add_compile_options(-mcmodel=medlow)
endif()   

if(RTOS)
    add_definitions(-DSHL_BUILD_RTOS)
endif()