#ifndef ADVENT_OF_CODE_LOG_H
#define ADVENT_OF_CODE_LOG_H

#define LOG_DEBUG 4
#define LOG_INFO 3
#define LOG_WARN 2
#define LOG_ERR 1
#define LOG_OFF 0

#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#if LOG_LEVEL >= LOG_ERR
#define SUCCESS(tag, fmt) printf("%s(%s:%d %s)/S [%s]: " fmt "%s\n", "\033[1;32m", __FILENAME__, __LINE__, __func__, tag, "\033[0m")
#define SUCCESS_(tag, fmt, ...) printf("%s(%s:%d %s)/S [%s]: " fmt "%s\n", "\033[1;32m", __FILENAME__, __LINE__, __func__, tag, __VA_ARGS__, "\033[0m")
#define ERR(tag, fmt) printf("%s(%s:%d %s)/E [%s]: " fmt "%s\n", "\033[1;31m", __FILENAME__, __LINE__, __func__, tag, "\033[0m")
#define ERR_(tag, fmt, ...) printf("%s(%s:%d %s)/E [%s]: " fmt "%s\n", "\033[1;31m", __FILENAME__, __LINE__, __func__, tag, __VA_ARGS__, "\033[0m")
#else
#define SUCCESS(tag, fmt)
#define SUCCESS_(tag, fmt, ...)
#define ERR(tag, fmt)
#define ERR_(tag, fmt, ...)
#endif

#if LOG_LEVEL >= LOG_WARN
#define WARN(tag, fmt) printf("%s(%s:%d %s)/W [%s]: " fmt "%s\n", "\033[1;33m", __FILENAME__, __LINE__, __func__, tag, "\033[0m")
#define WARN_(tag, fmt, ...) printf("%s(%s:%d %s)/W [%s]: " fmt "%s\n", "\033[1;33m", __FILENAME__, __LINE__, __func__, tag, __VA_ARGS__, "\033[0m")
#else
#define WARN(tag, fmt)
#define WARN_(tag, fmt, ...)
#endif

#if LOG_LEVEL >= LOG_INFO
#define LOG(tag, fmt) printf("%s(%s:%d %s)/I [%s]: " fmt "%s\n", "\033[0;34m", __FILENAME__, __LINE__, __func__, tag, "\033[0m")
#define LOG_(tag, fmt, ...) printf("%s(%s:%d %s)/I [%s]: " fmt "%s\n", "\033[0;34m", __FILENAME__, __LINE__, __func__, tag, __VA_ARGS__, "\033[0m")
#else
#define LOG(tag, fmt)
#define LOG_(tag, fmt, ...)
#endif

#if LOG_LEVEL >= LOG_DEBUG
#define DEBUG(tag, fmt) printf("%s(%s:%d %s)/D [%s]: " fmt "%s\n", "\033[1;37m", __FILENAME__, __LINE__, __func__, tag, "\033[0m")
#define DEBUG_(tag, fmt, ...) printf("%s(%s:%d %s)/D [%s]: " fmt "%s\n", "\033[1;37m", __FILENAME__, __LINE__, __func__, tag, __VA_ARGS__, "\033[0m")
#else
#define DEBUG(tag, fmt)
#define DEBUG_(tag, fmt, ...)
#endif

#endif
