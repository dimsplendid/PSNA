# simpleNA
simple Photosystem network analysis tool 

Use the Ford Fulkerson algrithm to analysis the photosystem's network.
FMO, 8 sites(nodes) network.
LHCII-MON, 14 sites(nodes) network.
PSI, 96 sites(nodes) network

This tool is for finding the important node or hub of the network by using Ford Fulkerson alg.

test push pull of git

## Introduction
### programs
Using the Object-Oriented Programming in C11
All data structure define in lib/data_struct.h
``` clike=*
typedef struct Object_struct Object;
// use pointer to pointer to escape the copy
int init_Object(Object ** self);
struct Object_struct{
    // members
    ...
    // methods
    int (*method1)(Object * self,...);
    ...
    int (*free)(Object * self);
};
```
usage
``` clike=*
// initailize object
Object * myObject;
init_Object(&myObject);
// usage
myObject->method1(myObject,...);
...
// destruct
myObject->free(myObject);
```
