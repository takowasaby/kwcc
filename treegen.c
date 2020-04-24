#include "kwcc.h"

void print_node(Node *node, int depth) {
    for (int i = 0; i < depth; i++) {
        printf("\t");
    }

    switch (node->kind) {
    case ND_NUM:
        printf("%d\n", node->val);
        return;
    case ND_LVAR:
        printf("[rbp-%d]\n", (node->index + 1) * 8);
        return;
    case ND_ADD:
        printf("ADD\n");
        break;
    case ND_SUB:
        printf("SUB\n");
        break;
    case ND_MUL:
        printf("MUL\n");
        break;
    case ND_DIV:
        printf("DIV\n");
        break;
    case ND_EQ:
        printf("EQ\n");
        break;
    case ND_NE:
        printf("NE\n");
        break;
    case ND_LT:
        printf("LT\n");
        break;
    case ND_LE:
        printf("LE\n");
        break;
    case ND_ASN:
        printf("ASN\n");
        break;
    default:
        error("構文木のノードの種類が不適切です");
        break;
    }

    print_node(node->lhs, depth + 1);
    print_node(node->rhs, depth + 1);
}

void tree_gen(Node *node) {
    print_node(node, 0);
}