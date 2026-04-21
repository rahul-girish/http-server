#ifndef CONTAINER_OF_H
#define CONTAINER_OF_H

#include <stddef.h>

/*
 *  container_of(ptr, type, member)
 *  Get the pointer to the container of ptr.
 *  type is the type of the container, member is the member of the container.
 *  Not const safe.
 */

#define container_of(ptr, type, member)                                        \
    ({                                                                         \
        const typeof(((type *)0)->member) *__mptr = (ptr);                     \
        _Static_assert(__builtin_types_compatible_p(                           \
                           typeof(*(ptr)), typeof(((type *)0)->member)) ||     \
                           __builtin_types_compatible_p(typeof(*(ptr)), void), \
                       "container_of: pointer type mismatch");                 \
        (type *)((char *)__mptr - offsetof(type, member));                     \
    })

/*
 *  container_of_const(ptr, type, member)
 *  Same as container_of(), but const safe.
 */
#define container_of_const(ptr, type, member)                                  \
    _Generic((ptr),                                                            \
        const typeof(*(ptr)) *: (const type *)container_of(ptr, type, member), \
        default: (type *)container_of(ptr, type, member))
#endif