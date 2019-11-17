#include "compilation_engine.h"

bool is_type(int keyword) {
    if (keyword == INT) return true;
    else if (keyword == BOOLEAN) return true;
    else if (keyword == CHAR) return true;
    else if (keyword == VOID) return true;
    else return false;
}

CompilationEngine::CompilationEngine(JackTokenizer *tokenizer, const char* path) {
    tokenizer_ = tokenizer;
    if ((fp_ = fopen(path, "w")) == NULL)
        throw std::runtime_error("failed to open output file");
}

void CompilationEngine::compileClass() {
    tokenizer_->advance();
    if (tokenizer_->keyword() != CLASS)
        throw std::runtime_error("compileClass: invalid keyword");
    fprintf(fp_, "<class>\n");
    fprintf(fp_, "<keyword>class</keyword>\n");

    // class name
    tokenizer_->advance();
    if (tokenizer_->token_type() != IDENTIFIER)
        throw std::runtime_error("compileClass: should be class name");
    fprintf(fp_, "<identifier>%s</identifier>\n", tokenizer_->identifier().c_str());

    // open curly brace
    tokenizer_->advance();
    if (tokenizer_->token_type() != SYMBOL || tokenizer_->symbol() != "{")
        throw std::runtime_error("compileClass: should be {");
    fprintf(fp_, "<symbol>{</symbol>\n");

    // subroutines and variable declarations
    while (tokenizer_->has_more_tokens()) {
        tokenizer_->advance();
        if (tokenizer_->keyword() == METHOD || tokenizer_->keyword() == CONSTRUCTOR) {
            compileSubroutine();
        } else if (tokenizer_->keyword() == FIELD || tokenizer_->keyword() == STATIC) {
            compileClassVarDec();
        } else {
            throw std::runtime_error("compileClass: should be method or field");
        }
    }

    fprintf(fp_, "</class>\n");
}

void CompilationEngine::compileClassVarDec() {
    fprintf(fp_, "<classVarDec>\n");
    if (tokenizer_->keyword() == FIELD) {
        fprintf(fp_, "<keyword>field</keyword>\n");
    } else if (tokenizer_->keyword() == STATIC) {
        fprintf(fp_, "<keyword>static</keyword>\n");
    } else {
        throw std::runtime_error("compileClassVarDec: should be field or static");
    }

    // type
    tokenizer_->advance();
    if (!is_type(tokenizer_->keyword()))
        throw std::runtime_error("compileClassVarDec: should be type");
    fprintf(fp_, "<keyword>%s</keyword>\n", tokenizer_->keyword_as_string().c_str());

    // variable names
    while (true) {
        tokenizer_->advance();
        if (tokenizer_->token_type() != IDENTIFIER)
            throw std::runtime_error("compileClassVarDec: should be variable name\n");
        fprintf(fp_, "<identifier>%s</identifier>\n", tokenizer_->identifier().c_str());

        tokenizer_->advance();
        if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ",") {
            fprintf(fp_, "<symbol>,</symbol>\n");
        } else if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ";") {
            fprintf(fp_, "<symbol>;</symbol>\n");
            break;
        } else {
            throw std::runtime_error("compileClassVarDec: should be symbol\n");
        }
    }

    fprintf(fp_, "</classVarDec>\n");
}

