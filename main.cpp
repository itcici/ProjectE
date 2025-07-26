#include "eval.h"
#include <iostream>
#include <string>

// Main.cpp will serve as the entry point for the evaluator application.
// displaying a welcome message, accepting user input, and evaluating expressions.
//// It will handle exceptions and display results or errors accordingly.
int main() {
    Evaluator eval;
    std::string expr;

    std::cout << "\Infix Expression Evaluator!\n";
    std::cout << "Supported: + - * / % ^ (), comparisons, logical (&&, ||), unary (!, ++, --, -)\n";
    std::cout << "Type 'exit' to quit.\n";

    while (true) {
        std::cout << "Expression: ";
        std::getline(std::cin, expr);

        if (expr == "exit" || expr == "EXIT") break;

        try {
            int result = eval.eval(expr);
            std::cout << "Result: " << result << "\n";
        }
        catch (const std::exception& e) {
            std::cout << "Error: " << e.what() << "\n";
        }
    }

    return 0;
}
