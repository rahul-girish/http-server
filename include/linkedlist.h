#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "container_of.h"

#define LIST_POISON1 ((void *)0xdeadbeef)
#define LIST_POISON2 ((void *)0xcafebabe)

/**
 *  struct list
 *  A circular doubly-linked-list implementation.
 *  Generic because it is intrusive.
 */

struct list
{
    struct list *next;
    struct list *prev;
};

/**
 *   LIST_HEAD(name)
 *   Define a new struct list name and initialize it.
 */
#define LIST_HEAD(name) struct list name = LIST_HEAD_INIT(name)

/**
 *  LIST_HEAD_INIT(name)
 *  Make the next and prev pointers of struct list *name point to itself.
 */
#define LIST_HEAD_INIT(name) {&(name), &(name)}

/**
 *  LIST_INIT(ptr)
 *  Set the next and prev pointers of ptr to itself.
 */
#define LIST_INIT(ptr)                                                         \
    do                                                                         \
    {                                                                          \
        (ptr)->next = (ptr);                                                   \
        (ptr)->prev = (ptr);                                                   \
    } while (0)

/**
 *  __list_add(node, prev, next)
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

/**
 *  list_add(node, head)
 *  Add node after head.
 */
static inline void list_add(struct list *node, struct list *head)
{
    __list_add(node, head, head->next);
}

/**
 *  list_add_tail(node, head)
 *  Add node before head.
 */
static inline void list_add_tail(struct list *node, struct list *head)
{
    __list_add(node, head->prev, head);
}

/**
 *  __list_del(prev, next)
 *  Link prev and next together, deleting all nodes in between.
 *  Internal function.
 */
static inline void __list_del(struct list *prev, struct list *next)
{
    prev->next = next;
    next->prev = prev;
}

/**
 *  list_del(node)
 *  Delete node from the list
 */
static inline void list_del(struct list *node)
{
    __list_del(node->prev, node->next);
    node->next = LIST_POISON1;
    node->prev = LIST_POISON2;
}

/**
 *   list_del_init(node)
 *   Delete node from the list and initialize it.
 */
static inline void list_del_init(struct list *node)
{
    __list_del(node->prev, node->next);
    LIST_INIT(node);
}

/**
 *  list_replace(node1, node2)
 *  Replace node1 with node2.
 */
static inline void list_replace(struct list *node1, struct list *node2)
{
    node2->next = node1->next;
    node2->prev = node1->prev;
    node2->prev->next = node2;
    node2->next->prev = node2;
}

/**
 *  list_replace_init(node1, node2)
 *  Replace node1 with node2 and reinitialize node1.
 */
static inline void list_replace_init(struct list *node1, struct list *node2)
{
    list_replace(node1, node2);
    LIST_INIT(node1);
}

/**
 *  list_swap(node1, node2)
 *  Swap node1 and node 2.
 */
static inline void list_swap(struct list *node1, struct list *node2)
{
    struct list *pos = node2->prev;

    list_del(node2);
    list_replace(node1, node2);
    if (pos == node1)
        pos = node2;
    list_add(node1, pos);
}

/**
 *  list_move(node, head)
 *  Move node to the position after head.
 */
static inline void list_move(struct list *node, struct list *head)
{
    __list_del(node->prev, node->next);
    list_add(node, head);
}

/**
 *  list_move_tail(node, head)
 *  Move node to the position before head.
 */
static inline void list_move_tail(struct list *node, struct list *head)
{
    __list_del(node->prev, node->next);
    list_add_tail(node, head);
}

/**
 *  list_bulk_move_tail(head, first, last)
 *  Move the subsection of a list beginning at first and ending at last between
 *  head and head->prev
 */
static inline void list_bulk_move_tail(struct list *head, struct list *first,
                                       struct list *last)
{
    first->prev->next = last->next;
    last->next->prev = first->prev;

    head->prev->next = first;
    first->prev = head->prev;

    head->prev = last;
    last->next = head;
}

/**
 *  list_is_first(node, head)
 *  Checks if node is the first entry in the list head.
 */
static inline int list_is_first(const struct list *node,
                                const struct list *head)
{
    return node->prev == head;
}

/**
 *  list_is_last(node, head)
 *  Checks if node is the last entry in the list head.
 */
static inline int list_is_last(const struct list *node, const struct list *head)
{
    return node->next == head;
}

/**
 *  list_is_head(node, head)
 *  Checks if node is head.
 */
