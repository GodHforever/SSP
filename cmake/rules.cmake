
message(STATUS "check project source dir = ${PROJECT_SOURCE_DIR}")
# 设置源文件变量
file(GLOB SRC_FILES
    ${PROJECT_SOURCE_DIR}/src_demo/aec/src/*.c
    # ${PROJECT_SOURCE_DIR}/mini_log/m_log.c
    # ${PROJECT_SOURCE_DIR}/kernels/*.c
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_aec/*.c
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_aec/dios_ssp_aec_tde/*.c
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_agc/*.c
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_doa/*.c
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_gsc/*.c
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_hpf/*.c
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_mvdr/*.c
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_ns/*.c
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_share/*.c
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_vad/*.c




)


# 设置include_directories变量
set(HEADER_FILES 
    # ${PROJECT_SOURCE_DIR}/include
    # ${PROJECT_SOURCE_DIR}/mini_log
    # ${PROJECT_SOURCE_DIR}/kernels
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_aec
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_aec/dios_ssp_aec_tde
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_agc
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_doa
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_gsc
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_hpf
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_mvdr
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_ns
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_share
    # ${PROJECT_SOURCE_DIR}/kernels/dios_ssp_vad
    ${PROJECT_SOURCE_DIR}/src_demo/aec/src/

)
file(GLOB AEC_SRC_LIST
    ${PROJECT_SOURCE_DIR}/src_demo/aec/src/*.c

)

# 指定需要链接的库的路径，也可以在链接库的命令里写全
# link_directories(
#     ${PROJECT_SOURCE_DIR}/xxxxx
#     ${PROJECT_SOURCE_DIR}/xxxxx
# )