void CompilationEngine::compileSubroutine() {
    fprintf(fp_, "<subroutineDec>\n");

    if (tokenizer_->keyword() == METHOD) {
        fprintf(fp_, "<keyword>method</keyword>\n");
    } else if (tokenizer_->keyword() == CONSTRUCTOR) {
        fprintf(fp_, "<keyword>constructor</keyword>\n");
    }

    // type name
    tokenizer_->advance();
    if (tokenizer_->token_type() == KEYWORD && is_type(tokenizer_->keyword())) {
        fprintf(fp_, "<keyword>%s</keyword>\n", tokenizer_->keyword_as_string().c_str());
    } else if (tokenizer_->token_type() == IDENTIFIER) {
        fprintf(fp_, "<identifier>%s</identifier>\n", tokenizer_->identifier().c_str());
    } else {
        throw std::runtime_error("compileSubroutine: should be type name");
    }

    // method name
    tokenizer_->advance();
    if (tokenizer_->token_type() != IDENTIFIER)
        throw std::runtime_error("compileSubroutine: should be method name");
    fprintf(fp_, "<identifier>%s</identifier>\n", tokenizer_->identifier().c_str());

    // open bracket
    tokenizer_->advance();
    if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "("))
        throw std::runtime_error("compileSubroutine: should be (");
    fprintf(fp_, "<symbol>(</symbol>\n");

    // parameter list
    fprintf(fp_, "<parameterList>\n");
    compileParameterList();
    fprintf(fp_, "</parameterList>\n");

    // close bracket
    if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ")"))
        throw std::runtime_error("compileSubroutine: should be )");
    fprintf(fp_, "<symbol>)</symbol>\n");

    // body
    fprintf(fp_, "<subroutineBody>\n");

    // open curly brace
    tokenizer_->advance();
    if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "{"))
        throw std::runtime_error("compileSubroutine: should be {");
    fprintf(fp_, "<symbol>{</symbol>\n");

    while (true) {
        tokenizer_->advance();
        if (tokenizer_->token_type() == KEYWORD) {
            if (tokenizer_->keyword() == VAR) {
                compileVarDec();
            } else {
                compileStatements();
            }
        } else if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "}") {
            break;
        } else {
            throw std::runtime_error("compileSubroutine: invalid line");
        }
    }
    fprintf(fp_, "<symbol>}</symbol>\n");
    fprintf(fp_, "</subroutineBody>\n");

    fprintf(fp_, "</subroutineDec>\n");
}

void CompilationEngine::compileParameterList() {
    while (true) {
        tokenizer_->advance();
        if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ")")
            break;
        // type name
        if (tokenizer_->token_type() == KEYWORD && is_type(tokenizer_->keyword())) {
            fprintf(fp_, "<keyword>%s</keyword>\n", tokenizer_->keyword_as_string().c_str());
        } else if (tokenizer_->token_type() == IDENTIFIER) {
            fprintf(fp_, "<identifier>%s</identifier>\n", tokenizer_->identifier().c_str());
        } else {
            throw std::runtime_error("compileParameterList: should be type name");
        }

        // variable name
        tokenizer_->advance();
        if (tokenizer_->token_type() != IDENTIFIER)
            throw std::runtime_error("compileParameterList: should be variable name");
        fprintf(fp_, "<identifier>%s</identifier>\n", tokenizer_->identifier().c_str());


        tokenizer_->advance();
        if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ",") {
            fprintf(fp_, "<symbol>,</symbol\n");
        } else if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ")") {
            break;
        }
    }
}

void CompilationEngine::compileVarDec() {
    fprintf(fp_, "<varDec>\n");
    fprintf(fp_, "<keyword>var</keyword>");

    // type
    tokenizer_->advance();
    if (tokenizer_->token_type() == KEYWORD && is_type(tokenizer_->keyword())) {
        fprintf(fp_, "<keyword>%s</keyword>\n", tokenizer_->keyword_as_string().c_str());
    } else if (tokenizer_->token_type() == IDENTIFIER) {
        fprintf(fp_, "<identifier>%s</identifier>\n", tokenizer_->identifier().c_str());
    } else {
        throw std::runtime_error("compileVarDec: should be type name");
    }

    while (true) {
        tokenizer_->advance();
        if (tokenizer_->token_type() != IDENTIFIER)
            throw std::runtime_error("compileVarDec: should be variable name");
        fprintf(fp_, "<identifier>%s</identifier>\n", tokenizer_->identifier().c_str());

        tokenizer_->advance();
        if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ",") {
            fprintf(fp_, "<symbol>,</symbol\n");
        } else if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ";") {
            fprintf(fp_, "<symbol>;</symbol\n");
            break;
        } else {
            throw std::runtime_error("compileVarDec: should be symbol");
        }
    }

    fprintf(fp_, "</varDec>\n");
}

void CompilationEngine::compileStatements() {
    if (tokenizer_->token_type() != KEYWORD)
        throw std::runtime_error("compileStatements: should be keyword");

    fprintf(fp_, "<statements>\n");

    if (tokenizer_->keyword() == LET) {
        compileLet();
    } else if (tokenizer_->keyword() == IF) {
        compileIf();
    } else if (tokenizer_->keyword() == WHILE) {
        compileWhile();
    } else if (tokenizer_->keyword() == DO) {
        compileDo();
    }

    fprintf(fp_, "</statements>\n");
}

