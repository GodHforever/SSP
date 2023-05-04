/**
 * @file m_log.c
 * @author Liang Shui (Gao Hao)
 * @brief This module is a log system referencing https://github.com/rxi
 * @date 2023-02-17
 * 
 * Copyright (C) 2022 Gao Hao. All rights reserved.
 * 
 */

#include "m_log.h"


log_manager_t mlog = {
  .level = LOG_CLOSE,
};

static const char *level_strings[] = {
  "TRACE", "DEBUG", "INFO", "WARN", "ERROR", "FATAL", "CLOSE"
};

#ifdef LOG_USE_COLOR
static const char *level_colors[] = {
  "\x1b[94m", "\x1b[36m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};
#endif


static void stdout_callback(log_event *ev) {
  char buf[16];

#ifdef SHOW_TIME_TAG_
  buf[strftime(buf, sizeof(buf), "%H:%M:%S", ev->time)] = '\0';
#endif

#ifdef LOG_USE_COLOR
  fprintf(
    ev->udata, "%s %s%-5s\x1b[0m \x1b[90m%s:%d:\x1b[0m ",
    buf, level_colors[ev->level], level_strings[ev->level],
    ev->file, ev->line);
#else
  fprintf(
    ev->udata, "%s %-5s %s:%d: ",
    buf, level_strings[ev->level], ev->file, ev->line);
#endif
  vfprintf(ev->udata, ev->fmt, ev->ap);
  fprintf(ev->udata, "\n");
  fflush(ev->udata);
}


static void file_callback(log_event *ev) {
  char buf[64];

#ifdef SHOW_TIME_TAG_
  buf[strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", ev->time)] = '\0';
#endif

  fprintf(
    ev->udata, "%s %-5s %s:%d: ",
    buf, level_strings[ev->level], ev->file, ev->line);
  vfprintf(ev->udata, ev->fmt, ev->ap);
  fprintf(ev->udata, "\n");
  fflush(ev->udata);
}


static void lock(void)   {
  if (mlog.lock) { mlog.lock(true, mlog.udata); }
}


static void unlock(void) {
  if (mlog.lock) { mlog.lock(false, mlog.udata); }
}


const char* log_level_string(int level) {
  return level_strings[level];
}


void log_set_lock(log_LockFn fn, void *udata) {
  mlog.lock = fn;
  mlog.udata = udata;
}


void log_set_level(int level) {
  mlog.level = level;
}


void log_set_quiet(bool enable) {
  mlog.quiet = enable;
}


int log_add_callback(log_LogFn fn, void *udata, int level) {
  for (int i = 0; i < MAX_CALLBACKS; i++) {
    if (!mlog.callbacks[i].fn) {
      mlog.callbacks[i] = (callback) { fn, udata, level };
      return 0;
    }
  }
  return -1;
}


int log_add_fp(FILE *fp, int level) {
  return log_add_callback(file_callback, fp, level);
}


static void init_event(log_event *ev, void *udata) {

#ifdef SHOW_TIME_TAG_  
  if (!ev->time) {
    time_t t = time(NULL);
    ev->time = localtime(&t);
  }
#endif
  ev->udata = udata;
}


void log_log(int level, const char *file, int line, const char *fmt, ...) {
  log_event ev = {
    .fmt   = fmt,
    .file  = file,
    .line  = line,
    .level = level,
  };

  lock();

  if (!mlog.quiet && level >= mlog.level) {
    init_event(&ev, stderr);
    va_start(ev.ap, fmt);
    stdout_callback(&ev);
    va_end(ev.ap);
  }


// 如果日志需要同输入到文件中可以使用file_callback.
// 如果有其他数据流需求，也可以新注册callback
  for (int i = 0; i < MAX_CALLBACKS && mlog.callbacks[i].fn; i++) {
    callback *cb = &mlog.callbacks[i];
    if (level >= cb->level) {
      init_event(&ev, cb->udata);
      va_start(ev.ap, fmt);
      cb->fn(&ev);
      va_end(ev.ap);
    }
  }

  unlock();
}
