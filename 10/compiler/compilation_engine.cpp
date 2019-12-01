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
    tokenizer_->advance();
    while (tokenizer_->has_more_tokens()) {
        if (tokenizer_->token_type() != KEYWORD)
            break;
        if (tokenizer_->keyword() == METHOD || tokenizer_->keyword() == CONSTRUCTOR || tokenizer_->keyword() == FUNCTION) {
            compileSubroutine();
        } else if (tokenizer_->keyword() == FIELD || tokenizer_->keyword() == STATIC) {
            compileClassVarDec();
        } else {
            throw std::runtime_error("compileClass: should be method or field");
        }
    }

    // close curly brace
    if (tokenizer_->token_type() != SYMBOL || tokenizer_->symbol() != "}")
        throw std::runtime_error("compileClass: should be }");
    fprintf(fp_, "<symbol>}</symbol>\n");

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
    if (tokenizer_->token_type() == IDENTIFIER)
        fprintf(fp_, "<identifier>%s</identifier>\n", tokenizer_->identifier().c_str());
    else if (tokenizer_->token_type() == KEYWORD && is_type(tokenizer_->keyword()))
        fprintf(fp_, "<keyword>%s</keyword>\n", tokenizer_->keyword_as_string().c_str());
    else
        throw std::runtime_error("compileClassVarDec: should be type");

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
    tokenizer_->advance();
}

void CompilationEngine::compileSubroutine() {
    fprintf(fp_, "<subroutineDec>\n");

    if (tokenizer_->keyword() == METHOD)
        fprintf(fp_, "<keyword>method</keyword>\n");
    else if (tokenizer_->keyword() == CONSTRUCTOR)
        fprintf(fp_, "<keyword>constructor</keyword>\n");
    else if (tokenizer_->keyword() == FUNCTION)
        fprintf(fp_, "<keyword>function</keyword>\n");

    // type name
    tokenizer_->advance();
    if (tokenizer_->token_type() == KEYWORD && is_type(tokenizer_->keyword()))
        fprintf(fp_, "<keyword>%s</keyword>\n", tokenizer_->keyword_as_string().c_str());
    else if (tokenizer_->token_type() == IDENTIFIER)
        fprintf(fp_, "<identifier>%s</identifier>\n", tokenizer_->identifier().c_str());
    else
        throw std::runtime_error("compileSubroutine: should be type name");

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
        if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "}")
            break;
    }
    fprintf(fp_, "<symbol>}</symbol>\n");
    fprintf(fp_, "</subroutineBody>\n");

    fprintf(fp_, "</subroutineDec>\n");
    tokenizer_->advance();
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
            fprintf(fp_, "<symbol>,</symbol>\n");
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
    if (tokenizer_->token_type() == KEYWORD && is_type(tokenizer_->keyword()))
        fprintf(fp_, "<keyword>%s</keyword>\n", tokenizer_->keyword_as_string().c_str());
    else if (tokenizer_->token_type() == IDENTIFIER)
        fprintf(fp_, "<identifier>%s</identifier>\n", tokenizer_->identifier().c_str());
    else
        throw std::runtime_error("compileVarDec: should be type name");

    while (true) {
        tokenizer_->advance();
        if (tokenizer_->token_type() != IDENTIFIER)
            throw std::runtime_error("compileVarDec: should be variable name");
        fprintf(fp_, "<identifier>%s</identifier>\n", tokenizer_->identifier().c_str());

        tokenizer_->advance();
        if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ",") {
            fprintf(fp_, "<symbol>,</symbol>\n");
        } else if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ";") {
            fprintf(fp_, "<symbol>;</symbol>\n");
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

    while (true) {
        if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "}")
            break;
        if (tokenizer_->token_type() != KEYWORD)
            throw std::runtime_error("compileStatements: should be keyword");
        if (tokenizer_->keyword() == LET)
            compileLet();
        else if (tokenizer_->keyword() == IF)
            compileIf();
        else if (tokenizer_->keyword() == WHILE)
            compileWhile();
        else if (tokenizer_->keyword() == DO)
            compileDo();
        else if (tokenizer_->keyword() == RETURN)
            compileReturn();
    }

    fprintf(fp_, "</statements>\n");
}

void CompilationEngine::compileDo() {
    fprintf(fp_, "<doStatement>\n");
    fprintf(fp_, "<keyword>do</keyword>\n");

    tokenizer_->advance();
    std::string identifier = tokenizer_->identifier();
    tokenizer_->advance();
    compileSubroutineCall(identifier);

    if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ";"))
        throw std::runtime_error("compileDo: should be ;");
    fprintf(fp_, "<symbol>;</symbol>\n");

    fprintf(fp_, "</doStatement>\n");
    tokenizer_->advance();
}

