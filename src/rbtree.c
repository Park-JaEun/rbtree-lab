#include "rbtree.h"
#include <stdlib.h>

/* x 노드 중심으로 왼쪽 회전 */
static void left_rotate(rbtree* t, node_t* x) {
    node_t* y = x->right; // y는 x의 오른쪽 자식
    x->right = y->left; // y의 왼쪽 서브트리를 x의 오른쪽 서브트리로 이동
    if (y->left != t->nil) {
        y->left->parent = x;
    }
    y->parent = x->parent; // y의 부모를 x의 부모로 설정
    if (x->parent == t->nil) {
        t->root = y; // x가 루트이면 y가 새로운 루트가 됨
    }
    else if (x == x->parent->left) {
        x->parent->left = y; // x가 왼쪽 자식이면 y를 왼쪽 자식으로 설정
    }
    else {
        x->parent->right = y; // x가 오른쪽 자식이면 y를 오른쪽 자식으로 설정
    }
    y->left = x; // x를 y의 왼쪽 자식으로 설정
    x->parent = y; // x의 부모를 y로 설정
}

/* y노드 중심으로 오른쪽 회전*/
static void right_rotate(rbtree* t, node_t* y) {
    node_t* x = y->left; // x는 y의 왼쪽 자식
    y->left = x->right; // x의 오른쪽 서브트리를 y의 왼쪽 서브트리로 이동
    if (x->right != t->nil) {
        x->right->parent = y;
    }
    x->parent = y->parent; // x의 부모를 y의 부모로 설정
    if (y->parent == t->nil) {
        t->root = x; // y가 루트이면 x가 새로운 루트가 됨
    }
    else if (y == y->parent->right) {
        y->parent->right = x; // y가 오른쪽 자식이면 x를 오른쪽 자식으로 설정
    }
    else {
        y->parent->left = x; // y가 왼쪽 자식이면 x를 왼쪽 자식으로 설정
    }
    x->right = y; // y를 x의 오른쪽 자식으로 설정
    y->parent = x; // y의 부모를 x로 설정
}


/* 새 노드 삽입 후 레드-블랙 트리 속성 유지를 위한 균형 작업하는 함수
 z노드(RED)에 대해 트리의 속성을 유지를 위한 색변경 + 회전*/
static void rbtree_insert_fixup(rbtree* t, node_t* z) {
    while (z->parent->color == RBTREE_RED) {
        if (z->parent == z->parent->parent->left) {
            node_t* y = z->parent->parent->right; // 삼촌 노드

            if (y->color == RBTREE_RED) {
                // 삼촌이 빨간색이면
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent; // 조부모 노드로 이동
            }
            else {
                if (z == z->parent->right) {
                    // z가 오른쪽 자식이면
                    z = z->parent;
                    left_rotate(t, z);
                }
                // z가 왼쪽 자식이면
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                right_rotate(t, z->parent->parent);
            }
        }
        else {
            // 부모가 조부모의 오른쪽 자식이면 (대칭)
            node_t* y = z->parent->parent->left; // 삼촌 노드

            if (y->color == RBTREE_RED) {
                // 삼촌이 빨간색이면
                z->parent->color = RBTREE_BLACK;
                y->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                z = z->parent->parent; // 조부모 노드로 이동
            }
            else {
                if (z == z->parent->left) {
                    // z가 왼쪽 자식이면
                    z = z->parent;
                    right_rotate(t, z);
                }
                // z가 오른쪽 자식이면
                z->parent->color = RBTREE_BLACK;
                z->parent->parent->color = RBTREE_RED;
                left_rotate(t, z->parent->parent);
            }
        }
    }
    t->root->color = RBTREE_BLACK; // 루트는 항상 검은색이어야 함
}

/* 서브트리 해제 함수
 후위 순회로 서브트리 모든 노드를 해제함 */
