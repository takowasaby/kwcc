#include "kwcc.h"

LVar *add_lvar(Token *token) {
    LVar *lvar = calloc(1, sizeof(LVar));
    lvar->name = token->str;
    lvar->len = token->len;
    lvar->index = locals->index + 1;
    lvar->next = locals;
    locals = lvar;
    return lvar;
}

LVar *find_lvar(Token *token) {
    for (LVar *itr = locals; itr; itr = itr->next) {
        if (token->len == itr->len && strncmp(token->str, itr->name, token->len) == 0) {
            return itr;
        }
    }
    return NULL;
}

bool consume_keyword(KeywordKind kwkind) {
    if (token->kind != TK_KEYWORD ||
        token->kwkind != kwkind) {
        return false;
    }
    token = token->next;
    return true;
}

bool consume_mark(MarkKind mkkind) {
    if (token->kind != TK_MARK ||
        token->mkkind != mkkind) {
        return false;
    }
    token = token->next;
    return true;
}

Token *consume_identifier() {
    if (token->kind != TK_IDENT) {
        return NULL;
    }
    Token *ret = token;
    token = token->next;
    return ret;
}

void expect_mark(MarkKind mkkind) {
    if (token->kind != TK_MARK ||
        token->mkkind != mkkind) {
        error_at(token->str, "指定された記号がトークンと一致しません");
    }
    token = token->next;
}

int expect_number() {
    if (token->kind != TK_NUM) {
        error_at(token->str, "数ではありません");
    }
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof() {
    return token->kind == TK_EOF;
}

Node *new_node(NodeKind kind, Node *lhs, Node *rhs) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node *new_node_num(int val) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

Node *new_node_identifier(int index) {
    Node *node = calloc(1, sizeof(Node));
    node->kind = ND_LVAR;
    node->index = index;
    return node;
}

Node *primary() {
    if (consume_mark(MK_LEFT_PAREN)) {
        Node *node = expr();
        expect_mark(MK_RIGHT_PAREN);
        return node;
    }
    Token* maybe_identifier = consume_identifier();
    if (maybe_identifier) {
        LVar *lvar = find_lvar(maybe_identifier);
        if (lvar) {
            return new_node_identifier(lvar->index);
        } else {
            LVar *newLVar = add_lvar(maybe_identifier);
            return new_node_identifier(newLVar->index);
        }
    }

    return new_node_num(expect_number());
}

Node *unary() {
    if (consume_mark(MK_PLUS)) {
        return primary();
    } else if (consume_mark(MK_MINUS)) {
        return new_node(ND_SUB, new_node_num(0), primary());
    }
    return primary();
}

Node *mul() {
    Node *node = unary();

    for (;;) {
        if (consume_mark(MK_ASTERISK)) {
            node = new_node(ND_MUL, node, unary());
        } else if (consume_mark(MK_SLASH)) {
            node = new_node(ND_DIV, node, unary());
        } else {
            return node;
        }
    }
}

Node *add() {
    Node *node = mul();

    for (;;) {
        if (consume_mark(MK_PLUS)) {
            node = new_node(ND_ADD, node, mul());
        } else if (consume_mark(MK_MINUS)) {
            node = new_node(ND_SUB, node, mul());
        } else {
            return node;
        }
    }
}

Node *relational() {
    Node *node = add();

    for (;;) {
        if (consume_mark(MK_LEFT_ANGLE)) {
            node = new_node(ND_LT, node, add());
        } else if (consume_mark(MK_LESS_EQUAL)) {
            node = new_node(ND_LE, node, add());
        } else if (consume_mark(MK_RIGHT_ANGLE)) {
            node = new_node(ND_LE, add(), node);
        } else if (consume_mark(MK_MORE_EQUAL)) {
            node = new_node(ND_LT, add(), node);
        } else {
            return node;
        }
    }
}

Node *equality() {
    Node *node = relational();

    for (;;) {
        if (consume_mark(MK_DOUBLE_EQUAL)) {
            node = new_node(ND_EQ, node, relational());
        } else if (consume_mark(MK_NOT_EQUAL)) {
            node = new_node(ND_NE, node, relational());
        } else {
            return node;
        }
    }
}

Node *assign() {
    Node *node = equality();

    if (consume_mark(MK_EQUALS)) {
        return new_node(ND_ASN, node, assign());
    }

    return node;
}

Node *expr() {
    return assign();
}

Node *stmt() {
    if (consume_keyword(KW_RETURN)) {
        Node *node = calloc(1, sizeof(Node));
        node->kind = ND_RETURN;
        node->lhs = expr();
        expect_mark(MK_SEMICOLON);
        return node;
    } else {
        Node *node = expr();
        expect_mark(MK_SEMICOLON);
        return node;
    }
}

void program() {
    locals = calloc(1, sizeof(LVar));
    locals->index = -1;
    locals->name = "";
    locals->len = 0;
    locals->next = NULL;

    int i = 0;
    while (!at_eof()) {
        code[i] = stmt();
        i++;
    }
    code[i] = NULL;
}
