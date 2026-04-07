#include "linkedlist.h"

void list_add(struct list *node, struct list *head)
{
    __list_add(node, head, head->next);
}

void list_add_tail(struct list *node, struct list *head)
{
    __list_add(node, head->prev, head);
}

void list_del(struct list *node) { __list_del(node->prev, node->next); }

void list_del_init(struct list *node)
{
    __list_del(node->prev, node->next);
    LIST_INIT(node);
}

int list_empty(struct list *head) { return head->next == head; }

void list_add_before(struct list *node, struct list *pos)
{
    __list_add(node, pos->prev, pos);
}

void list_add_after(struct list *node, struct list *pos)
{
    __list_add(node, pos, pos->next);
}