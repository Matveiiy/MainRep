#include "types.h"
//ends 1 element after primary expression
std::unique_ptr<AST::Expr> Parser::parse_primary() {
    if (lex.match(TOKEN_INT)) {
        int value = lex.last_int();
        lex.advance();
        return std::make_unique<AST::IntExpr>(value);
    } else if (lex.match(TOKEN_LPAREN)) {
        lex.advance();
        std::unique_ptr<AST::Expr> expr = parse_expression();
        if (!lex.match(TOKEN_RPAREN)) {
            error_system.add_error("expected ')'");
        }
    } else {
        error_system.add_error("expected primary expression");
        lex.advance();
    }
}
//ends parsing on first unknown element
std::unique_ptr<AST::Expr> Parser::parseBinopRHS(std::unique_ptr<AST::Expr> left) {
    while (true) {
        auto tok = lex.get_curtok();
        if (!precedence[tok].value) return left;
        lex.advance();
        auto right = parse_primary();
        if (!right) return nullptr;
        auto next_op = lex.get_curtok();
        if ((precedence[next_op].value > precedence[tok].value) || (precedence[next_op].is_right_associative && precedence[next_op].value == precedence[tok].value)) {
            right = parseBinopRHS(std::move(right));
            if (!right) return nullptr;
        }
        left = std::make_unique<AST::BinOpExpr>(std::move(left), std::move(right), tok);
    }
}
/*
    if not valid expression returns nullptr
    otherwise returns pointer to AST::Expr 
*/
std::unique_ptr<AST::Expr> Parser::parse_expression() {
    auto expr = parse_primary();
    if (expr == nullptr) {return nullptr;}
    return parseBinopRHS(std::move(expr));
}