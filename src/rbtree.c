#include "rbtree.h"
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree* p = (rbtree*)calloc(1, sizeof(rbtree)); // 트리 구조체 메모리 할당
  node_t* nil_node = (node_t*)calloc(1, sizeof(node_t)); // sentinel 노드(nil) 생성
 
  nil_node->color = RBTREE_BLACK;  // sentinel 노드는 항상 검정색
  p->nil = nil_node;
  p->root = p->nil;  // 빈 트리는 root가 nil을 가리킴

  return p;
}

void delete_rbtree(rbtree *t) {
    // 트리 내의 모든 노드를 삭제
    if (t->root != t->nil) {
        delete_node(t, t->root);  // 재귀적으로 노드 삭제
    }
    free(t->nil);  // sentinel 노드 메모리 해제
    free(t);  // 트리 구조체 메모리 해제
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  return t->root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  return 0;
}
