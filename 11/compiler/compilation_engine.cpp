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
    writer_ = new VMWriter(path);
    table_ = new SymbolTable();
    if ((fp_ = fopen(path, "w")) == NULL)
        throw std::runtime_error("failed to open output file");
    while_count_ = 0;
    if_count_ = 0;
}

void CompilationEngine::compileClass() {
    tokenizer_->advance();
    if (tokenizer_->keyword() != CLASS)
        throw std::runtime_error("compileClass: invalid keyword");

    // class name
    tokenizer_->advance();
    if (tokenizer_->token_type() != IDENTIFIER)
        throw std::runtime_error("compileClass: should be class name");
    class_name_ = tokenizer_->identifier();

    // open curly brace
    tokenizer_->advance();
    if (tokenizer_->token_type() != SYMBOL || tokenizer_->symbol() != "{")
        throw std::runtime_error("compileClass: should be {");

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
    if (tokenizer_->symbol() != "}")
        throw std::runtime_error("compileClass: should be }");
}

void CompilationEngine::compileClassVarDec() {
    if (tokenizer_->keyword() != FIELD && tokenizer_->keyword() != STATIC)
        throw std::runtime_error("compileClassVarDec: should be field or static");
    int kind = tokenizer_->keyword() == FIELD ? TB_FIELD : TB_STATIC;

    // type
    tokenizer_->advance();
    std::string type;
    if (tokenizer_->token_type() == IDENTIFIER)
        type = tokenizer_->identifier();
    else if (is_type(tokenizer_->keyword()))
        type = tokenizer_->keyword_as_string();
    else
        throw std::runtime_error("compileClassVarDec: should be type");

    // variable names
    while (true) {
        tokenizer_->advance();
        if (tokenizer_->token_type() != IDENTIFIER)
            throw std::runtime_error("compileClassVarDec: should be variable name\n");
        std::string variable_name = tokenizer_->identifier();

        table_->define(variable_name, type, kind);

        tokenizer_->advance();
        if (tokenizer_->symbol() == ";")
            break;
        else if (tokenizer_->symbol() != ",")
            throw std::runtime_error("compileClassVarDec: should be symbol\n");
    }

    tokenizer_->advance();
}

void CompilationEngine::compileSubroutine() {
    table_->startSubroutine();

    int subroutine_type = tokenizer_->keyword();
    if (subroutine_type != METHOD && subroutine_type != CONSTRUCTOR && subroutine_type != FUNCTION)
        throw std::runtime_error("compileSubroutine: invalid subroutine type");

    // type name
    tokenizer_->advance();
    std::string type;
    if (tokenizer_->token_type() == KEYWORD && is_type(tokenizer_->keyword()))
        type = tokenizer_->keyword_as_string();
    else if (tokenizer_->token_type() == IDENTIFIER)
        type = tokenizer_->identifier();
    else
        throw std::runtime_error("compileSubroutine: should be type name");

    // method name
    tokenizer_->advance();
    if (tokenizer_->token_type() != IDENTIFIER)
        throw std::runtime_error("compileSubroutine: should be method name");
    std::string subroutine_name = tokenizer_->identifier();

    // open bracket
    tokenizer_->advance();
    if (tokenizer_->symbol() != "(")
        throw std::runtime_error("compileSubroutine: should be (");

    // parameter list
    compileParameterList();

    // close bracket
    if (tokenizer_->symbol() != ")")
        throw std::runtime_error("compileSubroutine: should be )");

    // open curly brace
    tokenizer_->advance();
    if (tokenizer_->symbol() != "{")
        throw std::runtime_error("compileSubroutine: should be {");

    bool is_statement_started = false;
    while (true) {
        tokenizer_->advance();
        if (tokenizer_->token_type() == KEYWORD) {
            if (tokenizer_->keyword() == VAR) {
                compileVarDec();
            } else {
                // assume the first statement comes after all variable declarations
                if (!is_statement_started) {
                    int n_locals = table_->varCount(TB_VAR);
                    writer_->writeFunction(class_name_ + "." + subroutine_name, n_locals);
                }
                compileStatements();
                is_statement_started = true;
            }
        } else if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "}") {
            break;
        } else {
            throw std::runtime_error("compileSubroutine: invalid line");
        }
        if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "}")
            break;
    }

    tokenizer_->advance();
}

