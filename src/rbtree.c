#include "rbtree.h"
#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree* p = (rbtree*)calloc(1, sizeof(rbtree)); // Ʈ�� ����ü �޸� �Ҵ�
  node_t* nil_node = (node_t*)calloc(1, sizeof(node_t)); // sentinel ���(nil) ����
 
  nil_node->color = RBTREE_BLACK;  // sentinel ���� �׻� ������
  p->nil = nil_node;
  p->root = p->nil;  // �� Ʈ���� root�� nil�� ����Ŵ

  return p;
}

void delete_rbtree(rbtree *t) {
    // Ʈ�� ���� ��� ��带 ����
    if (t->root != t->nil) {
        delete_node(t, t->root);  // ��������� ��� ����
    }
    free(t->nil);  // sentinel ��� �޸� ����
    free(t);  // Ʈ�� ����ü �޸� ����
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
