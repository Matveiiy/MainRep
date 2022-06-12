#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED
#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <string>
#include <fstream>
#include <memory>
using namespace std;
enum TokenType{
    TOKEN_PLUS,
    TOKEN_MULT,
    TOKEN_DIVIDE,
    TOKEN_MINUS,
    TOKEN_ASSIGN,
    TOKEN_NEWLINE,
    TOKEN_EOF,
    TOKEN_UNKNOWN,
    TOKEN_LPAREN,
    TOKEN_RPAREN,
    TOKEN_LBRACE,
    TOKEN_RBRACE,
    TOKEN_INT,
    TOKEN_IDENTIFIER,
    TOKEN_SEMICOLON,
};
enum PrecedenceValue {
    PREC_NONE=0,
    PREC_TERM,
    PREC_FACTOR,
};
struct Precedence{
    PrecedenceValue value;
    bool is_right_associative;
    Precedence(PrecedenceValue v=PREC_NONE, bool right_associative=false):value(v), is_right_associative(right_associative) {}
};
class Lexer{
    const std::string filepath; std::ifstream fin;
    char cur_char = ' ';TokenType curtok;
    int number_data = 0; std::string identifier_data;
    int next_symbol() { if (fin.eof()) return EOF;  return fin.get(); }
    TokenType _get_token() {
        while (cur_char == ' ' || cur_char == '\t') cur_char = next_symbol();
        if (isdigit(cur_char)) {
            int number_data = 0;
            do {
                number_data = number_data * 10 + (cur_char - '0');
                cur_char = next_symbol();
            } while (isdigit(cur_char));
            return TOKEN_INT;
        }
        if (isalpha(cur_char)) {
            identifier_data = "";
            do {
                identifier_data += cur_char;
                cur_char = next_symbol();
            } while (isalnum(cur_char));
            return TOKEN_IDENTIFIER;
        }
        if (cur_char == '+') {cur_char = next_symbol();return TOKEN_PLUS;}
        if (cur_char == '-') {cur_char = next_symbol();return TOKEN_MINUS;}
        if (cur_char == '*') {cur_char = next_symbol();return TOKEN_MULT;}
        if (cur_char == '/') {cur_char = next_symbol();return TOKEN_DIVIDE;}
        if (cur_char == '=') {cur_char = next_symbol();return TOKEN_ASSIGN;}
        if (cur_char == ';') {cur_char = next_symbol();return TOKEN_SEMICOLON;}
        if (cur_char == '(') {cur_char = next_symbol();return TOKEN_LPAREN;}
        if (cur_char == ')') {cur_char = next_symbol();return TOKEN_RPAREN;}
        if (cur_char == '{') {cur_char = next_symbol();return TOKEN_LBRACE;}
        if (cur_char == '}') {cur_char = next_symbol();return TOKEN_RBRACE;}
        if (cur_char == '\n') {cur_char = next_symbol();return TOKEN_NEWLINE;}
        if (fin.eof()) {return TOKEN_EOF;}
        return TOKEN_UNKNOWN;
    }
    inline TokenType get_token() {return _get_token();}
public:
    void advance() {curtok = get_token();}
    bool match(TokenType t) {
        if (curtok == t) {
            advance();
            return true;
        }
        return false;
    }
    TokenType next() {advance();return curtok;}
    TokenType get_curtok() {return curtok;}
    inline int last_int() {return number_data;}
    inline std::string last_identifier() {return identifier_data;}
    inline int get_line() {return NULL;}//TODO
    inline int get_column() {return NULL;}//TODO
    inline std::string get_filepath() {return filepath; }
    inline Lexer(const std::string& Filepath):filepath(Filepath), fin(Filepath) {}
    ~Lexer() {}
};
struct ErrorSystem{
    std::vector<std::string> errors;
    std::vector<std::string> warnings;
    Lexer& lex;
    void add_error(std::string msg) {errors.push_back(lex.get_filepath() + ":" + std::to_string(lex.get_line()) + ":" + std::to_string(lex.get_column()) + " error => " + msg);}//{errors.push_back(lex.get_filepath() + ":" + lex.get_line() + ":" + lex.get_column() + " error => " + msg);}
    ErrorSystem(Lexer& Lex):lex(Lex) {}
};
namespace AST{
    class Expr{
    public:
        Expr() {}
        virtual ~Expr() {}
        virtual void print() { std::cout << "(Expr)" << std::endl; }
        virtual void write_asm(std::ofstream& fout) {}
    };
    class IntExpr: public Expr{
    public:
        int value;
        IntExpr(int Value):value(Value) {}
        ~IntExpr() {}
        void print() { std::cout << "(NumberExpr: " << value << ")" << std::endl; }
        void write_asm(std::ofstream& fout) { fout << "push " << value << std::endl; }
    };
    class BinOpExpr: public Expr{
    public:
        int op;
        std::unique_ptr<Expr> left;
        std::unique_ptr<Expr> right;
        BinOpExpr(std::unique_ptr<Expr> Left, std::unique_ptr<Expr> Right, int Op):left(std::move(Left)), right(std::move(Right)), op(Op) {}
        void print() {
            std::cout << "(BinOpExpr: ";
            left->print();
            std::cout << " ";
            switch (op) {
                case '+': std::cout << "+"; break;
                case '-': std::cout << "-"; break;
                case '*': std::cout << "*"; break;
                case '/': std::cout << "/"; break;
                default: std::cout << "unknown"; break;
            }
            std::cout << " ";
            right->print();
        }
        ~BinOpExpr() {}
    };
}
class Parser{
    std::unique_ptr<AST::Expr> parseBinopRHS(std::unique_ptr<AST::Expr> left);
    void InitPrecedence() {
        precedence[TOKEN_PLUS] = {PREC_TERM, false};
        precedence[TOKEN_MINUS] = {PREC_TERM, false};
        precedence[TOKEN_MULT] = {PREC_FACTOR, false};
        precedence[TOKEN_DIVIDE] = {PREC_FACTOR, false};
    }
public:
    Precedence precedence[256];
    Lexer lex;
    ErrorSystem error_system;
    std::unique_ptr<AST::Expr> parse_primary();
    std::unique_ptr<AST::Expr> parse_expression();
    Parser(const char* filename):lex(filename), error_system(lex) {InitPrecedence();}
    void parse() {
        lex.advance();
        while (!lex.match(TOKEN_EOF)) {
            std::unique_ptr<AST::Expr> expr = parse_expression();
            if (expr) {
                expr->print();
            }
        }
    }
    ~Parser() {}
};
#endif // TYPES_H_INCLUDED