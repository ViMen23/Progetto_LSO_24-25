#ifndef LIST_H
#define LIST_H

#include <stddef.h>

#define DEFINE_LIST_FUNCTIONS(Typename, Type)\
    typedef struct Typename##Node {\
        Type data;\
        struct Typename##Node* next;\
    } Typename##Node;\
    \
    typedef struct Typename##List {\
        Typename##Node *head;\
        size_t size;\
    } Typename##List;\

#endif