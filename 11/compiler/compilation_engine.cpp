#include "compilation_engine.h"

bool is_type(int keyword) {
    if (keyword == INT) return true;
    else if (keyword == BOOLEAN) return true;
    else if (keyword == CHAR) return true;
    else if (keyword == VOID) return true;
    else return false;
}

void CompilationEngine::writePushWithVar(std::string identifier) {
    int index = table_->indexOf(identifier);
    switch (table_->kindOf(identifier)) {
        case TB_VAR:
            writer_->writePush(VM_LOCAL, index);
            break;
        case TB_ARG:
            writer_->writePush(VM_ARG, index);
            break;
        case TB_FIELD:
            writer_->writePush(VM_THIS, index);
            break;
        case TB_STATIC:
            writer_->writePush(VM_STATIC, index);
            break;
        default:
            throw std::runtime_error("writePushWithVar: invalid kind");
    }
}

CompilationEngine::CompilationEngine(JackTokenizer *tokenizer, const char* path) : while_count_(0), if_count_(0) {
    tokenizer_ = tokenizer;
    writer_ = new VMWriter(path);
    table_ = new SymbolTable();
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

    while (true) {
        tokenizer_->advance();
        if (tokenizer_->token_type() == KEYWORD) {
            if (tokenizer_->keyword() == VAR) {
                compileVarDec();
            } else {
                int n_locals = table_->varCount(TB_VAR);
                writer_->writeFunction(class_name_ + "." + subroutine_name, n_locals);
                if (subroutine_type == CONSTRUCTOR) {
                    // memory size
                    writer_->writePush(VM_CONST, table_->varCount(TB_FIELD));
                    // memory allocation for constructor
                    writer_->writeCall("Memory.alloc", 1);
                    // set allocated memory to THIS
                    writer_->writePop(VM_POINTER, 0);
                } else if (subroutine_type == METHOD) {
                    // the first argument is THIS address
                    writer_->writePush(VM_ARG, 0);
                    // set pointer to THIS
                    writer_->writePop(VM_POINTER, 0);
                }
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
        else if (tokenizer_->symbol() != ",")
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

    // drop returned value
    writer_->writePop(VM_TEMP, 0);

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
        // prerve result
        writer_->writePop(VM_TEMP, 0);
        // set pointer address
        writer_->writePop(VM_POINTER, 1);
        // restore result
        writer_->writePush(VM_TEMP, 0);
        // store result
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

    int while_count = while_count_++;

    writer_->writeLabel("WhileStart" + std::to_string(while_count));

    tokenizer_->advance();
    compileExpression();

    writer_->writeIf("WhileContinue" + std::to_string(while_count));
    writer_->writeGoto("WhileEnd" + std::to_string(while_count));
    writer_->writeLabel("WhileContinue" + std::to_string(while_count));

    // close bracket
    if (tokenizer_->symbol() != ")")
        throw std::runtime_error("compileWhile: should be )");

    // open curly brace
    tokenizer_->advance();
    if (tokenizer_->symbol() != "{")
        throw std::runtime_error("compileWhile: should be {");

    tokenizer_->advance();
    compileStatements();

    writer_->writeGoto("WhileStart" + std::to_string(while_count));

    // close curly brace
    if (tokenizer_->symbol() != "}")
        throw std::runtime_error("compileWhile: should be }");

    tokenizer_->advance();

    writer_->writeLabel("WhileEnd" + std::to_string(while_count));
}

void CompilationEngine::compileReturn() {
    tokenizer_->advance();

    // void
    if (tokenizer_->token_type() == SYMBOL && tokenizer_->symbol() == ";")
        writer_->writePush(VM_CONST, 0);
    else
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

    int if_count = if_count_++;
    writer_->writeIf("IfContinue" + std::to_string(if_count));
    writer_->writeGoto("IfEnd" + std::to_string(if_count));
    writer_->writeLabel("IfContinue" + std::to_string(if_count));

    // open curly brace
    tokenizer_->advance();
    if (tokenizer_->symbol() != "{")
        throw std::runtime_error("compileIf: should be {");

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
        if (tokenizer_->symbol() != "{")
            throw std::runtime_error("compileIf: (else) should be {");

        writer_->writeGoto("ElseEnd" + std::to_string(if_count));
        writer_->writeLabel("IfEnd" + std::to_string(if_count));

        tokenizer_->advance();
        compileStatements();

        // close curly brace
        if (tokenizer_->symbol() != "}")
            throw std::runtime_error("compileIf: (else) shoule be }");

        writer_->writeLabel("ElseEnd" + std::to_string(if_count));

        tokenizer_->advance();
    } else {
        writer_->writeLabel("IfEnd" + std::to_string(if_count));
    }
}

void CompilationEngine::compileExpression() {
    if (tokenizer_->token_type() == SYMBOL)
        if (tokenizer_->symbol() == ";" || tokenizer_->symbol() == ")" || tokenizer_->symbol() == ",")
            return;

    bool first = true;
    char prev_op = '\0';
    while (true) {
        if (!first)
            tokenizer_->advance();
        compileTerm();
        switch (prev_op) {
            case '+':
                writer_->writeArithmetic(ADD);
                break;
            case '-':
                writer_->writeArithmetic(SUB);
                break;
            case '*':
                writer_->writeCall("Math.multiply", 2);
                break;
            case '/':
                writer_->writeCall("Math.divide", 2);
                break;
            case '&':
                writer_->writeArithmetic(AND);
                break;
            case '|':
                writer_->writeArithmetic(OR);
                break;
            case '<':
                writer_->writeArithmetic(LT);
                break;
            case '>':
                writer_->writeArithmetic(GT);
                break;
            case '=':
                writer_->writeArithmetic(EQ);
                break;
        }
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
                    prev_op = tokenizer_->symbol().c_str()[0];
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
}

void CompilationEngine::compileTerm() {
    // value
    if (tokenizer_->token_type() == SYMBOL)
        if (tokenizer_->symbol() == ";" || tokenizer_->symbol() == ")" || tokenizer_->symbol() == ",")
            return;

    int tmp_token_type = tokenizer_->token_type();

    if (tmp_token_type == INT_CONST) {
        writer_->writePush(VM_CONST, tokenizer_->int_val());
        tokenizer_->advance();
    } else if (tmp_token_type == STRING_CONST) {
        std::string str = tokenizer_->string_val();
        writer_->writePush(VM_CONST, str.size());
        writer_->writeCall("String.new", 1);
        writer_->writePop(VM_TEMP, 1);
        for (int i = 0; i < str.size(); ++i) {
            writer_->writePush(VM_TEMP, 1);
            writer_->writePush(VM_CONST, str.at(i));
            writer_->writeCall("String.appendChar", 2);
            // drop returned value
            writer_->writePop(VM_TEMP, 2);
        }
        writer_->writePush(VM_TEMP, 1);
        tokenizer_->advance();
    } else if (tmp_token_type == KEYWORD) {
        switch (tokenizer_->keyword()) {
            case TRUE:
                writer_->writePush(VM_CONST, 0);
                writer_->writeArithmetic(NOT);
                break;
            case FALSE:
            case _NULL:
                writer_->writePush(VM_CONST, 0);
                break;
            case THIS:
                writer_->writePush(VM_POINTER, 0);
                break;
            default:
                throw std::runtime_error("compileTerm: invalid keyword");
        }
        tokenizer_->advance();
    } else if (tmp_token_type == IDENTIFIER) {
        std::string identifier = tokenizer_->identifier();
        tokenizer_->advance();
        int token_type = tokenizer_->token_type();
        if (token_type == SYMBOL && tokenizer_->symbol() == "[") {
            // varName[expression]
            tokenizer_->advance();
            compileExpression();
            tokenizer_->advance();
            writePushWithVar(identifier);
            writer_->writeArithmetic(ADD);
            writer_->writePop(VM_POINTER, 1);
            writer_->writePush(VM_THAT, 0);
        } else if (token_type == SYMBOL && (tokenizer_->symbol() == "(" || tokenizer_->symbol() == ".")) {
            // subroutineCall
            compileSubroutineCall(identifier);
        } else {
            // varName
            writePushWithVar(identifier);
        }
    } else if (tmp_token_type == SYMBOL) {
        if (tokenizer_->symbol() == "(") {
            // (expression)
            tokenizer_->advance();
            compileExpression();
            tokenizer_->advance();
        } else if (tokenizer_->symbol() == "-" || tokenizer_->symbol() == "~") {
            // unaryOp term
            char op = tokenizer_->symbol().c_str()[0];
            tokenizer_->advance();
            compileTerm();
            switch (op) {
                case '-':
                    writer_->writeArithmetic(NEG);
                    break;
                case '~':
                    writer_->writeArithmetic(NOT);
                    break;
                default:
                    throw std::runtime_error("compileTerm: invalid unary op");
            }
        } else {
            throw std::runtime_error("compileTerm: should be - or ~");
        }
    } else {
        throw std::runtime_error("compileTerm: invalid");
    }
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
    std::string function_name;

    if (tokenizer_->symbol() == "(") {
        tokenizer_->advance();
        writer_->writePush(VM_POINTER, 0);
        n_args += compileExpressionList();
        function_name = class_name_ + "." + identifier;
    } else if (tokenizer_->symbol() == ".") {
        tokenizer_->advance();
        if (tokenizer_->token_type() != IDENTIFIER)
            throw std::runtime_error("compileSubroutineCall: should be identifier");
        int kind = table_->kindOf(identifier);
        if (kind == TB_NONE) {
            // Function call
            function_name = identifier + "." + tokenizer_->identifier();
            n_args = 0;
        } else {
            // Method call
            std::string type = table_->typeOf(identifier);
            function_name = type + "." + tokenizer_->identifier();
            // set this pointer
            writePushWithVar(identifier);
        }

        tokenizer_->advance();
        if (tokenizer_->symbol() != "(")
            throw std::runtime_error("compileSubroutineCall: should be (");

        tokenizer_->advance();
        n_args += compileExpressionList();
    } else {
        throw std::runtime_error("compileSubroutineCall: invalid");
    }

    writer_->writeCall(function_name, n_args);

    tokenizer_->advance();
}
