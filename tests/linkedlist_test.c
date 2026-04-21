#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>

struct list_int
{
    int data;
    struct list list;
};

struct list_int *list_int_init(int data)
{
    printf("allocated memory for %d\n", data);
    struct list_int *node = (struct list_int *)malloc(sizeof(struct list_int));
    LIST_INIT(&node->list);
    node->data = data;

    return node;
}

void list_int_del(struct list_int *node, struct list *head)
{
    if (list_entry_is_head(node, head, list))
        return;
    list_del_init(&node->list);
    printf("freed memory for %d\n", node->data);
    free(node);
}

int main(void)
{
    LIST_HEAD(head);
    int data;
    int choice = 0;
    struct list_int *node1;

    printf("linked list demo\n");
    printf("\n1. add\n2. delete\n3. display\n\n");
    while (choice != 4)
    {
        printf("enter choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            printf("enter value: ");
            scanf("%d", &data);
            node1 = list_int_init(data);
            list_add(&node1->list, &head);
            break;
        case 2:
            if (!list_empty(&head))
            {
                node1 = container_of(head.next, struct list_int, list);
                list_int_del(node1, &head);
            }
            break;
        case 3:
            list_for_each_entry(node1, &head, list)
            {
                printf("%d ", node1->data);
            }
            break;
        }
    }

    struct list_int *temp;
    list_for_each_entry_safe(node1, temp, &head, list)
    {
        list_int_del(node1, &head);
    }

    return 0;
}