static inline int list_is_head(const struct list *node, const struct list *head)
{
    return node == head;
}

/**
 *  list_empty(head)
 *  Checks if head is empty.
 */
static inline int list_empty(const struct list *head)
{
    return head->next == head;
}

/**
 *  list_rotate_left(head)
 *  Rotate the list to the left.
 */
static inline void list_rotate_left(struct list *head)
{
    struct list *first;

    if (!list_empty(head))
    {
        first = head->next;
        list_move_tail(first, head);
    }
}

/**
 *  list_rotate_to_front(node, head)
 *  Rotates the list so that node comes after head.
 */
static inline void list_rotate_to_front(struct list *node, struct list *head)
{
    // Simply delete head and add it before node.
    list_move_tail(head, node);
}

/**
 *  list_is_singular(node)
 *  Checks if head has only a single node.
 */
static inline int list_is_singular(const struct list *head)
{
    return !list_empty(head) && (head->next == head->prev);
}

/**
 *  __list_cut_position(other_head, head, node)
 *  Cuts the list head at node, the first part, including node, ends up in
 * new_head. The second part remains in head. Internal function.
 */
static inline void __list_cut_position(struct list *other_head,
                                       struct list *head, struct list *node)
{
    struct list *other_first = node->next;
    other_head->next = head->next;
    other_head->next->prev = other_head;
    other_head->prev = node;
    node->next = other_head;
    head->next = other_first;
    other_first->prev = head;
}

/**
 *  list_cut_position(other_head, head, node)
 *  Cuts a list in two. The first part, including node, goes to other_head and
 *  the second part goes to head. node is a node on head. other_head should be
 *  an empty list or a list you do not care about.
 */
static inline void list_cut_position(struct list *other_head, struct list *head,
                                     struct list *node)
{
    if (list_empty(head))
        return;
    if (list_is_singular(head) && !list_is_head(node, head) &&
        (node != head->next))
        return;
    if (list_is_head(node, head))
        LIST_INIT(other_head);
    else
        __list_cut_position(other_head, head, node);
}

/**
 *  list_cut_before(other_head, head, node)
 *  Cuts a list in two. The first part, excluding node, goes to other_head and
 *  the second part goes to head. node is a node on head. other_head should be
 *  an empty list or a list you do not care about.
 */
static inline void list_cut_before(struct list *other_head, struct list *head,
                                   struct list *node)
{
    if (head->next == node)
    {
        LIST_INIT(other_head);
        return;
    }
    other_head->next = head->next;
    other_head->next->prev = other_head;
    other_head->prev = node->prev;
    other_head->prev->next = other_head;
    head->next = node;
    node->prev = head;
}

/**
 *  __list_splice(head, prev, next)
 *  Join two lists together.
 *  Internal function.
 */
static inline void __list_splice(const struct list *other_head,
                                 struct list *prev, struct list *next)
{
    struct list *first = other_head->next;
    struct list *last = other_head->prev;

    first->prev = prev;
    prev->next = first;

    last->next = next;
    next->prev = last;
}

/**
 *  list_splice(other_head, head)
 *  Join two lists together.
 *  The entries in other_head are inserted at the front of head.
 *  This is meant for stacks.
 */
static inline void list_splice(const struct list *other_head, struct list *head)
{
    if (!list_empty(other_head))
        __list_splice(other_head, head, head->next);
}

/**
 *  list_splice_tail(other_head, head)
 *  Join two lists together.
 *  The entries in other_head are inserted at the tail of head.
 *  This is meant for queues.
 */
static inline void list_splice_tail(struct list *other_head, struct list *head)
{
    if (!list_empty(other_head))
        __list_splice(other_head, head->prev, head);
}

/**
 *  list_splice_init(other_head, head)
 *  Join two lists together. Reinitialize the empty list.
 *  The entries in other_head are inserted at the front of head.
 *  This is meant for stacks.
 */
static inline void list_splice_init(struct list *other_head, struct list *head)
{
    if (!list_empty(other_head))
    {
        __list_splice(other_head, head, head->next);
        LIST_INIT(other_head);
    }
}

/**
 *  list_splice_tail_init(other_head, head)
 *  Join two lists together. Reinitialize the empty list.
 *  The entries in other_head are inserted at the tail of head.
 *  This is meant for queues.
 */
