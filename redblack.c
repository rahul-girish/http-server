#include <stdio.h>
#include <stdlib.h>

enum color
{
    RED,
    BLACK
};

struct Node
{
    int data;
    enum color color;
    struct Node *left, *right, *parent;
};

struct Node *create_node(int data)
{
    struct Node *new = (struct Node *)malloc(sizeof(struct Node));
    new->data = data;
    new->color = RED;
    new->left = new->right = new->parent = NULL;

    return new;
}

void left_rotate(struct Node *node1, struct Node **root)
{
    struct Node *node2 = node1->right;
    node1->right = node2->left;
    if (node2->left != NULL)
        node2->left->parent = node1;
    node2->parent = node1->parent;

    if (node1->parent == NULL)
        *root = node2;
    else if (node1 == node1->parent->left)
    {
        node1->parent->left = node2;
    }
    else
    {
        node1->parent->right = node2;
    }

    node2->left = node1;
    node1->parent = node2;
}

void right_rotate(struct Node *node1, struct Node **root)
{
    struct Node *node2 = node1->left;
    node1->left = node2->right;

    if (node2->right != NULL)
        node2->right->parent = node1;
    node2->parent = node1->parent;

    if (node1->parent == NULL)
        *root = node2;
    else if (node1 == node1->parent->right)
    {
        node1->parent->right = node2;
    }
    else
        node1->parent->left = node2;

    node2->right = node1;
    node1->parent = node2;
}
//////////////////////////////////////////////////////
void fixInsert(struct Node *node, struct Node **root)
{
    while (node != *root && node->parent->color == RED)
    {
        if (node->parent == node->parent->parent->left)
        {
            struct Node *uncle = node->parent->parent->right;

            if (uncle != NULL && uncle->color == RED)
            {
                node->parent->color = BLACK;
                uncle->color = BLACK;

                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else
            {
                if (node == node->parent->right)
                {
                    node = node->parent;
                    left_rotate(node, root);
                }

                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                right_rotate(node->parent->parent, root);
            }
        }
        else
        {
            struct Node *uncle = node->parent->parent->left;
            printf("1\n");
            if (uncle != NULL && uncle->color == RED)
            {
                printf("2\n");
                node->parent->color = BLACK;
                uncle->color = BLACK;
                node->parent->parent->color = RED;
                node = node->parent->parent;
            }
            else
            {
                printf("3\n");
                if (node == node->parent->left)
                {
                    printf("4\n");
                    node = node->parent;
                    right_rotate(node, root);
                }
                node->parent->color = BLACK;
                node->parent->parent->color = RED;
                left_rotate(node->parent->parent, root);
            }
        }
    }

    (*root)->color = BLACK;
}

//////////////////////////////////////////////////
void insert(struct Node *node, struct Node **root)
{
    struct Node *parent = NULL;
    struct Node *cur = *root;
    while (cur != NULL)
    {
        parent = cur;
        if (cur->data > node->data)
            cur = cur->left;
        else
            cur = cur->right;
    }

    node->parent = parent;

    if (parent == NULL)
        *root = node;
    else if (node->data < parent->data)
        parent->left = node;
    else
        parent->right = node;

    if (parent == NULL)
    {
        node->color = BLACK;
        return;
    }

    if (node->parent->parent == NULL)
        return;

    fixInsert(node, root);
}

void inorder(struct Node *root)
{
    if (root != NULL)
    {
        inorder(root->left);
        printf("%d ", root->data);
        inorder(root->right);
    }
}

int main(void)
{
    struct Node *root = NULL;

    struct Node *temp = NULL;

    int data;
    int choice = 0;
    printf("1. insert\n 2. delete\n 3. search\n 4. display\n");

    while (choice != 5)
    {
        printf("enter choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            printf("enter the value: ");
            scanf("%d", &data);
            temp = create_node(data);
            insert(temp, &root);
            break;
        case 2:
            break;
        case 3:
            break;
        case 4:
            inorder(root);
            break;
        }
    }

    return 0;
}