void CompilationEngine::compileLet() {
    fprintf(fp_, "<letStatement>\n");
    fprintf(fp_, "<keyword>let</keyword>\n");

    // variable name
    tokenizer_->advance();
    if (tokenizer_->token_type() != IDENTIFIER)
        throw std::runtime_error("compileLet: should be variable name");
    fprintf(fp_, "<identifier>%s</identifier>\n", tokenizer_->identifier().c_str());

    tokenizer_->advance();
    // array access
    if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "[") {
        fprintf(fp_, "<symbol>[</symbol>\n");
        tokenizer_->advance();
        compileExpression();
        if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "]"))
            throw std::runtime_error("compileLet: should be ]");
        fprintf(fp_, "<symbol>]</symbol>\n");
        tokenizer_->advance();
    }

    // equal
    if (tokenizer_->token_type() != SYMBOL || tokenizer_->symbol() != "=")
        throw std::runtime_error("compileLet: should be =");
    fprintf(fp_, "<symbol>=</symbol>\n");

    tokenizer_->advance();
    compileExpression();

    if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ";"))
        throw std::runtime_error("compileLet: should be ;");
    fprintf(fp_, "<symbol>;</symbol>\n");
    fprintf(fp_, "</letStatement>\n");
    tokenizer_->advance();
}

void CompilationEngine::compileWhile() {
    fprintf(fp_, "<whileStatement>\n");
    fprintf(fp_, "<keyword>while</keyword>\n");

    // open bracket
    tokenizer_->advance();
    if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "("))
        throw std::runtime_error("compileWhile: should be (");
    fprintf(fp_, "<symbol>(</symbol>\n");

    tokenizer_->advance();
    compileExpression();

    // close bracket
    if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ")"))
        throw std::runtime_error("compileWhile: should be )");
    fprintf(fp_, "<symbol>)</symbol>\n");

    // open curly brace
    tokenizer_->advance();
    if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "{"))
        throw std::runtime_error("compileWhile: should be {");
    fprintf(fp_, "<symbol>{</symbol>\n");

    tokenizer_->advance();
    compileStatements();

    // close curly brace
    if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "}"))
        throw std::runtime_error("compileWhile: should be }");
    fprintf(fp_, "<symbol>}</symbol>\n");

    fprintf(fp_, "</whileStatement>\n");
    tokenizer_->advance();
}

void CompilationEngine::compileReturn() {
    fprintf(fp_, "<returnStatement>\n");
    fprintf(fp_, "<keyword>return</keyword>\n");

    tokenizer_->advance();
    compileExpression();

    if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ";"))
        throw std::runtime_error("compileReturn: should be ;");
    fprintf(fp_, "<symbol>;</symbol>\n");

    fprintf(fp_, "</returnStatement>\n");
    tokenizer_->advance();
}

void CompilationEngine::compileIf() {
    fprintf(fp_, "<ifStatement>\n");
    fprintf(fp_, "<keyword>if</keyword>\n");

    // open bracket
    tokenizer_->advance();
    if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "("))
        throw std::runtime_error("compileIf: should be (");
    fprintf(fp_, "<symbol>(</symbol>\n");

    tokenizer_->advance();
    compileExpression();

    // close bracket
    if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ")"))
        throw std::runtime_error("compileIf: should be )");
    fprintf(fp_, "<symbol>)</symbol>\n");

    // open curly brace
    tokenizer_->advance();
    if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "{"))
        throw std::runtime_error("compileIf: should be {");
    fprintf(fp_, "<symbol>{</symbol>\n");

    tokenizer_->advance();
    compileStatements();

    // close curly brace
    if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "}"))
        throw std::runtime_error("compileIf: should be }");
    fprintf(fp_, "<symbol>}</symbol>\n");

    // else
    tokenizer_->advance();
    if (tokenizer_->token_type() == KEYWORD && tokenizer_->keyword() == ELSE) {
        fprintf(fp_, "<keyword>else</keyword>\n");

        // open curly brace
        tokenizer_->advance();
        if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "{"))
            throw std::runtime_error("compileIf: (else) should be {");
        fprintf(fp_, "<symbol>{</symbol>\n");

        tokenizer_->advance();
        compileStatements();

        // close curly brace
        if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "}"))
            throw std::runtime_error("compileIf: (else) shoule be }");
        fprintf(fp_, "<symbol>}</symbol>\n");

        tokenizer_->advance();
    }

    fprintf(fp_, "</ifStatement>\n");
}

void CompilationEngine::compileExpression() {
    if (tokenizer_->token_type() == SYMBOL && (tokenizer_->symbol() == ";" || tokenizer_->symbol() == ")" || tokenizer_->symbol() == ","))
        return;

    fprintf(fp_, "<expression>\n");
    bool first = true;
    while (true) {
        if (!first)
            tokenizer_->advance();
        compileTerm();
        if (tokenizer_->token_type() == SYMBOL) {
            bool done = false;
            switch (tokenizer_->symbol().at(0)) {
                case '+':
                case '-':
                case '*':
                case '/':
                case '&':
                case '|':
                case '<':
                case '>':
                case '=':
                    fprintf(fp_, "<symbol>%s</symbol>\n", tokenizer_->symbol().c_str());
                    break;
                default:
                    done = true;
                    break;
            }
            if (done)
                break;
        } else {
            break;
        }
        first = false;
    }
    fprintf(fp_, "</expression>\n");
}

