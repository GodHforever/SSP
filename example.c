/**
 * @file example.c
 * @author Liang Shui (Gao Hao)
 * @brief 
 * @date 2023-02-10
 * 
 * Copyright (C) 2022 Gao Hao. All rights reserved.
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "mini_log/m_log.h"


int main(int argc, char **argv)
{
    printf("pro ok\n");
    log_trace("Hello %s", "world");
    return 0;
}