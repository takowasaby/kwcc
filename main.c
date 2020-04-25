#include "kwcc.h"

Token *token;
LVar *locals;
Node *code[256];
char *user_input;

int compile_main() {
    token = tokenize(user_input);
    program();
    code_gen();
    return 0;
}

int parse_main() {
    token = tokenize(user_input);
    program();

    for (int i = 0; code[i] != NULL; i++) {
        tree_gen(code[i]);
    }
    
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
        if (strncmp("--", p, 2) == 0) {
            if (strncmp("parse", p + 2, 5) == 0) {
                executeParseMode = true;
            }
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