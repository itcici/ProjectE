#ifndef EVALUATOR_H
#define EVALUATOR_H

#include "stack.h"
#include <string>
#include <vector>


//eval.h will define the Evaluator class.
// tells complier that evaluator class will be used to evaluate infix expressions.

class Evaluator {
public:
    int eval(const std::string& expr);

private:
    std::vector<std::string> tokenize(const std::string& expr);
    std::vector<std::string> toPostfix(const std::vector<std::string>& tokens);
    int evalPostfix(const std::vector<std::string>& postfix);

    int applyOperator(const std::string& op, int lhs, int rhs);
    int precedence(const std::string& op);
    bool isRightAssociative(const std::string& op);
    bool isOperator(const std::string& token);
    int applyUnary(const std::string& op, int val);
    bool isUnaryOperator(const std::string& token);

};

#endif
