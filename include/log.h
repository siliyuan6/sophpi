#ifndef LOG_H
#define LOG_H

#include <stdio.h>

#ifdef __GNUC__
    #define LOG_COLOR_RESET   "\033[0m"   // 重置颜色
    #define LOG_COLOR_ERR     "\033[31m"  // 红色
    #define LOG_COLOR_WARN    "\033[33m"  // 黄色
    #define LOG_COLOR_INFO    "\033[32m"  // 绿色
    #define LOG_COLOR_DEBUG   "\033[37m"  // 白色（默认）

#else
    // 如果编译器不支持 ANSI 颜色代码，可以禁用颜色
    #define LOG_COLOR_RESET   ""
    #define LOG_COLOR_ERR     ""
    #define LOG_COLOR_WARN    ""
    #define LOG_COLOR_INFO    ""
    #define LOG_COLOR_DEBUG   ""
#endif

// 错误日志，红色
#define log_err(fmt, ...)    \
    do { \
        fprintf(stderr, LOG_COLOR_ERR "[ERROR] %s:%d    " fmt LOG_COLOR_RESET "\n", __func__, __LINE__, ##__VA_ARGS__); \
    } while (0)

// 警告日志，黄色
#define log_warn(fmt, ...)   \
    do { \
        fprintf(stderr, LOG_COLOR_WARN "[WARNING] %s:%d    " fmt LOG_COLOR_RESET "\n", __func__, __LINE__, ##__VA_ARGS__); \
    } while (0)

// 信息日志，绿色
#define log_info(fmt, ...)   \
    do { \
        fprintf(stdout, LOG_COLOR_INFO "[INFO] %s:%d    " fmt LOG_COLOR_RESET "\n", __func__, __LINE__, ##__VA_ARGS__); \
    } while (0)

// 调试日志，白色
#define log_debug(fmt, ...)  \
    do { \
        fprintf(stdout, LOG_COLOR_DEBUG "[DEBUG] %s:%d    " fmt LOG_COLOR_RESET "\n", __func__, __LINE__, ##__VA_ARGS__); \
    } while (0)

#endif // LOG_H
