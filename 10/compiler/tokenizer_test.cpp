#include "jack_tokenizer.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "./tokenizer_test <path>\n");
        return 1;
    }

    JackTokenizer tokenizer(argv[1]);

    FILE *fp;
    if ((fp = fopen("out.xml", "w")) == NULL) {
        fprintf(stderr, "failed to open out.xml\n");
        return 1;
    }

    fprintf(fp, "<tokens>\n");
    while (tokenizer.has_more_tokens()) {
        tokenizer.advance();
        int token_type = tokenizer.token_type();
        if (token_type == STRING_CONST) {
            std::cout << "string " << tokenizer.string_val() << std::endl;
            fprintf(fp, "<stringConstant>%s</stringConstant>\n",
                    tokenizer.string_val().c_str());
        } else if (token_type == INT_CONST) {
            std::cout << "int " << tokenizer.int_val() << std::endl;
            fprintf(fp, "<intConstant>%d</intConstant>\n", tokenizer.int_val());
        } else if (token_type == SYMBOL) {
            std::cout << "symbol " << tokenizer.symbol() << std::endl;
            fprintf(fp, "<symbol>%s</symbol>\n", tokenizer.symbol().c_str());
        } else if (token_type == KEYWORD) {
            std::cout << "keyword " << tokenizer.keyword_as_string() << std::endl;
            fprintf(fp, "<keyword>%s</keyword>\n",
                    tokenizer.keyword_as_string().c_str());
        } else if (token_type == IDENTIFIER) {
            std::cout << "identifier " << tokenizer.identifier() << std::endl;
            fprintf(fp, "<identifier>%s</identifier>\n",
                    tokenizer.identifier().c_str());
        } else {
            fprintf(stderr, "invalid token type\n");
            return 1;
        }
    }
    fprintf(fp, "</tokens>\n");
    return 0;
}
