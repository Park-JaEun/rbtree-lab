#include "rbtree.h"
#include <stdlib.h>

/* x ��� �߽����� ���� ȸ�� */
static void left_rotate(rbtree* t, node_t* x) {
    node_t* y = x->right; // y�� x�� ������ �ڽ�
    x->right = y->left; // y�� ���� ����Ʈ���� x�� ������ ����Ʈ���� �̵�
    if (y->left != t->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent; // y�� �θ� x�� �θ�� ����
    if (x->parent == t->nil) {
        t->root = y; // x�� ��Ʈ�̸� y�� ���ο� ��Ʈ�� ��
    }
    else if (x == x->parent->left) {
        x->parent->left = y; // x�� ���� �ڽ��̸� y�� ���� �ڽ����� ����
    }
    else {
        x->parent->right = y; // x�� ������ �ڽ��̸� y�� ������ �ڽ����� ����
    }
    y->left = x; // x�� y�� ���� �ڽ����� ����
    x->parent = y; // x�� �θ� y�� ����
}

/* y��� �߽����� ������ ȸ��*/
static void right_rotate(rbtree* t, node_t* y) {
    node_t* x = y->left; // x�� y�� ���� �ڽ�
    y->left = x->right; // x�� ������ ����Ʈ���� y�� ���� ����Ʈ���� �̵�
    if (x->right != t->nil) {
        x->right->parent = y;
    }
    x->parent = y->parent; // x�� �θ� y�� �θ�� ����
    if (y->parent == t->nil) {
        t->root = x; // y�� ��Ʈ�̸� x�� ���ο� ��Ʈ�� ��
    }
    else if (y == y->parent->right) {
        y->parent->right = x; // y�� ������ �ڽ��̸� x�� ������ �ڽ����� ����
    }
    else {
        y->parent->left = x; // y�� ���� �ڽ��̸� x�� ���� �ڽ����� ����
    }
    x->right = y; // y�� x�� ������ �ڽ����� ����
    y->parent = x; // y�� �θ� x�� ����
}


/* �� ��� ���� �� ����-�� Ʈ�� �Ӽ� ������ ���� ���� �۾��ϴ� �Լ�
 z���(RED)�� ���� Ʈ���� �Ӽ��� ������ ���� ������ + ȸ��*/
static void rbtree_insert_fixup(rbtree* t, node_t* z) {
    while (z->parent->color == RBTREE_RED) {
        if (z->parent == z->parent->parent->left) {
            node_t* y = z->parent->parent->right; // ���� ���

            if (y->color == RBTREE_RED) {
                // ������ �������̸�
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent; // ���θ� ���� �̵�
            }
            else {
                if (z == z->parent->right) {
                    // z�� ������ �ڽ��̸�
                    z = z->parent;
                    left_rotate(t, z);
                }
                // z�� ���� �ڽ��̸�
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                right_rotate(t, z->parent->parent);
            }
        }
        else {
            // �θ� ���θ��� ������ �ڽ��̸� (��Ī)
            node_t* y = z->parent->parent->left; // ���� ���

            if (y->color == RBTREE_RED) {
                // ������ �������̸�
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent; // ���θ� ���� �̵�
            }
            else {
                if (z == z->parent->left) {
                    // z�� ���� �ڽ��̸�
                    z = z->parent;
                    right_rotate(t, z);
                }
                // z�� ������ �ڽ��̸�
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                left_rotate(t, z->parent->parent);
            }
        }
    }
    t->root->color = RBTREE_BLACK; // ��Ʈ�� �׻� �������̾�� ��
}

/* ����Ʈ�� ���� �Լ�
 ���� ��ȸ�� ����Ʈ�� ��� ��带 ������ */
static void free_subtree(rbtree* t, node_t* x) {
    if (x != t->nil) {
        free_subtree(t, x->left);    // ���� ����Ʈ�� ����
        free_subtree(t, x->right);   // ������ ����Ʈ�� ����
        free(x);                     // ���� ��� ����
    }
}


//=================================================================================//
//=================================================================================//
//=================================================================================//

/*  ��Ƽ�� ���� Ʈ���� NIL ���� ��带 ��Ÿ��.
 Ʈ���� ��Ʈ�� �ʱ⿡�� ��Ƽ�� ��� ����.*/
rbtree* new_rbtree(void) {
    // ����-�� Ʈ�� ����ü �޸� �Ҵ�
    rbtree* t = (rbtree*)calloc(1, sizeof(rbtree));
    if (t == NULL) {
        perror("����-�� Ʈ�� ����ü �޸� �Ҵ� ����");
        exit(EXIT_FAILURE);
    }

    // ��Ƽ�� ���(nil) �޸� �Ҵ� �� �ʱ�ȭ
    t->nil = (node_t*)calloc(1, sizeof(node_t));
    if (t->nil == NULL) {
        perror("��Ƽ�� ��� �޸� �Ҵ� ����");
        free(t);
        exit(EXIT_FAILURE);
    }
    t->nil->color = RBTREE_BLACK; // ��Ƽ�� ���� �׻� ������
    t->nil->left = t->nil->right = t->nil->parent = NULL;
    t->root = t->nil; // �ʱ� Ʈ���� ��������Ƿ� ��Ʈ�� nil�� ����Ŵ

    return t;
}

/* t: ������ ����-�� Ʈ���� ������*/
void delete_rbtree(rbtree* t) {
    if (t == NULL) return;

    // Ʈ���� ��� ��带 ��������� ����
    free_subtree(t, t->root);

    // ��Ƽ�� ��� ����
    free(t->nil);

    // Ʈ�� ����ü ����
    free(t);
}

/* �� ��� ���� �� Ʈ���� ���� ��, ����� �Ӽ� ���� �۾�*/
node_t* rbtree_insert(rbtree* t, const key_t key) {
    // ���ο� ��� �޸� �Ҵ�
    node_t* z = (node_t*)calloc(1, sizeof(node_t));
    if (z == NULL) {
        perror("�� ��� �޸� �Ҵ� ����");
        exit(EXIT_FAILURE);
    }
    z->key = key;
    z->left = z->right = z->parent = t->nil;
    z->color = RBTREE_RED; // �� ���� �ʱ⿡�� ������

    // ���� ��ġ Ž��
    node_t* y = t->nil; // �� ����� �θ�
    node_t* x = t->root; // ���� Ž�� ���� ���

    while (x != t->nil) {
        y = x;
        if (z->key < x->key) {
            x = x->left;
        }
        else {
            // ��Ƽ��: �ߺ� Ű�� ������ ����Ʈ���� ����
            x = x->right;
        }
    }

    z->parent = y;

    if (y == t->nil) {
        // Ʈ���� ��������� �� ��尡 ��Ʈ..
        t->root = z;
    }
    else if (z->key < y->key) {
        y->left = z;
    }
    else {
        y->right = z;
    }

    // ����-�� Ʈ�� �Ӽ� ����
    rbtree_insert_fixup(t, z);

    return z;
}

node_t* rbtree_find(const rbtree* t, const key_t key) {
    node_t* x = t->root;

    while (x != t->nil) {
        if (key == x->key) {
            return x; // Ű�� ���� ��� �߰�
        }
        else if (key < x->key) {
            x = x->left; // ���� ����Ʈ���� �̵�
        }
        else {
            x = x->right; // ������ ����Ʈ���� �̵�
        }
    }

    return NULL; // Ű�� ���� ��尡 ����
}

node_t* rbtree_min(const rbtree* t) {
    return t->root;
}

node_t* rbtree_max(const rbtree* t) {
    return t->root;
}

int rbtree_erase(rbtree* t, node_t* p) {
    return 0;
}

int rbtree_to_array(const rbtree* t, key_t* arr, const size_t n) {
    return 0;
}