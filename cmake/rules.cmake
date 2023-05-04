
message(STATUS "check project source dir = ${PROJECT_SOURCE_DIR}")
# 设置源文件变量
file(GLOB SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/test.c
    ${PROJECT_SOURCE_DIR}/mini_log/m_log.c

)


# 设置include_directories变量
set(HEADER_FILES 
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/mini_log
)

# 指定需要链接的库的路径，也可以在链接库的命令里写全
# link_directories(
#     ${PROJECT_SOURCE_DIR}/xxxxx
#     ${PROJECT_SOURCE_DIR}/xxxxx
# )