static void free_subtree(rbtree* t, node_t* x) {
    if (x != t->nil) {
        free_subtree(t, x->left);    // 왼쪽 서브트리 해제
        free_subtree(t, x->right);   // 오른쪽 서브트리 해제
        free(x);                     // 현재 노드 해제
    }
}

/* 중위 순회로 트리의 키 값을 오름차순으로 배열에 저장함 */
static void inorder_traversal(rbtree* t, node_t* x, key_t* arr, size_t* index, size_t n) {
    if (x == t->nil || *index >= n) {
        return; // 노드가 nil이거나 배열이 가득 찼으면 종료
    }
    inorder_traversal(t, x->left, arr, index, n); // 왼쪽 서브트리 탐색
    if (*index < n) {
        arr[*index] = x->key; // 현재 노드의 키를 배열에 저장
        (*index)++;
    }
    inorder_traversal(t, x->right, arr, index, n); // 오른쪽 서브트리 탐색
}

/* 노드 삭제 후 레드블랙 트리 속성을 유지하기 위한 수정 작업
 x: 수정 중심 노드의 포인터  */
static void rbtree_erase_fixup(rbtree* t, node_t* x) {
    while (x != t->root && x->color == RBTREE_BLACK) {
        if (x == x->parent->left) {
            node_t* w = x->parent->right; // 형제 노드

            if (w->color == RBTREE_RED) {
                // 형제가 빨간색이면
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                left_rotate(t, x->parent);
                w = x->parent->right;
            }

            if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
                // 형제의 자식들이 모두 검은색이면
                w->color = RBTREE_RED;
                x = x->parent;
            }
            else {
                if (w->right->color == RBTREE_BLACK) {
                    // 형제의 오른쪽 자식이 검은색이면
                    w->left->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    right_rotate(t, w);
                    w = x->parent->right;
                }
                // 형제의 오른쪽 자식이 빨간색이면
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->right->color = RBTREE_BLACK;
                left_rotate(t, x->parent);
                x = t->root;
            }
        }
        else {   // x가 부모의 오른쪽 자식이면 -> 대칭
            node_t* w = x->parent->left; // 형제 노드

            if (w->color == RBTREE_RED) {
                // 형제가 빨간색이면
                w->color = RBTREE_BLACK;
                x->parent->color = RBTREE_RED;
                right_rotate(t, x->parent);
                w = x->parent->left;
            }

            if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
                // 형제의 자식들이 모두 검은색이면
                w->color = RBTREE_RED;
                x = x->parent;
            }
            else {
                if (w->left->color == RBTREE_BLACK) {
                    // 형제의 왼쪽 자식이 검은색이면
                    w->right->color = RBTREE_BLACK;
                    w->color = RBTREE_RED;
                    left_rotate(t, w);
                    w = x->parent->left;
                }
                // 형제의 왼쪽 자식이 빨간색이면
                w->color = x->parent->color;
                x->parent->color = RBTREE_BLACK;
                w->left->color = RBTREE_BLACK;
                right_rotate(t, x->parent);
                x = t->root;
            }
        }
    }
    x->color = RBTREE_BLACK; // x는 항상 검은색으로..
}