void CompilationEngine::compileDo() {
    fprintf(fp_, "<doStatement>\n");
    fprintf(fp_, "<keyword>do</keyword\n");

    // class name
    tokenizer_->advance();
    if (tokenizer_->token_type() != IDENTIFIER)
        throw std::runtime_error("compileDo: should be object");
    fprintf(fp_, "<identifier>%s</identifier>\n", tokenizer_->identifier().c_str());

    tokenizer_->advance();

    // dot
    tokenizer_->advance();
    if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "."))
        throw std::runtime_error("compileDo: should be .");
    fprintf(fp_, "<symbol>.</symbol>\n");

    // method name
    tokenizer_->advance();
    if (tokenizer_->token_type() != IDENTIFIER)
        throw std::runtime_error("compileDo: should be method name");
    fprintf(fp_, "<identifier>%s</identifier>\n", tokenizer_->identifier().c_str());

    // open bracket
    tokenizer_->advance();
    if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "("))
        throw std::runtime_error("compileDo: should be (");
    fprintf(fp_, "<symbol>(</symbol>\n");

    compileExpressionList();

    fprintf(fp_, "<symbol>)</symbol>\n");

    tokenizer_->advance();
    if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ";"))
        throw std::runtime_error("compileDo: should be ;");
    fprintf(fp_, "<symbol>;</symbol>\n");

    fprintf(fp_, "</doStatement>\n");
}

void CompilationEngine::compileLet() {
    fprintf(fp_, "<letStatement>\n");
    fprintf(fp_, "<keyword>let</keyword\n");

    // variable name
    tokenizer_->advance();
    if (tokenizer_->token_type() != IDENTIFIER)
        throw std::runtime_error("compileLet: should be variable name");
    fprintf(fp_, "<identifier>%s</identifier>\n", tokenizer_->identifier().c_str());

    // equal
    tokenizer_->advance();
    if (tokenizer_->token_type() != SYMBOL || tokenizer_->symbol() != "=")
        throw std::runtime_error("compileLet: should be =");
    fprintf(fp_, "<symbol>=</symbol>\n");

    compileExpression();

    fprintf(fp_, "<symbol>;</symbol>\n");
    fprintf(fp_, "</letStatement>\n");
}

void CompilationEngine::compileWhile() {

}

void CompilationEngine::compileReturn() {

}

void CompilationEngine::compileIf() {

}

void CompilationEngine::compileValueOrExpression(std::string terminal) {
    
}

void CompilationEngine::compileExpression() {
    fprintf(fp_, "<expression>\n");
    while (true) {
        tmp_token_type_ = tokenizer_->token_type();
        tokenizer_->advance();
    }
    fprintf(fp_, "</expression>\n");
}

void CompilationEngine::compileTerm() {
    // value
    tokenizer_->advance();
    tmp_token_type_ = tokenizer_->token_type();
    if (tmp_token_type_ == INT_CONST) {
        tmp_int_val_ = tokenizer_->int_val();
        tokenizer_->advance();
        if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ";") {
            fprintf(fp_, "<integerConstant>%d</integerConstant>\n", tmp_int_val_);
        } else {
            compileExpression();
        }
    } else if (tmp_token_type_ == STRING_CONST) {
        tmp_token_ = tokenizer_->string_val();
        tokenizer_->advance();
        if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ";") {
            fprintf(fp_, "<stringConstant>%s</stringConstant>\n", tmp_token_.c_str());
        } else {
            throw std::runtime_error("compileLet: should be ;");
        }
    } else if (tmp_token_type_ == IDENTIFIER) {
        tmp_token_ = tokenizer_->identifier();
        tokenizer_->advance();
        if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ";") {
            fprintf(fp_, "<identifier>%s</identifier>\n", tmp_token_.c_str());
        } else {
            compileExpression();
        }
    } else if (tmp_token_type_ == SYMBOL && tokenizer_->symbol() == "(") {
        tmp_token_ = tokenizer_->identifier();
        tokenizer_->advance();
        compileExpression();
    } else {
        throw std::runtime_error("compileLet: should be valid value");
    }
}

void CompilationEngine::compileExpressionList() {

}
