#include "rbtree.h"
#include <stdlib.h>

/* x 노드 중심으로 왼쪽 회전 */
static void left_rotate(rbtree* t, node_t* x) {
    node_t* y = x->right;
    x->right = y->left;
    if (y->left != t->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == t->nil) {
        t->root = y;
    }
    else if (x == x->parent->left) {
        x->parent->left = y;
    }
    else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

/* y노드 중심으로 오른쪽 회전*/
static void right_rotate(rbtree* t, node_t* y) {
    node_t* x = y->left;
    y->left = x->right;
    if (x->right != t->nil) {
        x->right->parent = y;
    }
    x->parent = y->parent;
    if (y->parent == t->nil) {
        t->root = x;
    }
    else if (y == y->parent->right) {
        y->parent->right = x;
    }
    else {
        y->parent->left = x;
    }
    x->right = y;
    y->parent = x;
}

/* 새 노드 삽입 후 레드-블랙 트리 속성 유지를 위한 균형 작업하는 함수
 z노드(RED)에 대해 트리의 속성을 유지를 위한 색변경 + 회전*/
static void rbtree_insert_fixup(rbtree* t, node_t* z) {
    while (z->parent->color == RBTREE_RED) {
        if (z->parent == z->parent->parent->left) {
            node_t* y = z->parent->parent->right;

            if (y->color == RBTREE_RED) {
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate(t, z);
                }
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                right_rotate(t, z->parent->parent);
            }
        }
        else {
            node_t* y = z->parent->parent->left;

            if (y->color == RBTREE_RED) {
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent;
            }
            else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate(t, z);
                }
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                left_rotate(t, z->parent->parent);
            }
        }
    }
    t->root->color = RBTREE_BLACK;
}

static void free_subtree(rbtree* t, node_t* x) {
    if (x != t->nil) {
        free_subtree(t, x->left);
        free_subtree(t, x->right);
        free(x);
    }
}


//=================================================================================//
//=================================================================================//
//=================================================================================//

/*  센티넬 노드는 트리의 NIL 리프 노드를 나타냄.
 트리의 루트는 초기에는 센티넬 노드 가짐.*/
rbtree* new_rbtree(void) {
    // 레드-블랙 트리 구조체 메모리 할당
    rbtree* t = (rbtree*)calloc(1, sizeof(rbtree));
    if (t == NULL) {
        perror("레드-블랙 트리 구조체 메모리 할당 실패");
        exit(EXIT_FAILURE);
    }

    // 센티넬 노드(nil) 메모리 할당 및 초기화
    t->nil = (node_t*)calloc(1, sizeof(node_t));
    if (t->nil == NULL) {
        perror("센티넬 노드 메모리 할당 실패");
        free(t);
        exit(EXIT_FAILURE);
    }
    t->nil->color = RBTREE_BLACK; // 센티넬 노드는 항상 검은색
    t->nil->left = t->nil->right = t->nil->parent = NULL;
    t->root = t->nil; // 초기 트리는 비어있으므로 루트는 nil을 가리킴

    return t;
}

/* t: 삭제할 레드-블랙 트리의 포인터*/
void delete_rbtree(rbtree* t) {
    if (t == NULL) return;

    // 트리의 모든 노드를 재귀적으로 해제
    free_subtree(t, t->root);

    // 센티넬 노드 해제
    free(t->nil);

    // 트리 구조체 해제
    free(t);
}

node_t* rbtree_insert(rbtree* t, const key_t key) {
    node_t* z = (node_t*)calloc(1, sizeof(node_t));
    if (z == NULL) {
        perror("새 노드 메모리 할당 실패");
        exit(EXIT_FAILURE);
    }
    z->key = key;
    z->left = z->right = z->parent = t->nil;
    z->color = RBTREE_RED;

    node_t* y = t->nil;
    node_t* x = t->root;

    while (x != t->nil) {
        y = x;
        if (z->key < x->key) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }

    z->parent = y;

    if (y == t->nil) {
        t->root = z;
    }
    else if (z->key < y->key) {
        y->left = z;
    }
    else {
        y->right = z;
    }

    rbtree_insert_fixup(t, z);

    return z;
}

node_t* rbtree_find(const rbtree* t, const key_t key) {
    return t->root;
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