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

    bool is_exist_input = false;
    bool execute_parse_mode = false;
    bool execute_vectest_mode = false;

    for (int i = 1; i < argc; i++) {
        char *p = argv[i];
        if (strncmp("--", p, 2) == 0) {
            if (strncmp("parse", p + 2, 5) == 0) {
                execute_parse_mode = true;
            }
            if (strncmp("vectest", p + 2, 7) == 0) {
                execute_vectest_mode = true;
            }
        } else {
            is_exist_input = true;
            user_input = argv[i];
        }
    }

    if (!is_exist_input) {
        error("引数が正しくありません\n");
        return 1;
    }

    if (execute_parse_mode) {
        return parse_main();
    }

    if (execute_vectest_mode) {
        return vec_test();
    }

    return compile_main();
}