#ifndef DYNAMIC_ARRAY_H // Include guard start
#define DYNAMIC_ARRAY_H

#include <stdio.h>
#include <stdlib.h>

/* 
 * Type definition for the dynamic array. 
 */
 typedef struct {
    int *data;
    int size;
    int capacity;
 } array_t;