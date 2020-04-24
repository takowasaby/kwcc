#include "kwcc.h"

Token *token;
Node *code[256];
char *user_input;

int compile_main() {
    token = tokenize(user_input);
    program();

    printf(".intel_syntax noprefix\n");
    printf(".global main\n");
    printf("main:\n");
    
    gen(code[0]);

    printf("    pop rax\n");
    printf("    ret\n");
    
    return 0;
}

int parse_main() {
    token = tokenize(user_input);
    Node* node = stmt();

    tree_gen(node);
    
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        error("引数の個数が正しくありません\n");
        return 1;
    }

    bool isExistInput = false;
    bool executeParseMode = false;

    for (int i = 1; i < argc; i++) {
        char *p = argv[i];
        if (p[0] == '-') {
            executeParseMode = true;
        } else {
            isExistInput = true;
            user_input = argv[i];
        }
    }

    if (!isExistInput) {
        error("引数が正しくありません\n");
        return 1;
    }

    if (executeParseMode) {
        return parse_main();
    }

    return compile_main();
}