static inline void list_splice_tail_init(struct list *other_head,
                                         struct list *head)
{
    if (!list_empty(other_head))
    {
        __list_splice(other_head, head->prev, head);
        LIST_INIT(other_head);
    }
}

/**
 *  list_entry(ptr, type, member)
 *  Get the entry for this node.
 *  type is the type of the container, member is the member of the container.
 */
#define list_entry(ptr, type, member) container_of(ptr, type, member)

/**
 *  list_first_entry(ptr, type, member)
 *  Get the first entry of a list.
 *  The list must not be empty.
 */
#define list_first_entry(ptr, type, member)                                    \
    container_of((ptr)->next, type, member)

/**
 *  list_last_entry(ptr, type, member)
 *  Get the last entry of a list.
 *  The list must not be empty.
 */
#define list_last_entry(ptr, type, member)                                     \
    container_of((ptr)->prev, type, member)

/**
 *  list_first_entry_or_null(ptr, type, member)
 *  Get the first entry of a list.
 *  Returns NULL if the list is empty.
 */
#define list_first_entry_or_null(ptr, type, member)                            \
    list_empty(ptr) ? NULL : list_first_entry(ptr, type, member)

/**
 *  list_last_entry_or_null(ptr, type, member)
 *  Get the last entry of a list.
 *  Returns NULL if the list is empty.
 */
#define list_last_entry_or_null(ptr, type, member)                             \
    list_empty(ptr) ? NULL : list_last_entry(ptr, type, member)

/**
 *  list_next_entry(pos, member)
 *  Get the next entry of the list.
 */
#define list_next_entry(pos, member)                                           \
    list_entry((pos)->member.next, typeof(*(pos)), member)

/**
 *  list_next_entry_circular(pos, head, member)
 *  Get the next entry of the list, including when pos is the last entry.
 */
#define list_next_entry_circular(pos, head, member)                            \
    (list_is_last(&(pos)->member, head)                                        \
         ? list_first_entry(head, typeof(*(pos)), member)                      \
         : list_next_entry(pos, member))

/**
 *  list_prev_entry(pos, member)
 *  Get the previous entry of the list.
 */
#define list_prev_entry(pos, member)                                           \
    list_entry((pos)->member.prev, typeof(*(pos)), member)

/**
 *  list_prev_entry_circular(pos, head, member)
 *  Get the previous entry of the list, including when pos is the first entry.
 */
#define list_prev_entry_circular(pos, head, member)                            \
    (list_is_first(&(pos)->member, head)                                       \
         ? list_last_entry(head, typeof(*(pos)), member)                       \
         : list_prev_entry(pos, member))

/**
 *  list_for_each(cursor, head)
 *  Iterate over a list using cursor.
 */
#define list_for_each(cursor, head)                                            \
    for (cursor = (head)->next; !list_is_head(cursor, (head));                 \
         cursor = (cursor)->next)

/**
 *  list_for_each_continue(cursor, head)
 *  Iterate over a list starting from cursor->next.
 */
#define list_for_each_continue(cursor, head)                                   \
    for (cursor = (cursor)->next; !list_is_head(cursor, (head));               \
         cursor = (cursor)->next)

/**
 *  list_for_each_prev(cursor, head)
 *  Iterate over a list backwards
 */
#define list_for_each_prev(cursor, head)                                       \
    for (cursor = (head)->prev; !list_is_head(cursor, (head));                 \
         cursor = (cursor)->prev)

/**
 *  list_for_each_safe(cursor, temp, head)
 *  Iterate over a list with deletion safety.
 */
#define list_for_each_safe(cursor, temp, head)                                 \
    for (cursor = (head)->next, temp = (cursor)->next;                         \
         !list_is_head(cursor, (head)); cursor = temp, temp = cursor->next)

/**
 *  list_for_each_prev_safe(cursor, temp, head)
 *  Iterate over a list backwards with deletion safety.
 */
#define list_for_each_prev_safe(cursor, temp, head)                            \
    for (cursor = (head)->prev, temp = (cursor)->prev;                         \
         !list_is_head(cursor, (head)); cursor = temp, temp = (cursor)->prev)

/**
 *  Count the number of nodes in a list.
 */
static inline size_t list_count_nodes(struct list *head)
{
    struct list *cursor;
    size_t count = 0;

    list_for_each(cursor, head) count++;

    return count;
}

/**
 *  list_entry_is_head(node, head, member)
 *  Test if an entry points to the head of the list.
 */
