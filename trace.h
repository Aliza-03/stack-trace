#include<stdio.h>
#include <stdint.h>
//#include <stdlib.h>
#include <elfutils/libdwfl.h>
#include <sys/types.h>
#include <unistd.h>

#ifndef TRACE_H
#define TRACE_H

// header file for stack tracer/ Includes basic functionality of traceing the stack and printing function name callbacks

#ifdef __cplusplus
extern "C" {
#endif

int init_dwarf(void);
void walk_stack(const char* p);


#ifdef __cplusplus
}
#endif

#endif /* TRACE_H */
