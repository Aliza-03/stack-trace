#include<stdio.h>
#include <stdint.h>
//#include <stdlib.h>
#include <elfutils/libdwfl.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef STACKTRACE_H
#define STACKTRACE_H

// header file for stack tracer/ Includes basic functionality of traceing the stack and printing function name callbacks

#ifdef __cplusplus
extern "C" {
#endif

void walk_stack(const char* p);


#ifdef __cplusplus
}
#endif

#endif /* STACKTRACE_H */
