#ifndef _SET_H
#define _SET_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct set set_t;

set_t* set_new(int (*compare)(void* data, void* data2));

bool set_contains(set_t* set,void* data);

bool set_add(set_t* set,void* data);

bool set_del(set_t* set,void* data);

#endif // _SET_H