void CompilationEngine::compileTerm() {
    // value
    if (tokenizer_->token_type() == SYMBOL && (tokenizer_->symbol() == ";" || tokenizer_->symbol() == ")" || tokenizer_->symbol() == ","))
        return;

    fprintf(fp_, "<term>\n");
    int tmp_token_type = tokenizer_->token_type();

    if (tmp_token_type == INT_CONST) {
        fprintf(fp_, "<integerConstant>\n");
        fprintf(fp_, "%d\n", tokenizer_->int_val());
        fprintf(fp_, "</integerConstant>\n");
        tokenizer_->advance();
    } else if (tmp_token_type == STRING_CONST) {
        fprintf(fp_, "<stringConstant>\n");
        fprintf(fp_, "%s\n", tokenizer_->string_val().c_str());
        fprintf(fp_, "</stringConstant>\n");
        tokenizer_->advance();
    } else if (tmp_token_type == KEYWORD) {
        fprintf(fp_, "<keywordConstant>\n");
        switch (tokenizer_->keyword()) {
            case TRUE:
                fprintf(fp_, "true\n");
                break;
            case FALSE:
                fprintf(fp_, "false\n");
                break;
            case _NULL:
                fprintf(fp_, "null\n");
                break;
            case THIS:
                fprintf(fp_, "this\n");
                break;
            default:
                throw std::runtime_error("compileTerm: invalid keyword");
        }
        fprintf(fp_, "</keywordConstant>\n");
        tokenizer_->advance();
    } else if (tmp_token_type == IDENTIFIER) {
        std::string identifier = tokenizer_->identifier();
        tokenizer_->advance();
        int token_type = tokenizer_->token_type();
        if (token_type == SYMBOL && tokenizer_->symbol() == "[") {
            // varName[expression]
            fprintf(fp_, "<identifier>%s</identifier>\n", identifier.c_str());
            fprintf(fp_, "<symbol>[</symbol>\n");
            tokenizer_->advance();
            compileExpression();
            fprintf(fp_, "<symbol>]</symbol>\n");
            tokenizer_->advance();
        } else if (token_type == SYMBOL && (tokenizer_->symbol() == "(" || tokenizer_->symbol() == ".")) {
            // subroutineCall
            compileSubroutineCall(identifier);
        } else {
            // varName
            fprintf(fp_, "<identifier>%s</identifier>\n", identifier.c_str());
        }
    } else if (tmp_token_type == SYMBOL) {
        if (tokenizer_->symbol() == "(") {
            // (expression)
            fprintf(fp_, "<symbol>(</symbol>\n");
            tokenizer_->advance();
            compileExpression();
            fprintf(fp_, "<symbol>)</symbol>\n");
            tokenizer_->advance();
        } else if (tokenizer_->symbol() == "-" || tokenizer_->symbol() == "~") {
            // unaryOp term
            fprintf(fp_, "<symbol>%s</symbol>\n", tokenizer_->symbol().c_str());
            tokenizer_->advance();
            compileTerm();
        } else {
            throw std::runtime_error("compileTerm: should be - or ~");
        }
    } else {
        throw std::runtime_error("compileTerm: invalid");
    }

    fprintf(fp_, "</term>\n");
}

void CompilationEngine::compileExpressionList() {
    fprintf(fp_, "<expressionList>\n");
    while (true) {
        if (tokenizer_->token_type() == SYMBOL && (tokenizer_->symbol() == ")" || tokenizer_->symbol() == ";"))
            break;
        compileExpression();
        if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ",") {
            fprintf(fp_, "<symbol>,</symbol>\n");
            tokenizer_->advance();
        }
    }
    fprintf(fp_, "</expressionList>\n");
}

void CompilationEngine::compileSubroutineCall(std::string identifier) {
    fprintf(fp_, "<subroutineCall>\n");
    fprintf(fp_, "<identifier>%s</identifier>\n", identifier.c_str());

    if (tokenizer_->symbol() == "(") {
        fprintf(fp_, "<symbol>(</symbol>\n");
        tokenizer_->advance();
        compileExpressionList();
        fprintf(fp_, "<symbol>)</symbol>\n");
    } else if (tokenizer_->symbol() == ".") {
        fprintf(fp_, "<symbol>.</symbol>\n");

        tokenizer_->advance();
        if (tokenizer_->token_type() != IDENTIFIER)
            throw std::runtime_error("compileSubroutineCall: should be identifier");
        fprintf(fp_, "<identifier>%s</identifier>\n", tokenizer_->identifier().c_str());

        tokenizer_->advance();
        if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "("))
            throw std::runtime_error("compileSubroutineCall: should be (");
        fprintf(fp_, "<symbol>(</symbol>\n");

        tokenizer_->advance();
        compileExpressionList();

        fprintf(fp_, "<symbol>)</symbol>\n");
    } else {
        throw std::runtime_error("compileSubroutineCall: invalid");
    }

    tokenizer_->advance();
    fprintf(fp_, "</subroutineCall>\n");
}
