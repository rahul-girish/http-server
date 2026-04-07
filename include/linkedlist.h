#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include <stddef.h>

/*
 *  struct list
 *  A circular doubly-linked-list implementation.
 *  Generic because it is intrusive.
 */

struct list
{
    struct list *next;
    struct list *prev;
};

/*
 *   LIST_HEAD_INIT(name) {&(name), &(name)}
 *   Make the next and prev pointers of struct list *name point to itself.
 */
#define LIST_HEAD_INIT(name) {&(name), &(name)}

/*
 *  LIST_INIT(ptr)
 *  Set the next and prev pointers of ptr to itself.
 */
#define LIST_INIT(ptr)                                                         \
    do                                                                         \
    {                                                                          \
        (ptr)->next = (ptr);                                                   \
        (ptr)->prev = (ptr);                                                   \
    } while (0)

/*
 *  static inline void __list_add(struct list *node, struct list *prev,
 *                                struct list *next)
 *  Add node between prev and next.
 *  Internal function.
 */
static inline void __list_add(struct list *node, struct list *prev,
                              struct list *next)
{
    node->next = next;
    node->prev = prev;
    next->prev = node;
    prev->next = node;
}

/*
 *   void list_add(struct list *node, struct list *head)
 *   Add node after head.
 *   Implemented in src/linkedlist.c
 */
void list_add(struct list *node, struct list *head);

/*
 *   void list_add_tail(struct list *node, struct list *head)
 *   Add node before head.
 *   Implemented in src/linkedlist.c
 */
void list_add_tail(struct list *node, struct list *head);

/*
 *   static inline void __list_del(struct list *prev, struct list *next)
 *   Link prev and next together, deleting all nodes in between.
 *   Internal function.
 */
static inline void __list_del(struct list *prev, struct list *next)
{
    prev->next = next;
    next->prev = prev;
}

#define container_of(ptr, type, member)                                        \
    ((type *)((char *)(ptr) - offsetof(type, member)))

#define list_entry(ptr, type, member) container_of(ptr, type, member)

#define LIST_HEAD(name) struct list name = LIST_HEAD_INIT(name)

#define list_for_each(cursor, head)                                            \
    for (cursor = (head)->next; cursor != head; cursor = (cursor)->next)

#define list_for_each_prev(cursor, head)                                       \
    for (cursor = (head)->prev; cursor != head; cursor = (cursor)->prev)

#define list_for_each_safe(cursor, temp, head)                                 \
    for (cursor = (head)->next, temp = (cursor)->next; cursor != head;         \
         cursor = temp, temp = (cursor)->next)

#define list_for_each_entry(container, type, member, head)                     \
    for (container = container_of((head)->next, type, member);                 \
         &((container)->member) != head;                                       \
         container = container_of(((container)->member).next, type, member))

#define list_for_each_entry_safe(container, type, member, head)                \
    for (struct list *node = (head)->next, *next = node->next;                 \
         node != (head) &&                                                     \
         ((container) = container_of(node, type, member), 1);                  \
         node = next, next = node->next)

void list_del(struct list *node);

void list_del_init(struct list *node);

int list_empty(struct list *head);

void list_add_before(struct list *node, struct list *pos);

void list_add_after(struct list *node, struct list *pos);

#endif