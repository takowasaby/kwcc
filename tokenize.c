#include "kwcc.h"

Token *new_token(TokenKind kind, Token *cur, char *str) {
    Token *tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    cur->next = tok;
    return tok;
}

bool is_in_idnet_tail(char c) {
    return ('a' <= c && c <= 'z')
        || ('A' <= c && c <= 'Z')
        || ('0' <= c && c <= '9')
        || c == '_';
}

Token *new_token_identifier(Token *cur, char *str) {
    Token *tok = new_token(TK_IDENT, cur, str);

    char *itr = str + 1;
    while(is_in_idnet_tail(*itr)) {
        itr++;
    }
    tok->len = itr - str;

    return tok;
}

Token *new_token_mark(Token *cur, char *str, int len) {
    Token *tok = new_token(TK_MARK, cur, str);

    int markKind = 0;
    for (int i = 0; i < len; i++) {
        markKind += str[i] << (8 * sizeof(char) * i);
    }
    tok->mkkind = markKind;

    return tok;
}

Token *new_token_keyword(KeywordKind kwkind, Token *cur, char *str) {
    Token *tok = new_token(TK_KEYWORD, cur, str);
    tok->kwkind = kwkind;
    return tok;
}

Token *new_token_num(Token *cur, char *str, int val) {
    Token *tok = new_token(TK_NUM, cur, str);
    tok->val = val;
    return tok;
}

Token *tokenize(char *p) {
    Token head;
    head.next = NULL;
    Token *cur = &head;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (strchr("<>=!", p[0]) && strchr("=", p[1])) {
            cur = new_token_mark(cur, p, 2);
            p += 2;
            continue;
        }

        if (strchr("+-*/()<>=;", *p)) {
            cur = new_token_mark(cur, p, 1);
            p += 1;
            continue;
        }

        if (strncmp("return", p, 6) == 0 && !is_in_idnet_tail(p[6])) {
            cur = new_token_keyword(KW_RETURN, cur, p);
            p += 6;
            continue;
        }

        if (('a' <= *p && *p <= 'z') || ('A' <= *p && *p <= 'Z') || *p == '_') {
            cur = new_token_identifier(cur, p);
            p += cur->len;
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token_num(cur, p, strtol(p, &p, 10));
            continue;
        }

        error_at(p, "トークナイズできません");
    }

    new_token(TK_EOF, cur, p);
    return head.next;
}