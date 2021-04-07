#ifndef LOG_H
#define LOG_H

#include <iostream>

#define MATTENGINE_LOG(message, ...)                                           \
	printf(message, __VA_ARGS__);                                              \
	puts("")
#define MATTENGINE_ASSERT(condition, message, ...)                             \
	if (!(condition)) {                                                        \
		MATTENGINE_LOG(message, __VA_ARGS__);                                  \
		exit(1);                                                               \
	}

#endif