#include "stdafx.hpp"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ShakaraTest
{
	namespace Interpreter
	{
		TEST_CLASS(InterpreterTests)
		{
		public:
			TEST_METHOD(InterpetPrintStatement)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					print("Shakara ", 1)
				)";

				std::stringstream stream(code, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				std::stringstream output;

				Shakara::Interpreter interpreter(output);
				interpreter.Execute(&root);

				Assert::AreEqual(
					"Shakara 1",
					output.str().c_str()
				);
			}

			TEST_METHOD(InterpretStringAmount)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					print(amt("What"))
					print(amt("What" + "Happened"))
				)";

				std::stringstream stream(code, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				std::stringstream output;

				Shakara::Interpreter interpreter(output);
				interpreter.Execute(&root);

				Assert::AreEqual(
					"412",
					output.str().c_str()
				);
			}

			TEST_METHOD(InterpetAssignmentAndPrint)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					name = "Shakara"

					print(name)

					name = "Lang"

					print(name)
				)";

				std::stringstream stream(code, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				std::stringstream output;

				Shakara::Interpreter interpreter(output);
				interpreter.Execute(&root);

				Assert::AreEqual(
					"ShakaraLang",
					output.str().c_str()
				);
			}

			TEST_METHOD(InterpetBinaryOperationsAndPrint)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					count = 1 + 1

					print(count)

					count = count - 1

					print(count)

					count = count * 4

					print(count)

					count = count / 2

					print(count)

					print("Shakar" + "a")
				)";

				std::stringstream stream(code, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				std::stringstream output;

				Shakara::Interpreter interpreter(output);
				interpreter.Execute(&root);

				// Should be "2142Shakara"
				Assert::AreEqual(
					"2142Shakara",
					output.str().c_str()
				);
			}

			TEST_METHOD(InterpetUnaryOperations)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					count = 24

					count += 4

					print(count)

					count -= 2

					print(count)

					count *= 2

					print(count)

					count /= 2

					print(count)

					count++

					print(count)
					
					count--

					print(count)

					name = "Shakar"

					name += "a"

					print(name)
				)";

				std::stringstream stream(code, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				std::stringstream output;

				Shakara::Interpreter interpreter(output);
				interpreter.Execute(&root);

				// Should be "282652262726Shakara"
				Assert::AreEqual(
					"282652262726Shakara",
					output.str().c_str()
				);
			}

			TEST_METHOD(InterpretFunctionDefinitionAndCall)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					count = 0

					count_up = (amount)
					{
						adder = amount - 1

						count += adder

						return count
					}

					print(count_up(1))
					print(count_up(4))
					print(count_up(6))
					print(count_up(count_up(2)))
				)";

				std::stringstream stream(code, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				std::stringstream output;

				Shakara::Interpreter interpreter(output);
				interpreter.Execute(&root);

				// Should be "03817"
				Assert::AreEqual(
					"03817",
					output.str().c_str()
				);
			}

			TEST_METHOD(InterpretFunctionReturn)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					exclaim_name = (name)
					{
						return name + "!"
					}

					print(exclaim_name("Maxwell"))
				)";

				std::stringstream stream(code, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				std::stringstream output;

				Shakara::Interpreter interpreter(output);
				interpreter.Execute(&root);

				// Should be "Maxwell!"
				Assert::AreEqual(
					"Maxwell!",
					output.str().c_str()
				);
			}

			TEST_METHOD(InterpretBooleanPrint)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					has_name = true
					name     = "Maxwell"

					print(has_name)
				)";

				std::stringstream stream(code, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				std::stringstream output;

				Shakara::Interpreter interpreter(output);
				interpreter.Execute(&root);

				// Should be "true"
				Assert::AreEqual(
					"true",
					output.str().c_str()
				);
			}

			TEST_METHOD(InterpretIfStatement)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					namer = (name)
					{
						if (name == "Maxwell")
							return name + "!"
						else if (name == "David")
						{						
							return name + "?"
						}
						else
							return name
					}

					print(namer("Maxwell"))
					print(namer("David"))
					print(namer("Max"))
				)";

				std::stringstream stream(code, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				std::stringstream output;

				Shakara::Interpreter interpreter(output);
				interpreter.Execute(&root);

				// Should be "Maxwell!David?Max"
				Assert::AreEqual(
					"Maxwell!David?Max",
					output.str().c_str()
				);
			}

			TEST_METHOD(InterpretWhileStatement)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					counter = 0

					while (counter != 10)
					{
						counter++

						print(counter)
					}
				)";

				std::stringstream stream(code, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				std::stringstream output;

				Shakara::Interpreter interpreter(output);
				interpreter.Execute(&root);

				// Should be "12345678910"
				Assert::AreEqual(
					"12345678910",
					output.str().c_str()
				);
			}

			TEST_METHOD(InterpretModulus)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					print(4 % 2 == 0)
				)";

				std::stringstream stream(code, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				std::stringstream output;

				Shakara::Interpreter interpreter(output);
				interpreter.Execute(&root);

				// Should be "true"
				Assert::AreEqual(
					"true",
					output.str().c_str()
				);
			}

			TEST_METHOD(InterpretLessGreater)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					print(1 < 2)
					print(2 > 2)
					print(2 >= 2)
					print(2 <= 2)
				)";

				std::stringstream stream(code, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				std::stringstream output;

				Shakara::Interpreter interpreter(output);
				interpreter.Execute(&root);

				// Should be "truefalsetruetrue"
				Assert::AreEqual(
					"truefalsetruetrue",
					output.str().c_str()
				);
			}

			TEST_METHOD(InterpretTypeCall)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					value = 1

					print(type(value))
					print(type("Test!"))
					print(type(2 * 2 + 2))
					print(type(2 * 2 == 4))
				)";

				std::stringstream stream(code, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				std::stringstream output;

				Shakara::Interpreter interpreter(output);
				interpreter.Execute(&root);

				// Should be "integerstringintegerboolean"
				Assert::AreEqual(
					"integerstringintegerboolean",
					output.str().c_str()
				);
			}

			TEST_METHOD(InterpretArrayUsage)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					// Create an array of phrases
                    // and use a variety of different
                    // types to see if it works
					potatoes = "Home-fried potatoes"

					phrases = [5] {
						"I don't eat no ham and eggs",
						"'Cause they're high in cholesterol",
						potatoes,
						2 * 2
					}

					phrases[3] = "Yes!"
					count      = 0

					push(phrases, "woo!")

					print(amt(phrases))

					pop(phrases, 3)

					while (count < amt(phrases))
					{
						phrase = phrases[count]

						print(phrase)						

						count++
					}
				)";

				std::stringstream stream(code, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				std::stringstream output;

				Shakara::Interpreter interpreter(output);
				interpreter.Execute(&root);

				Assert::AreEqual(
					"5I don't eat no ham and eggs'Cause they're high in cholesterolHome-fried potatoeswoo!",
					output.str().c_str()
				);
			}

			TEST_METHOD(InterpretIntegerCasting)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					print(integer("3"))
					print(integer(2))
					print(integer(6.0))
					print(integer(true))
				)";

				std::stringstream stream(code, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				std::stringstream output;

				Shakara::Interpreter interpreter(output);
				interpreter.Execute(&root);

				// Should be "3261"
				Assert::AreEqual(
					"3261",
					output.str().c_str()
				);
			}

			TEST_METHOD(InterpretDecimalCasting)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					print(decimal(6.0))
					print(decimal("2.24"))
					print(decimal(true))
				)";

				std::stringstream stream(code, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				std::stringstream output;

				Shakara::Interpreter interpreter(output);
				interpreter.Execute(&root);

				// Should be "62.241"
				Assert::AreEqual(
					"62.241",
					output.str().c_str()
				);
			}

			TEST_METHOD(InterpretStringCasting)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					print(string(6.0))
					print(string("Hey"))
					print(string(true))
					print(string(2))
				)";

				std::stringstream stream(code, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				std::stringstream output;

				Shakara::Interpreter interpreter(output);
				interpreter.Execute(&root);

				// Should be "6.24Heytrue2"
				Assert::AreEqual(
					"6.0Heytrue2",
					output.str().c_str()
				);
			}

			TEST_METHOD(InterpretBooleanCasting)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					print(boolean(1.0))
					print(boolean("false"))
					print(boolean(true))
					print(boolean(0))
				)";

				std::stringstream stream(code, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				std::stringstream output;

				Shakara::Interpreter interpreter(output);
				interpreter.Execute(&root);

				// Should be "truefalsetruefalse"
				Assert::AreEqual(
					"truefalsetruefalse",
					output.str().c_str()
				);
			}

			TEST_METHOD(InterpretForeachLooping)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					test_array = [5] {
						"Hello",
						"Darkness",
						"My",
						"Old",
						"Friend"
					}

					foreach (test_array)
						print(item)

					foreach (test_array,, idx)
						print(item, ": ", idx)

					foreach (test_array, itr, idx)
						print(itr, ": ", idx)
				)";

				std::stringstream stream(code, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				std::stringstream output;

				Shakara::Interpreter interpreter(output);
				interpreter.Execute(&root);

				// Should be "HelloDarknessMyOldFriendHello: 0Darkness: 1My: 2Old: 3Friend: 4Hello: 0Darkness: 1My: 2Old: 3Friend: 4"
				Assert::AreEqual(
					"HelloDarknessMyOldFriendHello: 0Darkness: 1My: 2Old: 3Friend: 4Hello: 0Darkness: 1My: 2Old: 3Friend: 4",
					output.str().c_str()
				);
			}
			
		};
	}
}