#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    MK_PLUS = '+',
    MK_MINUS = '-',
    MK_ASTERISK = '*',
    MK_SLASH = '/',
    MK_LEFT_PAREN = '(',
    MK_RIGHT_PAREN = ')',
    MK_LEFT_ANGLE = '<',
    MK_RIGHT_ANGLE = '>',
    MK_EQUALS = '=',
    MK_SEMICOLON = ';',
    MK_LESS_EQUAL = '<' + ('=' << (8 * sizeof(char))),
    MK_MORE_EQUAL = '>' + ('=' << (8 * sizeof(char))),
    MK_DOUBLE_EQUAL = '=' + ('=' << (8 * sizeof(char))),
    MK_NOT_EQUAL = '!' + ('=' << (8 * sizeof(char))),    
} MarkKind;

typedef enum {
    KW_RETURN,
    KW_IF,
    KW_ELSE,
    KW_WHILE,
    KW_FOR,
} KeywordKind;

typedef enum {
    TK_KEYWORD,
    TK_MARK,
    TK_IDENT,
    TK_NUM,
    TK_EOF,
} TokenKind;

typedef struct Token Token;

struct Token {
    TokenKind kind;
    Token *next;
    MarkKind mkkind;
    KeywordKind kwkind;
    int val;
    char *str;
    int len;
};

typedef struct LVar LVar;

struct LVar {
    LVar *next;
    char *name;
    int len;
    int index;
};

typedef enum {
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_NUM,
    ND_EQ,
    ND_NE,
    ND_LT,
    ND_LE,
    ND_ASN,
    ND_LVAR,
    ND_RETURN,
} NodeKind;

typedef struct Node Node;

struct Node {
    NodeKind kind;
    Node *lhs;
    Node *rhs;
    int val;
    int index;
};

typedef struct Vec Vec;

struct Vec {
    void **fst;
    int size;
    int cap;
};

Vec *vec_create(int cap);
void vec_push_back(Vec* vec, void *elem);
void *vec_at(Vec* vec, int index);
int vec_size(Vec* vec);
void vec_release(Vec* vec);
int vec_test();

void error_at(char *loc, char *fmt, ...);
void error(char *fmt, ...);

LVar *add_lvar(Token *token);
LVar *find_lvar(Token *token);

// 次のトークンが期待している記号の場合、トークンを一つ読み進めて真を返す
bool consume_mark(MarkKind mkkind);
bool consume_keyword(KeywordKind kwkind);
Token *consume_identifier();
// 次のトークンが期待している記号の場合、トークンを読み進める
// そうでない場合エラーを報告する
void expect_mark(MarkKind mkkind);
// 次のトークンが数値だった場合、トークンを読み進めて数値を返す
// そうでない場合エラーを報告する
int expect_number();
bool at_eof();

Token *new_token(TokenKind kind, Token *cur, char *str);
bool is_in_idnet_tail(char c);
Token *new_token_identifier(Token *cur, char *str);
Token *new_token_mark(Token *cur, char *str, int len);
Token *new_token_keyword(KeywordKind kwkind, Token *cur, char *str);
Token *new_token_num(Token *cur, char *str, int val);
Token *tokenize(char *p);

Node *new_node(NodeKind kind, Node *lhs, Node *rhs);
Node *new_node_num(int val);
Node *new_node_identifier(int index);

Node *primary();
Node *unary();
Node *mul();
Node *add();
Node *relational();
Node *equality();
Node *assign();
Node *expr();
Node *stmt();
void program();

void gen_lval(Node *node) ;
void gen(Node *node);
void code_gen();

void print_node(Node *node, int depth);
void tree_gen(Node *node);

extern Token *token;
extern LVar *locals;
extern Node *code[256];
extern char *user_input;
