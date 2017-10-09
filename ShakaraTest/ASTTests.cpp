#include "stdafx.hpp"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ShakaraTest
{
	namespace AST
	{
		TEST_CLASS(ASTTests)
		{
		public:
			TEST_METHOD(ASTBuildSingleAssignment)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string statement = "test = 1";

				std::stringstream stream(statement, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				// There should only be one child in
				// the root node, the assignment node
				Assert::AreEqual(
					static_cast<size_t>(1),
					static_cast<size_t>(root.Children())
				);

				// Now, recurse through and check each
				// type and make sure they are all
				// correct
				Shakara::AST::AssignmentNode* node = static_cast<Shakara::AST::AssignmentNode*>(root[0]);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(node->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::IDENTIFIER),
					static_cast<uint8_t>(node->GetIdentifier()->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::INTEGER),
					static_cast<uint8_t>(node->GetAssignment()->Type())
				);
			}

			TEST_METHOD(ASTBuildBinaryOperationAssignment)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string statement = "test = 2 * 2";

				std::stringstream stream(statement, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				// There should only be one child in
				// the root node, the assignment node
				Assert::AreEqual(
					static_cast<size_t>(1),
					static_cast<size_t>(root.Children())
				);

				// Now, recurse through and check each
				// type and make sure they are all
				// correct
				Shakara::AST::AssignmentNode* node = static_cast<Shakara::AST::AssignmentNode*>(root[0]);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(node->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::IDENTIFIER),
					static_cast<uint8_t>(node->GetIdentifier()->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::BINARY_OP),
					static_cast<uint8_t>(node->GetAssignment()->Type())
				);

				// Make sure that the correct values
				// are being loaded into the binary
				// operation
				Shakara::AST::BinaryOperation* operation = static_cast<Shakara::AST::BinaryOperation*>(node->GetAssignment());

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::INTEGER),
					static_cast<uint8_t>(operation->GetLeftHand()->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::MULTIPLY),
					static_cast<uint8_t>(operation->Operation())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::INTEGER),
					static_cast<uint8_t>(operation->GetRightHand()->Type())
				);
			}

			TEST_METHOD(ASTBuildMultiBinaryOperationAssignment)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string statement = "test = 2 * 2 / 2";

				std::stringstream stream(statement, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				// There should only be one child in
				// the root node, the assignment node
				Assert::AreEqual(
					static_cast<size_t>(1),
					static_cast<size_t>(root.Children())
				);

				// Now, recurse through and check each
				// type and make sure they are all
				// correct
				Shakara::AST::AssignmentNode* node = static_cast<Shakara::AST::AssignmentNode*>(root[0]);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(node->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::IDENTIFIER),
					static_cast<uint8_t>(node->GetIdentifier()->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::BINARY_OP),
					static_cast<uint8_t>(node->GetAssignment()->Type())
				);

				// Make sure that the types are correct
				// including the nested binary operation
				Shakara::AST::BinaryOperation* operation = static_cast<Shakara::AST::BinaryOperation*>(node->GetAssignment());

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::INTEGER),
					static_cast<uint8_t>(operation->GetLeftHand()->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::MULTIPLY),
					static_cast<uint8_t>(operation->Operation())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::BINARY_OP),
					static_cast<uint8_t>(operation->GetRightHand()->Type())
				);

				// Make sure the nested is an integer divided
				// by another integer
				Shakara::AST::BinaryOperation* nested = static_cast<Shakara::AST::BinaryOperation*>(operation->GetRightHand());

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::INTEGER),
					static_cast<uint8_t>(nested->GetLeftHand()->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::DIVIDE),
					static_cast<uint8_t>(nested->Operation())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::INTEGER),
					static_cast<uint8_t>(nested->GetRightHand()->Type())
				);
			}

			TEST_METHOD(ASTBuildMultiLineAssignment)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					count    = 1
					argument = 2 * 2
					named    = 2 * 2 * 9 / 2
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

				// There should only be one child in
				// the root node, the assignment node
				Assert::AreEqual(
					static_cast<size_t>(3),
					static_cast<size_t>(root.Children())
				);

				// Check if the first child is an
				// assignment node
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(root[0]->Type())
				);
			}

			TEST_METHOD(ASTBuildFunctionDeclaration)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					count    = 1
					argument = 2 * 2 * 2
					
					add_to_counter = (value, other)
					{
						count += value

						count *= value + value
					}
					
					stuff = 1
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

				// There should be four children corresponding
				// to each declaration in the root
				Assert::AreEqual(
					static_cast<size_t>(4),
					static_cast<size_t>(root.Children())
				);

				// Check if the first child is an
				// assignment node
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(root[0]->Type())
				);

				// Next, check if the second child
				// is also an assignment node
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(root[1]->Type())
				);

				// Now for the more nitty gritty one
				// check if the function declaration
				// actually is a function declaration
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::FUNCTION),
					static_cast<uint8_t>(root[2]->Type())
				);

				// Now, check the declaration more thoroughly
				Shakara::AST::FunctionDeclaration* declaration = static_cast<Shakara::AST::FunctionDeclaration*>(root[2]);
			
				// Begin by checking if we have the right amount
				// of arguments for the function
				Assert::AreEqual(
					static_cast<size_t>(2),
					static_cast<size_t>(declaration->Arguments().size())
				);

				// Now check for the body of the function, which
				// is a RootNode stuck in the node
				//
				// Should be two nodes
				Shakara::AST::RootNode* body = static_cast<Shakara::AST::RootNode*>(declaration->Body());
				
				Assert::AreEqual(
					static_cast<size_t>(2),
					static_cast<size_t>(body->Children())
				);

				// Finally, check if the statement below the function
				// declaration is intact and outside of the function
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(root[3]->Type())
				);
			}

			TEST_METHOD(ASTBuildIncrementDecrement)
			{
				// Create a test increment decrement
				// and insert it into a stringstream
				std::string statement = R"(
					test++
					test--
				)";

				std::stringstream stream(statement, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				// There should only be one child in
				// the root node, the assignment node
				Assert::AreEqual(
					static_cast<size_t>(2),
					static_cast<size_t>(root.Children())
				);

				// Now, recurse through and check each
				// type and make sure they are all
				// correct
				Shakara::AST::AssignmentNode* incNode = static_cast<Shakara::AST::AssignmentNode*>(root[0]);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(incNode->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::IDENTIFIER),
					static_cast<uint8_t>(incNode->GetIdentifier()->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::BINARY_OP),
					static_cast<uint8_t>(incNode->GetAssignment()->Type())
				);

				// Check if the first node's binary operation is
				// the correct sign
				Shakara::AST::BinaryOperation* increment = static_cast<Shakara::AST::BinaryOperation*>(incNode->GetAssignment());
			
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ADD),
					static_cast<uint8_t>(increment->Operation())
				);

				// Do the same for the second root node
				Shakara::AST::AssignmentNode* decNode = static_cast<Shakara::AST::AssignmentNode*>(root[1]);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(decNode->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::IDENTIFIER),
					static_cast<uint8_t>(decNode->GetIdentifier()->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::BINARY_OP),
					static_cast<uint8_t>(decNode->GetAssignment()->Type())
				);

				// Check if the first node's binary operation is
				// the correct sign
				Shakara::AST::BinaryOperation* decrement = static_cast<Shakara::AST::BinaryOperation*>(decNode->GetAssignment());

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::SUBTRACT),
					static_cast<uint8_t>(decrement->Operation())
				);
			}

			TEST_METHOD(ASTBuildAssignmentArithmeticOperators)
			{
				// Create a test increment decrement
				// and insert it into a stringstream
				std::string statement = R"(
					test += 1
					test -= 2
					test *= 4 + 4
					test /= 2
				)";

				std::stringstream stream(statement, std::ios::in);

				// Tokenize the stringstream
				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Run the ASTBuilder to grab an AST
				Shakara::AST::RootNode   root;
				Shakara::AST::ASTBuilder builder;
				builder.Build(&root, tokens);

				// There should only be one child in
				// the root node, the assignment node
				Assert::AreEqual(
					static_cast<size_t>(4),
					static_cast<size_t>(root.Children())
				);

				// Now, recurse through and check each
				// type and make sure they are all
				// correct
				Shakara::AST::AssignmentNode* plusEqualAssign = static_cast<Shakara::AST::AssignmentNode*>(root[0]);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(plusEqualAssign->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::IDENTIFIER),
					static_cast<uint8_t>(plusEqualAssign->GetIdentifier()->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::BINARY_OP),
					static_cast<uint8_t>(plusEqualAssign->GetAssignment()->Type())
				);

				// Check if the first node's binary operation is
				// the correct sign
				Shakara::AST::BinaryOperation* plusEqual = static_cast<Shakara::AST::BinaryOperation*>(plusEqualAssign->GetAssignment());

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ADD),
					static_cast<uint8_t>(plusEqual->Operation())
				);

				// Now check the minus equal expression
				Shakara::AST::AssignmentNode* minusEqualAssign = static_cast<Shakara::AST::AssignmentNode*>(root[1]);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(minusEqualAssign->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::IDENTIFIER),
					static_cast<uint8_t>(minusEqualAssign->GetIdentifier()->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::BINARY_OP),
					static_cast<uint8_t>(minusEqualAssign->GetAssignment()->Type())
				);

				// Check if the binary operation is
				// the correct sign
				Shakara::AST::BinaryOperation* minusEqual = static_cast<Shakara::AST::BinaryOperation*>(minusEqualAssign->GetAssignment());

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::SUBTRACT),
					static_cast<uint8_t>(minusEqual->Operation())
				);

				// Now check the multiply equal expression
				// This one should have a nested binary operation
				// so make sure that is the case
				Shakara::AST::AssignmentNode* multiplyEqualAssign = static_cast<Shakara::AST::AssignmentNode*>(root[2]);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(multiplyEqualAssign->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::IDENTIFIER),
					static_cast<uint8_t>(multiplyEqualAssign->GetIdentifier()->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::BINARY_OP),
					static_cast<uint8_t>(multiplyEqualAssign->GetAssignment()->Type())
				);

				// Check if the binary operation is
				// the correct sign
				Shakara::AST::BinaryOperation* multiplyEqual = static_cast<Shakara::AST::BinaryOperation*>(multiplyEqualAssign->GetAssignment());

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::MULTIPLY),
					static_cast<uint8_t>(multiplyEqual->Operation())
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::BINARY_OP),
					static_cast<uint8_t>(multiplyEqual->GetRightHand()->Type())
				);

				// Finally check the divide equal expression
				Shakara::AST::AssignmentNode* divideEqualAssign = static_cast<Shakara::AST::AssignmentNode*>(root[3]);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(divideEqualAssign->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::IDENTIFIER),
					static_cast<uint8_t>(divideEqualAssign->GetIdentifier()->Type())
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::BINARY_OP),
					static_cast<uint8_t>(divideEqualAssign->GetAssignment()->Type())
				);

				// Check if the binary operation is
				// the correct sign
				Shakara::AST::BinaryOperation* divideEqual = static_cast<Shakara::AST::BinaryOperation*>(divideEqualAssign->GetAssignment());

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::DIVIDE),
					static_cast<uint8_t>(divideEqual->Operation())
				);
			}

			TEST_METHOD(ASTBuildFunctionCall)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					add_to_counter = (value)
					{
						count += value
					}

					add_to_counter(2 * 2 * 2)

					test = 2 * 2
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

				// There should be three children in the root
				// the function definition, the function call
				// and the test assignment
				Assert::AreEqual(
					static_cast<size_t>(3),
					static_cast<size_t>(root.Children())
				);

				// Check if the first child is a function
				// declaration node
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::FUNCTION),
					static_cast<uint8_t>(root[0]->Type())
				);
				
				// Now check if the next root is a function
				// call node
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::CALL),
					static_cast<uint8_t>(root[1]->Type())
				);

				// Now, check the call node to see if the sole
				// argument is an integer
				Shakara::AST::FunctionCall* call = static_cast<Shakara::AST::FunctionCall*>(root[1]);

				// Make sure we have the correct number of arguments
				Assert::AreEqual(
					static_cast<size_t>(1),
					static_cast<size_t>(call->Arguments().size())
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::BINARY_OP),
					static_cast<uint8_t>(call->Arguments()[0]->Type())
				);
			}

			TEST_METHOD(ASTBuildStringAndDecimalAssignment)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					name  = "Shakara"
					scale = 1.25
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

				// There should only be two children in
				// the root node, the assignment node for
				// the string and the assignment node for
				// the decimal
				Assert::AreEqual(
					static_cast<size_t>(2),
					static_cast<size_t>(root.Children())
				);

				// Check if the first child is an assignment
				// node
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(root[0]->Type())
				);

				// Now check if the next root is also an assignment
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(root[1]->Type())
				);

				// Now check the assigment to see if the first
				// is assigned to a string
				Shakara::AST::AssignmentNode* nameAssign = static_cast<Shakara::AST::AssignmentNode*>(root[0]);

				// Make sure that the assigned value is a string
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::STRING),
					static_cast<uint8_t>(nameAssign->GetAssignment()->Type())
				);

				// Make sure it is the correct string as well
				Shakara::AST::StringNode* nameString = static_cast<Shakara::AST::StringNode*>(nameAssign->GetAssignment());

				Assert::AreEqual(
					"Shakara",
					nameString->Value().c_str()
				);

				// Now check the decimal assignment to see if the
				// type is correct
				// Now check the assigment to see if the first
				// is assigned to a string
				Shakara::AST::AssignmentNode* scaleAssign = static_cast<Shakara::AST::AssignmentNode*>(root[1]);

				// Make sure that the assigned value is a string
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::DECIMAL),
					static_cast<uint8_t>(scaleAssign->GetAssignment()->Type())
				);

				// Make sure it is the correct string as well
				Shakara::AST::DecimalNode* scaleDecimal = static_cast<Shakara::AST::DecimalNode*>(scaleAssign->GetAssignment());

				Assert::AreEqual(
					1.25f,
					scaleDecimal->Value()
				);
			}

			TEST_METHOD(ASTBuildPrintStatement)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					name = "Shakara"
					
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

				// There should only be two children in
				// the root node, the assignment node and
				// the print call
				Assert::AreEqual(
					static_cast<size_t>(2),
					static_cast<size_t>(root.Children())
				);

				// Check if the first node is an assignment
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(root[0]->Type()),
					L"Expected ASSIGN!"
				);

				// Next check if the second node is a function
				// call and then check the flags to see if its
				// a special print
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::CALL),
					static_cast<uint8_t>(root[1]->Type()),
					L"Expected CALL!"
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::CallFlags::PRINT),
					static_cast<uint8_t>(static_cast<Shakara::AST::FunctionCall*>(root[1])->Flags()),
					L"Expected a special PRINT flag on this call!"
				);
			}

			TEST_METHOD(ASTBuildReturnStatement)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					greet = (name)
					{
						return "Hello, " + name + "!"
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

				// There should only be one child in
				// the root node, the definition node
				Assert::AreEqual(
					static_cast<size_t>(1),
					static_cast<size_t>(root.Children())
				);

				// Check if the first child is a function
				// declaration node
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::FUNCTION),
					static_cast<uint8_t>(root[0]->Type())
				);

				// Now, check if the child node is a return
				// node
				Shakara::AST::RootNode* funcBody = static_cast<Shakara::AST::RootNode*>(static_cast<Shakara::AST::FunctionDeclaration*>(root[0])->Body());

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::RETURN),
					static_cast<uint8_t>((*funcBody)[0]->Type())
				);
			}

			TEST_METHOD(ASTBuildBooleanAndLogicOperation)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					test_bool      = true
					test_bool_test = test_bool && 1 == 1
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

				// There should only be two children in
				// the root node, the definition node
				Assert::AreEqual(
					static_cast<size_t>(2),
					static_cast<size_t>(root.Children())
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(root[0]->Type())
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(root[1]->Type())
				);

				Shakara::AST::AssignmentNode* node = static_cast<Shakara::AST::AssignmentNode*>(root[1]);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::LOGICAL_OP),
					static_cast<uint8_t>(node->GetAssignment()->Type())
				);
			}

			TEST_METHOD(ASTBuildIfStatement)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					has_name = true
					name     = "Maxwell"
					
					if (has_name && name == "Maxwell" || name == "Jack")
					{
						name += "!"	
					}

					name += "!"
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

				// There should only be two children in
				// the root node, the definition node
				Assert::AreEqual(
					static_cast<size_t>(4),
					static_cast<size_t>(root.Children())
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(root[0]->Type())
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(root[1]->Type())
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::IF_STATEMENT),
					static_cast<uint8_t>(root[2]->Type())
				);
			}

			TEST_METHOD(ASTBuildWhileStatement)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					while (condition)
						print(condition)
					
					print("Yes!")	
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

				// There should only be two children in
				// the root node, the definition node
				Assert::AreEqual(
					static_cast<size_t>(2),
					static_cast<size_t>(root.Children())
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::WHILE_STATEMENT),
					static_cast<uint8_t>(root[0]->Type())
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::CALL),
					static_cast<uint8_t>(root[1]->Type())
				);
			}

			TEST_METHOD(ASTBuildArrayDefines)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					fixed_array = [size]

					dynamic_array = [] { 1, 2 }
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

				// There should only be two children in
				// the root node, the definition node
				Assert::AreEqual(
					static_cast<size_t>(2),
					static_cast<size_t>(root.Children())
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(root[0]->Type())
				);

				// Check the first assignment
				Shakara::AST::AssignmentNode* assignFirst = static_cast<Shakara::AST::AssignmentNode*>(root[0]);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ARRAY),
					static_cast<uint8_t>(assignFirst->GetAssignment()->Type())
				);

				// Check that the array is fixed
				Shakara::AST::ArrayNode* firstArray = static_cast<Shakara::AST::ArrayNode*>(assignFirst->GetAssignment());

				Assert::IsTrue(firstArray->Fixed());

				// Make sure that the capacity is a identifier
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::IDENTIFIER),
					static_cast<uint8_t>(firstArray->Capacity()->Type())
				);

				// Check the second assignment of a dynamic
				// array
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(root[1]->Type())
				);

				// Check the first assignment
				Shakara::AST::AssignmentNode* assignSecond = static_cast<Shakara::AST::AssignmentNode*>(root[1]);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ARRAY),
					static_cast<uint8_t>(assignSecond->GetAssignment()->Type())
				);

				// Check that the array is fixed
				Shakara::AST::ArrayNode* secondArray = static_cast<Shakara::AST::ArrayNode*>(assignSecond->GetAssignment());

				Assert::IsFalse(secondArray->Fixed());

				// Check if we have two elements
				Assert::AreEqual(
					static_cast<size_t>(2),
					secondArray->Size()
				);
			}

			TEST_METHOD(ASTBuildArrayAccess)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					array[index] = 1 + 1

					array[index] += 2

					print(array[index] + 1)
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

				// There should only be two children in
				// the root node, the definition node
				Assert::AreEqual(
					static_cast<size_t>(3),
					static_cast<size_t>(root.Children())
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(root[0]->Type())
				);

				// Check to see if the assignment has an identifier of an
				// array element identifier, and a binary expression as the
				// assignment
				Shakara::AST::AssignmentNode* firstAssignment = static_cast<Shakara::AST::AssignmentNode*>(root[0]);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ARRAY_ELEMENT_IDENTIFIER),
					static_cast<uint8_t>(firstAssignment->GetIdentifier()->Type())
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::BINARY_OP),
					static_cast<uint8_t>(firstAssignment->GetAssignment()->Type())
				);

				// This should also be an assignment, but to an array element
				// with a binary operation as the assigned node
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ASSIGN),
					static_cast<uint8_t>(root[1]->Type())
				);

				Shakara::AST::AssignmentNode* secondAssignment = static_cast<Shakara::AST::AssignmentNode*>(root[1]);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ARRAY_ELEMENT_IDENTIFIER),
					static_cast<uint8_t>(secondAssignment->GetIdentifier()->Type())
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::BINARY_OP),
					static_cast<uint8_t>(secondAssignment->GetAssignment()->Type())
				);

				// Make sure that the binary operation's left hand is the
				// element identifier
				Shakara::AST::BinaryOperation* operation = static_cast<Shakara::AST::BinaryOperation*>(secondAssignment->GetAssignment());

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::ARRAY_ELEMENT_IDENTIFIER),
					static_cast<uint8_t>(operation->GetLeftHand()->Type())
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::CALL),
					static_cast<uint8_t>(root[2]->Type())
				);
			}

			TEST_METHOD(ASTBuildForeachLoop)
			{
				// Create a test statement and insert
				// it into a stringstream
				std::string code = R"(
					foreach (collection)
					{
					}

					foreach (collection, item)
					{
					}

					foreach (collection, item, index)
					{
					}

					foreach (collection,, index)
					{
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

				// There should only be four children in
				// the root node
				Assert::AreEqual(
					static_cast<size_t>(4),
					static_cast<size_t>(root.Children())
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::FOREACH_STATEMENT),
					static_cast<uint8_t>(root[0]->Type())
				);

				// Make sure that the foreach has an identifier for the
				// collection but no item or index names
				Shakara::AST::ForeachStatement* firstForeach = static_cast<Shakara::AST::ForeachStatement*>(root[0]);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::IDENTIFIER),
					static_cast<uint8_t>(firstForeach->Collection()->Type())
				);

				Assert::IsNull(firstForeach->ItemName());

				Assert::IsNull(firstForeach->IndexName());

				// Do the same for the second, but instead check if
				// the item name is an identifier
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::FOREACH_STATEMENT),
					static_cast<uint8_t>(root[1]->Type())
				);

				Shakara::AST::ForeachStatement* secondForeach = static_cast<Shakara::AST::ForeachStatement*>(root[1]);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::IDENTIFIER),
					static_cast<uint8_t>(secondForeach->Collection()->Type())
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::IDENTIFIER),
					static_cast<uint8_t>(secondForeach->ItemName()->Type())
				);

				Assert::IsNull(secondForeach->IndexName());

				// Make sure all of the parts of this foreach
				// are defined
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::FOREACH_STATEMENT),
					static_cast<uint8_t>(root[2]->Type())
				);

				Shakara::AST::ForeachStatement* thirdForeach = static_cast<Shakara::AST::ForeachStatement*>(root[2]);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::IDENTIFIER),
					static_cast<uint8_t>(thirdForeach->Collection()->Type())
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::IDENTIFIER),
					static_cast<uint8_t>(thirdForeach->ItemName()->Type())
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::IDENTIFIER),
					static_cast<uint8_t>(thirdForeach->IndexName()->Type())
				);

				// Finally, make sure the item name is omitted in
				// this loop
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::FOREACH_STATEMENT),
					static_cast<uint8_t>(root[3]->Type())
				);

				Shakara::AST::ForeachStatement* fourthForeach = static_cast<Shakara::AST::ForeachStatement*>(root[3]);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::IDENTIFIER),
					static_cast<uint8_t>(fourthForeach->Collection()->Type())
				);

				Assert::IsNull(fourthForeach->ItemName());

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::AST::NodeType::IDENTIFIER),
					static_cast<uint8_t>(fourthForeach->IndexName()->Type())
				);
			}

		};
	}
}