#include "jack_tokenizer.h"

bool is_symbol(char ch) {
    switch (ch) {
        case '(':
        case ')':
        case '{':
        case '}':
        case '[':
        case ']':
        case ';':
        case ',':
        case '.':
        case '+':
        case '-':
        case '*':
        case '/':
        case '&':
        case '|':
        case '<':
        case '>':
        case '=':
        case '~':
            return true;
    }
    return false;
}

int is_keyword(std::string str) {
    if (str == "class") return CLASS;
    if (str == "constructor") return CONSTRUCTOR;
    if (str == "function") return FUNCTION;
    if (str == "method") return METHOD;
    if (str == "field") return FIELD;
    if (str == "static") return STATIC;
    if (str == "var") return VAR;
    if (str == "int") return INT;
    if (str == "char") return CHAR;
    if (str == "boolean") return BOOLEAN;
    if (str == "void") return VOID;
    if (str == "true") return TRUE;
    if (str == "false") return FALSE;
    if (str == "null") return _NULL;
    if (str == "this") return THIS;
    if (str == "let") return LET;
    if (str == "do") return DO;
    if (str == "if") return IF;
    if (str == "else") return ELSE;
    if (str == "while") return WHILE;
    if (str == "return") return RETURN;
    return -1;
}

JackTokenizer::JackTokenizer(const char* path) : ifs_(path) {
    current_token_ = "";
    next_token_ = "";
    advance();
}

bool JackTokenizer::has_more_tokens() {
    return !next_token_.empty();
}

void JackTokenizer::advance() {
    current_token_ = next_token_;
    next_token_ = "";
    current_token_type_ = next_token_type_;
    next_token_type_ = -1;

    if (ifs_.eof())
        return;

    // skip to next head
    char ch = ifs_.get();
    while (!ifs_.eof()) {
        char next_ch = ifs_.get();
        if (ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t') {
            ch = next_ch;
            continue;
        }
        // skip to end of line
        if (ch == '/' && next_ch == '/') {
            while (!ifs_.eof())
                if (ifs_.get() == '\n')
                    break;
            ch = ifs_.get();
            continue;
        }
        // skip to end of comment
        if (ch == '/' && next_ch == '*') {
            ch = '\0';
            while (!ifs_.eof()) {
                next_ch = ifs_.get();
                if (ch == '*' && next_ch == '/')
                    break;
                ch = next_ch;
            }
            ch = ifs_.get();
            continue;
        }
        break;
    }
    if (ifs_.eof())
        return;
    ifs_.unget();
    ifs_.unget();

    char head_ch = ifs_.get();
    if (head_ch == '"') { // string const
        char tmp;
        while ((tmp = ifs_.get()) != '"')
            next_token_ += tmp;
        next_token_type_ = STRING_CONST;
    } else if (head_ch >= '0' && head_ch <= '9') { // int const
        char tmp;
        next_token_ += head_ch;
        while ((tmp = ifs_.get()) >= '0' && tmp <= '9')
            next_token_ += tmp;
        ifs_.unget();
        next_token_type_ = INT_CONST;
    } else if (is_symbol(head_ch)) { // symbol
        next_token_ += head_ch;
        next_token_type_ = SYMBOL;
    } else { // keyword or identifier
        next_token_ += head_ch;
        char tmp;
        while ((tmp = ifs_.get()) != ' ' && tmp != '\t' && !is_symbol(tmp))
            next_token_ += tmp;
        ifs_.unget();
        if (is_keyword(next_token_) > -1)
            next_token_type_ = KEYWORD;
        else
            next_token_type_ = IDENTIFIER;
    }
}

int JackTokenizer::token_type() {
    return current_token_type_;
}

int JackTokenizer::keyword() {
    if (token_type() != KEYWORD)
        throw std::runtime_error(current_token_ + " is not a keyword type");
    return is_keyword(current_token_);
}

std::string JackTokenizer::keyword_as_string() {
    if (token_type() != KEYWORD)
        throw std::runtime_error(current_token_ + " is not a keyword type");
    return current_token_;
}

std::string JackTokenizer::symbol() {
    if (token_type() != SYMBOL)
        throw std::runtime_error(current_token_ + " is not a symbol type");
    return std::string("") + current_token_[0];
}

std::string JackTokenizer::identifier() {
    if (token_type() != IDENTIFIER)
        throw std::runtime_error(current_token_ + " is not a identifier type");
    return current_token_;
}

int JackTokenizer::int_val() {
    if (token_type() != INT_CONST)
        throw std::runtime_error(current_token_ + " is not a int_const type");
    return atoi(current_token_.c_str());
}

std::string JackTokenizer::string_val() {
    if (token_type() != STRING_CONST)
        throw std::runtime_error(current_token_ + " is not a string_const type");
    return current_token_;
}
