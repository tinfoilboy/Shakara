#include <iostream>
#include <vector>
#include <fstream>
#include <functional>
#include <map>
#ifdef __linux
#include <stddef.h>
#include <cmath>
#endif

#include "../Shakara/Tokenizer/TokenizerTypes.hpp"
#include "../Shakara/Tokenizer/Tokenizer.hpp"
#include "../Shakara/AST/ASTBuilder.hpp"
#include "../Shakara/AST/ASTNode.hpp"
#include "../Shakara/AST/Nodes/ASTRootNode.hpp"
#include "../Shakara/Interpreter/Interpreter.hpp"

int main(int argc, char* argv[])
{
	// We have one argument, and thus, we can interpret
	// a file
	if (argc >= 2)
	{
		// Tokenize the file
		std::vector<Shakara::Token> tokens;

		std::ifstream file(argv[1]);

		Shakara::Tokenizer tokenizer;
		tokenizer.Tokenize(file, tokens);

		// Run the ASTBuilder to grab an AST
		Shakara::AST::RootNode   root;
		Shakara::AST::ASTBuilder builder;
		builder.Build(&root, tokens);

		Shakara::Interpreter interpreter;
		interpreter.ErrorHandler([]() {
			exit(0);
		});

		// Pass each argument into the interpreter
		// starting at 1
		for (size_t index = 1; index < static_cast<size_t>(argc); index++)
			interpreter.AppendCommandArgument(argv[index]);

		interpreter.Execute(&root);
	}
	else if (argc == 1)
	{
		std::cout << "Shakara Interpreter" << std::endl;
		std::cout << "-------------------" << std::endl;
		std::cout << std::endl;
		std::cout << "Input a file path as an argument to interpret Shakara code" << std::endl;
	}

	return 0;
}