static void rbtree_transplant(rbtree* t, node_t* u, node_t* v) {
    if (u->parent == t->nil) {
        t->root = v; // u가 루트이면 v가 새로운 루트가 됨
    }
    else if (u == u->parent->left) {
        u->parent->left = v; // u가 왼쪽 자식이면 부모의 왼쪽 포인터를 v로 설정
    }
    else {
        u->parent->right = v; // u가 오른쪽 자식이면 부모의 오른쪽 포인터를 v로 설정
    }
    v->parent = u->parent; // v의 부모를 u의 부모로 설정
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

/* 새 노드 생성 후 트리에 삽입 후, 레드블랙 속성 유지 작업*/
node_t* rbtree_insert(rbtree* t, const key_t key) {
    // 새로운 노드 메모리 할당
    node_t* z = (node_t*)calloc(1, sizeof(node_t));
    if (z == NULL) {
        perror("새 노드 메모리 할당 실패");
        exit(EXIT_FAILURE);
    }
    z->key = key;
    z->left = z->right = z->parent = t->nil;
    z->color = RBTREE_RED; // 새 노드는 초기에는 빨간색

    // 삽입 위치 탐색
    node_t* y = t->nil; // 새 노드의 부모
    node_t* x = t->root; // 현재 탐색 중인 노드

    while (x != t->nil) {
        y = x;
        if (z->key < x->key) {
            x = x->left;
        }
        else {
            // 멀티셋: 중복 키는 오른쪽 서브트리에 삽입
            x = x->right;
        }
    }

    z->parent = y;

    if (y == t->nil) {
        // 트리가 비어있으면 새 노드가 루트..
        t->root = z;
    }
    else if (z->key < y->key) {
        y->left = z;
    }
    else {
        y->right = z;
    }

    // 레드-블랙 트리 속성 유지
    rbtree_insert_fixup(t, z);

    return z;
}

node_t* rbtree_find(const rbtree* t, const key_t key) {
    node_t* x = t->root;

    while (x != t->nil) {
        if (key == x->key) {
            return x; // 키를 가진 노드 발견
        }
        else if (key < x->key) {
            x = x->left; // 왼쪽 서브트리로 이동
        }
        else {
            x = x->right; // 오른쪽 서브트리로 이동
        }
    }

    return NULL; // 키를 가진 노드가 없음
}

node_t* rbtree_min(const rbtree* t) {
    node_t* x = t->root;
    if (x == t->nil) {
        return NULL; // 트리가 비어있음
    }

    // 왼쪽 서브트리를 따라가며 최소 노드 탐색
    while (x->left != t->nil) {
        x = x->left;
    }

    return x;
}

node_t* rbtree_max(const rbtree* t) {
    node_t* x = t->root;
    if (x == t->nil) {
        return NULL; // 트리가 비어있음
    }

    // 오른쪽 서브트리를 따라가며 최대 노드 탐색
    while (x->right != t->nil) {
        x = x->right;
    }

    return x;
}

/* 노드를 트리에서 삭제 후, 레드블랙 속성 유지 작업*/
int rbtree_erase(rbtree* t, node_t* z) {
    if (z == NULL || z == t->nil) {
        return -1; // 존재하지 않는 노드를 삭제할 수 없음
    }

    node_t* y = z;
    color_t y_original_color = y->color;
    node_t* x;

    if (z->left == t->nil) {
        // 왼쪽 자식이 NIL이면 z를 오른쪽 자식으로 대체
        x = z->right;
        rbtree_transplant(t, z, z->right);
    }
    else if (z->right == t->nil) {
        // 오른쪽 자식이 NIL이면 z를 왼쪽 자식으로 대체
        x = z->left;
        rbtree_transplant(t, z, z->left);
    }
    else {
        // 양쪽 자식이 모두 있으면 후계자를 찾아 대체
        y = z->right;
        while (y->left != t->nil) {
            y = y->left;
        }
        y_original_color = y->color;
        x = y->right;

        if (y->parent == z) {
            x->parent = y;
        }
        else {
            rbtree_transplant(t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        }

        rbtree_transplant(t, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->color = z->color;
    }

    // 삭제된 노드의 메모리 해제
    free(z);

    if (y_original_color == RBTREE_BLACK) {
        // 삭제된 노드가 검은색이면 레드-블랙 속성 유지 필요
        rbtree_erase_fixup(t, x);
    }

    return 0;
}

/*
 트리의 내용을 키 순서대로 배열에 저장*/
int rbtree_to_array(const rbtree* t, key_t* arr, const size_t n) {
    if (t == NULL || arr == NULL) {
        return 0; // 유효하지 않은 입력
    }

    size_t index = 0;
    inorder_traversal((rbtree*)t, t->root, arr, &index, n);

    return (int)index;
}