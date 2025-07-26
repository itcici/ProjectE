#include "eval.h"
#include <sstream>
#include <cctype>
#include <cmath>
#include <stdexcept>
#include <unordered_set>

// eval.cpp will implement the Evaluator class.
// It will handle the evaluation of infix expressions, including tokenization,
// conversion to postfix notation, and evaluation of the postfix expression.

int Evaluator::eval(const std::string& expr) {
    auto tokens = tokenize(expr);
    auto postfix = toPostfix(tokens);
    return evalPostfix(postfix);
}

// The tokenize function will split the input expression into tokens.
std::vector<std::string> Evaluator::tokenize(const std::string& expr) {
    std::vector<std::string> tokens;
    std::string num;
    bool expectUnary = true;

    const std::unordered_set<std::string> multiCharOps = {
        "==", "!=", ">=", "<=", "&&", "||", "++", "--"
    };

    for (size_t i = 0; i < expr.length(); ++i) {
        char ch = expr[i];

        if (std::isspace(ch)) continue;

        // Handle multi-digit numbers
        if (std::isdigit(ch)) {
            num += ch;
            expectUnary = false;
        }
        else {
            if (!num.empty()) {
                tokens.push_back(num);
                num.clear();
            }

            std::string op(1, ch);
            if (i + 1 < expr.length()) {
                std::string twoChar = op + expr[i + 1];
                if (multiCharOps.count(twoChar)) {
                    op = twoChar;
                    ++i;
                }
            }

            if (op == "(" || op == ")") {
                tokens.push_back(op);
                expectUnary = (op == "(");
            }
            else if (op == "+" || op == "-" || op == "++" || op == "--" || op == "!") {
                if (expectUnary)
                    tokens.push_back("u" + op);
                else
                    tokens.push_back(op);
                expectUnary = true;
            }
            else if (multiCharOps.count(op) || std::string("*/%^><=&|").find(op[0]) != std::string::npos) {
                tokens.push_back(op);
                expectUnary = true;
            }
            else {
                throw std::invalid_argument("Invalid token: " + op);
            }
        }
    }

    if (!num.empty()) {
        tokens.push_back(num);
    }

    return tokens;
}

// The following functions will help determine the type of token and its precedence.
bool Evaluator::isOperator(const std::string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "%" || token == "^" ||
        token == "==" || token == "!=" || token == ">" || token == "<" || token == ">=" || token == "<=" ||
        token == "&&" || token == "||";
}


//// The isUnaryOperator function checks if the token is a unary operator.
bool Evaluator::isUnaryOperator(const std::string& token) {
    return token == "u+" || token == "u-" || token == "u!" || token == "u++" || token == "u--";
}


//// The precedence function returns the precedence level of the operator.
int Evaluator::precedence(const std::string& op) {
    if (op == "u!" || op == "u++" || op == "u--" || op == "u-" || op == "u+") return 8;
    if (op == "^") return 7;
    if (op == "*" || op == "/" || op == "%") return 6;
    if (op == "+" || op == "-") return 5;
    if (op == ">" || op == "<" || op == ">=" || op == "<=") return 4;
    if (op == "==" || op == "!=") return 3;
    if (op == "&&") return 2;
    if (op == "||") return 1;
    return 0;
}

//// The isRightAssociative function checks if the operator is right associative.
bool Evaluator::isRightAssociative(const std::string& op) {
    return op == "^" || op.substr(0, 1) == "u";
}

//the toPostfix function converts the infix expression tokens to postfix notation.
std::vector<std::string> Evaluator::toPostfix(const std::vector<std::string>& tokens) {
    std::vector<std::string> output;
    Stack<std::string> ops;

    for (const auto& token : tokens) {
        if (std::isdigit(token[0])) {
            output.push_back(token);
        }
        else if (token == "(") {
            ops.push(token);
        }
        else if (token == ")") {
            while (!ops.empty() && ops.top() != "(") {
                output.push_back(ops.top());
                ops.pop();
            }
            if (!ops.empty()) ops.pop();
        }
        else if (isOperator(token) || isUnaryOperator(token)) {
            while (!ops.empty() && ops.top() != "(" &&
                (precedence(ops.top()) > precedence(token) ||
                    (precedence(ops.top()) == precedence(token) && !isRightAssociative(token)))) {
                output.push_back(ops.top());
                ops.pop();
            }
            ops.push(token);
        }
    }

    while (!ops.empty()) {
        output.push_back(ops.top());
        ops.pop();
    }

    return output;
}

//// The evalPostfix function evaluates the postfix expression.
int Evaluator::evalPostfix(const std::vector<std::string>& postfix) {
    Stack<int> operands;

    for (const auto& token : postfix) {
        if (std::isdigit(token[0])) {
            operands.push(std::stoi(token));
        }
        else if (isUnaryOperator(token)) {
            if (operands.empty()) throw std::runtime_error("Stack is empty");
            int val = operands.top(); operands.pop();
            operands.push(applyUnary(token, val));
        }
        else if (isOperator(token)) {
            if (operands.size() < 2) throw std::runtime_error("Stack is empty");
            int rhs = operands.top(); operands.pop();
            int lhs = operands.top(); operands.pop();
            operands.push(applyOperator(token, lhs, rhs));
        }
        else {
            throw std::invalid_argument("Invalid token in postfix: " + token);
        }
    }

    return operands.top();
}


//// The applyOperator function applies the operator to the two operands.
int Evaluator::applyOperator(const std::string& op, int lhs, int rhs) {
    if (op == "+") return lhs + rhs;
    if (op == "-") return lhs - rhs;
    if (op == "*") return lhs * rhs;
    if (op == "/") return lhs / rhs;
    if (op == "%") return lhs % rhs;
    if (op == "^") return std::pow(lhs, rhs);

    if (op == "==") return lhs == rhs;
    if (op == "!=") return lhs != rhs;
    if (op == "<") return lhs < rhs;
    if (op == ">") return lhs > rhs;
    if (op == "<=") return lhs <= rhs;
    if (op == ">=") return lhs >= rhs;

    if (op == "&&") return lhs && rhs;
    if (op == "||") return lhs || rhs;

    throw std::invalid_argument("Unknown operator: " + op);
}

//// The applyUnary function applies the unary operator to the operand.
int Evaluator::applyUnary(const std::string& op, int val) {
    if (op == "u-") return -val;
    if (op == "u+") return +val;
    if (op == "u!") return !val;
    if (op == "u++") return val + 1;
    if (op == "u--") return val - 1;
    throw std::invalid_argument("Unknown unary operator: " + op);
}