void CompilationEngine::compileParameterList() {
    while (true) {
        tokenizer_->advance();
        if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ")")
            break;
        // type name
        std::string type;
        if (tokenizer_->token_type() == KEYWORD && is_type(tokenizer_->keyword()))
            type = tokenizer_->keyword_as_string();
        else if (tokenizer_->token_type() == IDENTIFIER)
            type = tokenizer_->identifier();
        else
            throw std::runtime_error("compileParameterList: should be type name");

        // variable name
        tokenizer_->advance();
        if (tokenizer_->token_type() != IDENTIFIER)
            throw std::runtime_error("compileParameterList: should be variable name");
        std::string variable_name = tokenizer_->identifier();

        table_->define(variable_name, type, TB_ARG);

        tokenizer_->advance();
        if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ")")
            break;
        else if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ","))
            throw std::runtime_error("compileParameterList: should be ,");
    }
}

void CompilationEngine::compileVarDec() {
    // type
    tokenizer_->advance();
    std::string type;
    if (tokenizer_->token_type() == KEYWORD && is_type(tokenizer_->keyword()))
        type = tokenizer_->keyword_as_string();
    else if (tokenizer_->token_type() == IDENTIFIER)
        type = tokenizer_->identifier();
    else
        throw std::runtime_error("compileVarDec: should be type name");

    while (true) {
        tokenizer_->advance();
        if (tokenizer_->token_type() != IDENTIFIER)
            throw std::runtime_error("compileVarDec: should be variable name");
        std::string variable_name = tokenizer_->identifier();

        table_->define(variable_name, type, TB_VAR);

        tokenizer_->advance();
        if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ";")
            break;
        else if (tokenizer_->symbol() == ",")
            throw std::runtime_error("compileVarDec: should be ,");
    }
}

void CompilationEngine::compileStatements() {
    if (tokenizer_->token_type() != KEYWORD)
        throw std::runtime_error("compileStatements: should be keyword");

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
}

void CompilationEngine::compileDo() {
    tokenizer_->advance();
    std::string identifier = tokenizer_->identifier();
    tokenizer_->advance();
    compileSubroutineCall(identifier);

    if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ";"))
        throw std::runtime_error("compileDo: should be ;");

    tokenizer_->advance();
}

void CompilationEngine::compileLet() {
    // variable name
    tokenizer_->advance();
    if (tokenizer_->token_type() != IDENTIFIER)
        throw std::runtime_error("compileLet: should be variable name");
    std::string variable_name = tokenizer_->identifier();

    int kind = table_->kindOf(variable_name);
    std::string type = table_->typeOf(variable_name);
    int index = table_->indexOf(variable_name);

    int segment;
    if (kind == TB_STATIC)
        segment = VM_STATIC;
    else if (kind == TB_FIELD)
        segment = VM_THIS;
    else if (kind == TB_ARG)
        segment = VM_ARG;
    else if (kind == TB_VAR)
        segment = VM_LOCAL;
    else
        throw std::runtime_error("compileLet: invalid kind");

    tokenizer_->advance();
    // array access
    bool is_array = false;
    if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "[") {
        tokenizer_->advance();
        compileExpression();
        if (!(tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == "]"))
            throw std::runtime_error("compileLet: should be ]");
        tokenizer_->advance();
        // get address
        writer_->writePush(segment, index);
        // add index
        writer_->writeArithmetic(ADD);
        // set address to temp
        writer_->writePop(VM_TEMP, 0);
        is_array = true;
    }

    // equal
    if (tokenizer_->symbol() != "=")
        throw std::runtime_error("compileLet: should be =");

    tokenizer_->advance();
    compileExpression();

    if (tokenizer_->symbol() != ";")
        throw std::runtime_error("compileLet: should be ;");
    tokenizer_->advance();

    if (is_array) {
        writer_->writePush(VM_TEMP, 0);
        writer_->writePop(VM_POINTER, 1);
        writer_->writePop(VM_THAT, 0);
    } else {
        writer_->writePop(segment, index);
    }
}

void CompilationEngine::compileWhile() {
    // open bracket
    tokenizer_->advance();
    if (tokenizer_->symbol() != "(")
        throw std::runtime_error("compileWhile: should be (");

    writer_->writeLabel("WhileStart" + std::to_string(while_count_));

    tokenizer_->advance();
    compileExpression();

    writer_->writeIf("WhileContinue" + std::to_string(while_count_));

    // close bracket
    if (tokenizer_->symbol() != ")")
        throw std::runtime_error("compileWhile: should be )");

    // open curly brace
    tokenizer_->advance();
    if (tokenizer_->symbol() != "{")
        throw std::runtime_error("compileWhile: should be {");

    writer_->writeGoto("WhileEnd" + std::to_string(while_count_));

    writer_->writeLabel("WhileContinue" + std::to_string(while_count_));

    tokenizer_->advance();
    compileStatements();

    writer_->writeGoto("WhileStart" + std::to_string(while_count_));

    // close curly brace
    if (tokenizer_->symbol() != "}")
        throw std::runtime_error("compileWhile: should be }");

    tokenizer_->advance();

    writer_->writeLabel("WhileEnd" + std::to_string(while_count_));

    ++while_count_;
}

