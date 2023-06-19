
cmake_minimum_required(VERSION 3.0)

if(SET_CROSS_COMPILE)
  #don't check compiler!
  # 禁止Cmake检查对编译器选项进行检查，因为交叉编译的工程在本机上没有运行环境
  set(CMAKE_C_COMPILER_WORKS 1)  
  set(CMAKE_CXX_COMPILER_WORKS 1)
endif()

# 给出工程名和描述
project(LITTLE_SSP)

# CMake具有许多内置的构建配置，可用于编译工程。 这些配置指定了代码优化的级别，以及调试信息是否包含在二进制文件中。
# 这些优化级别，主要有：
# Release —— 不可以打断点调试，程序开发完成后发行使用的版本，占的体积小。 它对代码做了优化，因此速度会非常快，
# 在编译器中使用命令： -O3 -DNDEBUG 可选择此版本。
# Debug ——调试的版本，体积大。
# 在编译器中使用命令： -g 可选择此版本。
# MinSizeRel—— 最小体积版本
# 在编译器中使用命令：-Os -DNDEBUG可选择此版本。
# RelWithDebInfo—— 既优化又能调试。
# 在编译器中使用命令：-O2 -g -DNDEBUG可选择此版本
# 执行cmake命令时刻使用 “cmake .. -DCMAKE_BUILD_TYPE=Release”
set(CMAKE_ASM_FLAGS_DEBUG "-g3 -ffunction-sections -fdata-sections -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast")
set(CMAKE_C_FLAGS_DEBUG "-g3 -ffunction-sections -fdata-sections -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast")
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -ffunction-sections -fdata-sections -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast")
set(CMAKE_ASM_FLAGS_RELEASE "-O3 -ffunction-sections -fdata-sections -DNDEBUG -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast")
set(CMAKE_C_FLAGS_RELEASE "-O3 -ffunction-sections -fdata-sections -DNDEBUG -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -ffunction-sections -fdata-sections -DNDEBUG -Wno-pointer-to-int-cast -Wno-int-to-pointer-cast")

# 去掉不用的section，减少程序的大小
link_libraries("-Wl,--gc-sections")

###############compile lib###############
set(LIB_NAME SSP_LIB)
include(cmake/rules.cmake)
LIST(APPEND SRC_LIST ${SRC_FILES})

# 现代Cmake一般建议将源文件直接在add_xxx中声明
# 添加一个目标库，首先包含所有需要的源文件
add_library(${LIB_NAME} ${SRC_LIST})

# 目标库所有需要包含的头文件，以PUBLIC形式
# PUBLIC 会使得这些文件在编译该库和其他目标编译链接到该库时都被调用
LIST(APPEND HEADER_LIST ${HEADER_FILES})
target_include_directories(${LIB_NAME} PUBLIC ${HEADER_LIST})

#目标库需要链接的库，使用PUBLIC，如果有其他文件链接目标库，则该库也被链进去
target_link_libraries(${LIB_NAME} PUBLIC -lm)

# 添加的宏后续可以通过命令行控制

if(SET_DEBUG)
  set_target_properties(${LIB_NAME} PROPERTIES COMPILE_FLAGS -g)
endif()
set_target_properties(${LIB_NAME} PROPERTIES OUTPUT_NAME "ssp_lib")
# 设置目标库被链接时使用静态链接
SET(CMAKE_EXE_LINKER_FLAGS "-static")


###############compile executable###############
# 指定生成可执行文件的源文件
add_executable(example ${CMAKE_CURRENT_LIST_DIR}/example.c)
target_link_libraries(example PUBLIC ${LIB_NAME})
target_include_directories(example PUBLIC
  ${CMAKE_CURRENT_LIST_DIR}/include
  ${CMAKE_CURRENT_LIST_DIR}/mini_log
  ${CMAKE_CURRENT_LIST_DIR}/kernels
)
if(SET_DEBUG)
  set_target_properties(example PROPERTIES COMPILE_FLAGS -g)
endif()