#define list_entry_is_head(node, head, member)                                 \
    list_is_head(&(node)->member, (head))

/**
 *  list_for_each_entry(cursor, head, member)
 *  Iterate over the entries in a list.
 */
#define list_for_each_entry(cursor, head, member)                              \
    for (cursor = list_first_entry(head, typeof(*cursor), member);             \
         !list_entry_is_head(cursor, head, member);                            \
         cursor = list_next_entry(cursor, member))

/**
 *  list_for_each_entry_reverse(cursor, head, member)
 *  Iterate backward over the entries in a list.
 */
#define list_for_each_entry_reverse(cursor, head, member)                      \
    for (cursor = list_last_entry(head, typeof(*cursor), member);              \
         !list_entry_is_head(cursor, head, member);                            \
         cursor = list_prev_entry(cursor, member))

/**
 *  list_prepare_entry(cursor, head, member)
 *  Prepare cursor for use as a start point in list_for_each_entry_continue()
 */
#define list_prepare_entry(cursor, head, member)                               \
    ((cursor) ?: list_entry(head, typeof(*cursor), member))

/**
 *  list_for_each_entry_continue(cursor, head, member)
 *  Iterate over a list of entries starting from the next entry from cursor.
 */
#define list_for_each_entry_continue(cursor, head, member)                     \
    for (cursor = list_next_entry(cursor, member);                             \
         !list_entry_is_head(cursor, head, member);                            \
         cursor = list_next_entry(cursor, member))

/**
 *  list_for_each_entry_continue(cursor, head, member)
 *  Iterate over a list of entries backwards starting from the next entry from
 *  cursor.
 */
#define list_for_each_entry_continue_reverse(cursor, head, member)             \
    for (cursor = list_prev_entry(cursor, member);                             \
         !list_entry_is_head(cursor, head, member);                            \
         cursor = list_prev_entry(cursor, member))

/**
 *  list_for_each_entry_from(cursor, head, member)
 *  Iterate over a list of entries starting from cursor.
 */
#define list_for_each_entry_from(cursor, head, member)                         \
    for (; !list_entry_is_head(cursor, head, member);                          \
         cursor = list_next_entry(cursor, member))

/**
 *  list_for_each_entry_from_reverse(cursor, head, member)
 *  Iterate over a list of entries backward starting from cursor.
 */
#define list_for_each_entry_from_reverse(cursor, head, member)                 \
    for (; !list_entry_is_head(cursor, head, member);                          \
         cursor = list_prev_entry(cursor, member))

/**
 *  list_for_each_entry_safe(cursor, temp, head, member)
 *  Iterate over a list of entries, safe from entry deletion.
 */
#define list_for_each_entry_safe(cursor, temp, head, member)                   \
    for (cursor = list_first_entry(head, typeof(*cursor), member),             \
        temp = list_next_entry(cursor, member);                                \
         !list_entry_is_head(cursor, head, member);                            \
         cursor = temp, temp = list_next_entry(temp, member))

/**
 *  list_for_each_entry_safe_continue(cursor, temp, head, member)
 *  Iterate over a list of entries starting from the next entry from
 *  cursor, safe from entry deletion.
 */
#define list_for_each_entry_safe_continue(cursor, temp, head, member)          \
    for (cursor = list_next_entry(cursor, member),                             \
        temp = list_next_entry(cursor, member);                                \
         !list_entry_is_head(cursor, head, member);                            \
         cursor = temp, temp = list_next_entry(temp, member))

/**
 *  list_for_each_entry_safe_from(cursor, temp, head, member)
 *  Iterate over a list of entries starting from cursor, safe from entry
 *  deletion.
 */
#define list_for_each_entry_safe_from(cursor, temp, head, member)              \
    for (temp = list_next_entry(cursor, member);                               \
         !list_entry_is_head(cursor, head, member);                            \
         cursor = temp, temp = list_next_entry(temp, member))

/**
 *  list_for_each_entry_safe_reverse(cursor, temp, head, member)
 *  Iterate over a list of entries beackward starting from cursor, safe from
 *  entry deletion.
 */
#define list_for_each_entry_safe_reverse(cursor, temp, head, member)           \
    for (cursor = list_last_entry(head, typeof(*cursor), member),              \
        temp = list_prev_entry(cursor, member);                                \
         !list_entry_is_head(cursor, head, member);                            \
         cursor = temp, temp = list_prev_entry(temp, member))

#endif