void CompilationEngine::compileReturn() {
    tokenizer_->advance();
    compileExpression();

    if (tokenizer_->symbol() != ";")
        throw std::runtime_error("compileReturn: should be ;");

    tokenizer_->advance();

    writer_->writeReturn();
}

void CompilationEngine::compileIf() {
    // open bracket
    tokenizer_->advance();
    if (tokenizer_->symbol() != "(")
        throw std::runtime_error("compileIf: should be (");

    tokenizer_->advance();
    compileExpression();

    // close bracket
    if (tokenizer_->symbol() != ")")
        throw std::runtime_error("compileIf: should be )");

    writer_->writeIf("IfContinue" + std::to_string(if_count_));

    writer_->writeGoto("IfEnd" + std::to_string(if_count_));

    // open curly brace
    tokenizer_->advance();
    if (tokenizer_->symbol() != "{")
        throw std::runtime_error("compileIf: should be {");

    writer_->writeLabel("IfContinue" + std::to_string(if_count_));

    tokenizer_->advance();
    compileStatements();

    // close curly brace
    if (tokenizer_->symbol() != "}")
        throw std::runtime_error("compileIf: should be }");

    // else
    tokenizer_->advance();
    if (tokenizer_->token_type() == KEYWORD && tokenizer_->keyword() == ELSE) {
        // open curly brace
        tokenizer_->advance();
        if (!tokenizer_->symbol() == "{")
            throw std::runtime_error("compileIf: (else) should be {");

        writer_->writeLabel("IfEnd" + std::to_string(if_count_));

        tokenizer_->advance();
        compileStatements();

        // close curly brace
        if (tokenizer_->symbol() != "}")
            throw std::runtime_error("compileIf: (else) shoule be }");

        tokenizer_->advance();
    }

    writer_->writeLabel("IfEnd" + std::to_string(if_count_));
    ++if_count_;
}

void CompilationEngine::compileExpression() {
    if (tokenizer_->token_type() == SYMBOL)
        if (tokenizer_->symbol() == ";" || tokenizer_->symbol() == ")" || tokenizer_->symbol() == ",")
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
        fprintf(fp_, "<integerConstant>%d</integerConstant>\n", tokenizer_->int_val());
        tokenizer_->advance();
    } else if (tmp_token_type == STRING_CONST) {
        fprintf(fp_, "<stringConstant>%s</stringConstant>\n", tokenizer_->string_val().c_str());
        tokenizer_->advance();
    } else if (tmp_token_type == KEYWORD) {
        switch (tokenizer_->keyword()) {
            case TRUE:
            case FALSE:
            case _NULL:
            case THIS:
                break;
            default:
                throw std::runtime_error("compileTerm: invalid keyword");
        }
        fprintf(fp_, "<keywordConstant>%s</keywordConstant>\n", tokenizer_->keyword_as_string().c_str());
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

int CompilationEngine::compileExpressionList() {
    int n_args = 0;
    while (true) {
        if (tokenizer_->token_type() == SYMBOL && (tokenizer_->symbol() == ")" || tokenizer_->symbol() == ";"))
            break;
        compileExpression();
        ++n_args;
        if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ",")
            tokenizer_->advance();
    }
    return n_args;
}

void CompilationEngine::compileSubroutineCall(std::string identifier) {
    int n_args = 1;
    writer_->writePush(VM_THIS, 0);

    if (tokenizer_->symbol() == "(") {
        tokenizer_->advance();
        n_args = compileExpressionList();
    } else if (tokenizer_->symbol() == ".") {
        tokenizer_->advance();
        if (tokenizer_->token_type() != IDENTIFIER)
            throw std::runtime_error("compileSubroutineCall: should be identifier");
        identifier += ".";
        identifier += tokenizer_->identifier();

        tokenizer_->advance();
        if (tokenizer_->symbol() != "(")
            throw std::runtime_error("compileSubroutineCall: should be (");

        tokenizer_->advance();
        n_args = compileExpressionList();
    } else {
        throw std::runtime_error("compileSubroutineCall: invalid");
    }

    writer_->writeCall(identifier, n_args);

    tokenizer_->advance();
}
