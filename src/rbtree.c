#include "rbtree.h"
#include <stdlib.h>

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
    return t->root;
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