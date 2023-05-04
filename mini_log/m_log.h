/**
 * @file m_log.h
 * @author Liang Shui (Gao Hao)
 * @brief This module is a log system referencing https://github.com/rxi
 * @date 2023-02-16
 * 
 *  Copyright (C) 2022 Gao Hao. All rights reserved.
 * 
 */

#ifndef MINI_LOG_H
#define MINI_LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <stdbool.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// #define SHOW_TIME_TAG_ 如果日志需要开启时间戳
#define MAX_CALLBACKS 32
#define LOG_USE_COLOR

typedef struct {
  va_list ap;
  const char *fmt;
  const char *file;

#ifdef SHOW_TIME_TAG_
  struct tm *time;
#endif

  void *udata;
  int line;
  int level;
} log_event;

// 该指针用于用户自定义日志回调函数，可以对日志进行自定义的输出
typedef void (*log_LogFn)(log_event *ev);

// 该指针用于用户自定义的锁函数，针对多线程使用
typedef void (*log_LockFn)(bool lock, void *udata);

typedef struct {
  log_LogFn fn;
  void *udata;
  int level;
} callback;

typedef struct {
  void *udata;
  log_LockFn lock;
  int level;
  bool quiet;
  callback callbacks[MAX_CALLBACKS];
} log_manager_t;

/**
 * @brief 枚举变量指示了日志等级，如果设置了LOG_CLOSE则所有日志均不会显示，详见log_set_level
 * 
 */
enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL, LOG_CLOSE};

#define log_trace(...) log_log(LOG_TRACE, __FILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, __FILE__, __LINE__, __VA_ARGS__)
#define log_info(...)  log_log(LOG_INFO,  __FILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN,  __FILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, __FILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, __FILE__, __LINE__, __VA_ARGS__)

/**
 * @brief 以string形式返回当前日志的等级
 * 
 * @param level 日志等级
 * @return const char* 
 */
const char* log_level_string(int level);

/**
 * @brief 用于多线程中启动锁
 * 
 * @param fn 锁回调函数
 * @param udata 需要加锁的数据流
 */
void log_set_lock(log_LockFn fn, void *udata);

/**
 * @brief 用于设置日志等级
 * 
 * @param level 日志等级从低到高，所有低于当前日志等级的日志均不会被输出。
 */
void log_set_level(int level);

/**
 * @brief 设置quiet后，日志将不会输出到stderr，但仍会继续输出到其他callback中。
 * 
 * @param enable
 */
void log_set_quiet(bool enable);

/**
 * @brief 添加输出日志的回调函数
 * 
 * @param fn 函数指针
 * @param udata 数据流
 * @param level 日志等级
 * @return int 0 成功，-1 失败
 */
int log_add_callback(log_LogFn fn, void *udata, int level);

/**
 * @brief 添加一个文件流，将日志输出到文件中
 * 
 * @param fp 文件流
 * @param level 日志等级
 * @return int 0 成功，-1 失败
 */
int log_add_fp(FILE *fp, int level);

/**
 * @brief 日志打印函数
 * 
 * @param level 日志等级
 * @param file 当前文件名
 * @param line 源代码行号
 * @param fmt 格式化字符串
 * @param ... 
 */
void log_log(int level, const char *file, int line, const char *fmt, ...);


#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus

#endif // MINI_LOG_H