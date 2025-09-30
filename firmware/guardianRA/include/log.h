#ifndef LOG_H
#define LOG_H

#include <tm/tmonitor.h>

/* Simple logging wrappers for T-Monitor */
#define LOG_INFO(fmt, ...)  tm_printf((UB*)"[INFO] " fmt "\n", ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)  tm_printf((UB*)"[WARN] " fmt "\n", ##__VA_ARGS__)
#define LOG_ERR(fmt, ...)   tm_printf((UB*)"[ERR ] " fmt "\n", ##__VA_ARGS__)

#endif /* LOG_H */
