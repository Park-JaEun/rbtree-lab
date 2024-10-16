#include "rbtree.h"
#include <stdlib.h>

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