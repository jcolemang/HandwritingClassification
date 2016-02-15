

#ifndef DYNAMIC_ARRAY_H
    #include "dynamic_array.h"
    #define DYNAMIC_ARRAY_H
#endif


#define DIMENSIONS 2
#define CORE 0
#define BORDER 1
#define NOISE 2


typedef struct DBScanPoint 
{
    int location[DIMENSIONS];
    int classification;
    int num_in_threshold;
    int has_been_visited;
    DynamicArray* points_in_threshold;

} DBScanPoint;

