#include "stdafx.hpp"
#include "CppUnitTest.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ShakaraTest
{		
	namespace Tokenizer
	{
		TEST_CLASS(TokenizerTests)
		{
		public:
			TEST_METHOD(TokenizeStatement)
			{
				std::string statement = "test = 1";

				std::stringstream stream(statement, std::ios::in);

				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Make sure that we have three tokens
				// for this test statement
				Assert::AreEqual(static_cast<size_t>(3), tokens.size());

				// Next, make sure that each token is
				// the right type
				//
				// The order should be, identifier,
				// equals, integer
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[0].type),
					L"Incorrect type! Expected an IDENTIFIER type."
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::EQUAL),
					static_cast<uint8_t>(tokens[1].type),
					L"Incorrect Type! Expected an EQUAL type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::INTEGER),
					static_cast<uint8_t>(tokens[2].type),
					L"Incorrect type! Expected an INTEGER type"
				);
			}

			TEST_METHOD(TokenizeMultipleStatements)
			{
				std::string statement = R"(
				count = 1
				flags=2
			)";

				std::stringstream stream(statement, std::ios::in);

				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Make sure that we have three tokens
				// for this test statement
				Assert::AreEqual(static_cast<size_t>(6), tokens.size());

				// Next, make sure that each token is
				// the right type
				//
				// The order should be, identifier,
				// equals, integer
				//
				// And since there is two equal statements,
				// they should be identical
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[0].type),
					L"Incorrect type! Expected an IDENTIFIER type."
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::EQUAL),
					static_cast<uint8_t>(tokens[1].type),
					L"Incorrect Type! Expected an EQUAL type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::INTEGER),
					static_cast<uint8_t>(tokens[2].type),
					L"Incorrect type! Expected an INTEGER type"
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[3].type),
					L"Incorrect type! Expected an IDENTIFIER type."
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::EQUAL),
					static_cast<uint8_t>(tokens[4].type),
					L"Incorrect Type! Expected an EQUAL type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::INTEGER),
					static_cast<uint8_t>(tokens[5].type),
					L"Incorrect type! Expected an INTEGER type"
				);
			}

			TEST_METHOD(TokenizeFunction)
			{
				std::string code = R"(
					count = 1
				
					add_to_counter = (value, other)
					{
						count = count + value
					}
				)";

				std::stringstream stream(code, std::ios::in);

				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Make sure that we have three tokens
				// for this test statement
				Assert::AreEqual(static_cast<size_t>(17), tokens.size());

				// This is going to be long, make sure that
				// each type is accounted for, including
				// the count definition above the function
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[0].type),
					L"Incorrect type! Expected IDENTIFIER!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::EQUAL),
					static_cast<uint8_t>(tokens[1].type),
					L"Incorrect type! Expected EQUAL!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::INTEGER),
					static_cast<uint8_t>(tokens[2].type),
					L"Incorrect type! Expected INTEGER!"
				);

				// Go on to checking the types for the
				// function
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[3].type),
					L"Incorrect type! Expected IDENTIFIER!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::EQUAL),
					static_cast<uint8_t>(tokens[4].type),
					L"Incorrect type! Expected IDENTIFIER!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::BEGIN_ARGS),
					static_cast<uint8_t>(tokens[5].type),
					L"Incorrect type! Expected BEGIN_ARGS!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[6].type),
					L"Incorrect type! Expected IDENTIFIER!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::ARG_SEPERATOR),
					static_cast<uint8_t>(tokens[7].type),
					L"Incorrect type! Expected ARG_SEPERATOR!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[8].type),
					L"Incorrect type! Expected IDENTIFIER!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::END_ARGS),
					static_cast<uint8_t>(tokens[9].type),
					L"Incorrect type! Expected END_ARGS!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::BEGIN_BLOCK),
					static_cast<uint8_t>(tokens[10].type),
					L"Incorrect type! Expected BEGIN_BLOCK!"
				);

				// Check that the assignment with adding
				// to the count variable is valid
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[11].type),
					L"Incorrect type! Expected IDENTIFIER!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::EQUAL),
					static_cast<uint8_t>(tokens[12].type),
					L"Incorrect type! Expected EQUAL!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[13].type),
					L"Incorrect type! Expected IDENTIFIER!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::PLUS),
					static_cast<uint8_t>(tokens[14].type),
					L"Incorrect type! Expected PLUS!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[15].type),
					L"Incorrect type! Expected IDENTIFIER!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::END_BLOCK),
					static_cast<uint8_t>(tokens[16].type),
					L"Incorrect type! Expected END_BLOCK!"
				);
			}

			TEST_METHOD(TokenizeUnaryIncrementDecrement)
			{
				std::string code = R"(
					count++
					
					count--
				)";

				std::stringstream stream(code, std::ios::in);

				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Make sure that we have four tokens
				// for this test statement
				Assert::AreEqual(static_cast<size_t>(4), tokens.size());
			
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[0].type),
					L"Incorrect token type at zero! Expected IDENTIFIER!"
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::INCREMENT),
					static_cast<uint8_t>(tokens[1].type),
					L"Incorrect token type at one! Expected INCREMENT!"
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[2].type),
					L"Incorrect token type at two! Expected IDENTIFIER!"
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::DECREMENT),
					static_cast<uint8_t>(tokens[3].type),
					L"Incorrect token type at three! Expected DECREMENT!"
				);
			}

			TEST_METHOD(TokenizeUnaryArithmeticEqual)
			{
				std::string code = R"(
					count += 1
					count -= 1
					count*=2
					count /= 4
				)";

				std::stringstream stream(code, std::ios::in);

				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Make sure that we have twelve tokens
				// for this test statement
				Assert::AreEqual(static_cast<size_t>(12), tokens.size());

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[0].type),
					L"Incorrect token type at zero! Expected IDENTIFIER!"
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::PLUS_EQUAL),
					static_cast<uint8_t>(tokens[1].type),
					L"Incorrect token type at one! Expected PLUS_EQUAL!"
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::INTEGER),
					static_cast<uint8_t>(tokens[2].type),
					L"Incorrect token type at two! Expected INTEGER!"
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[3].type),
					L"Incorrect token type at three! Expected IDENTIFIER!"
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::MINUS_EQUAL),
					static_cast<uint8_t>(tokens[4].type),
					L"Incorrect token type at four! Expected MINUS_EQUAL!"
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::INTEGER),
					static_cast<uint8_t>(tokens[5].type),
					L"Incorrect token type at five! Expected INTEGER!"
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[6].type),
					L"Incorrect token type at six! Expected IDENTIFIER!"
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::MULTIPLY_EQUAL),
					static_cast<uint8_t>(tokens[7].type),
					L"Incorrect token type at seven! Expected MULTIPLY_EQUAL!"
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::INTEGER),
					static_cast<uint8_t>(tokens[8].type),
					L"Incorrect token type at eight! Expected INTEGER!"
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[9].type),
					L"Incorrect token type at nine! Expected IDENTIFIER!"
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::DIVIDE_EQUAL),
					static_cast<uint8_t>(tokens[10].type),
					L"Incorrect token type at ten! Expected DIVIDE_EQUAL!"
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::INTEGER),
					static_cast<uint8_t>(tokens[11].type),
					L"Incorrect token type at eleven! Expected INTEGER!"
				);
			}

			TEST_METHOD(TokenizeSimpleString)
			{
				std::string statement = "name = \"Shakara\"";

				std::stringstream stream(statement, std::ios::in);

				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Make sure that we have three tokens
				// for this test statement
				Assert::AreEqual(static_cast<size_t>(3), tokens.size());

				// Next, make sure that each token is
				// the right type
				//
				// The order should be, identifier,
				// equals, string
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[0].type),
					L"Incorrect type! Expected an IDENTIFIER type."
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::EQUAL),
					static_cast<uint8_t>(tokens[1].type),
					L"Incorrect Type! Expected an EQUAL type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::STRING),
					static_cast<uint8_t>(tokens[2].type),
					L"Incorrect type! Expected an STRING type"
				);

				// The string token's value should be Shakara
				Assert::AreEqual(
					"Shakara",
					tokens[2].value.c_str(),
					L"Incorrect value! Should be \"Shakara\""
				);
			}

			TEST_METHOD(TokenizeEscapedString)
			{
				std::string statement = R"(name = "Shakara \"Language\"")";

				std::stringstream stream(statement, std::ios::in);

				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Make sure that we have three tokens
				// for this test statement
				Assert::AreEqual(static_cast<size_t>(3), tokens.size());

				// Next, make sure that each token is
				// the right type
				//
				// The order should be, identifier,
				// equals, string
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[0].type),
					L"Incorrect type! Expected an IDENTIFIER type."
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::EQUAL),
					static_cast<uint8_t>(tokens[1].type),
					L"Incorrect Type! Expected an EQUAL type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::STRING),
					static_cast<uint8_t>(tokens[2].type),
					L"Incorrect type! Expected an STRING type"
				);

				// The string token's value should be Shakara
				Assert::AreEqual(
					"Shakara \"Language\"",
					tokens[2].value.c_str(),
					LR"(Incorrect value! Should be "Shakara \"Language\""!)"
				);
			}

			TEST_METHOD(TokenizePrintString)
			{
				std::string statement = R"(
					print("This is Shakara")
				)";

				std::stringstream stream(statement, std::ios::in);

				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Make sure that we have three tokens
				// for this test statement
				Assert::AreEqual(static_cast<size_t>(4), tokens.size());

				// Next, make sure that each token is
				// the right type
				//
				// The order should be identifier, begin args
				// string, and end args
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[0].type),
					L"Incorrect type! Expected an IDENTIFIER type."
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::BEGIN_ARGS),
					static_cast<uint8_t>(tokens[1].type),
					L"Incorrect type! Expected an BEGIN_ARGS type."
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::STRING),
					static_cast<uint8_t>(tokens[2].type),
					L"Incorrect Type! Expected an STRING type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::END_ARGS),
					static_cast<uint8_t>(tokens[3].type),
					L"Incorrect Type! Expected an END_ARGS type!"
				);

				// The string token's value should be Shakara
				Assert::AreEqual(
					"This is Shakara",
					tokens[2].value.c_str(),
					LR"(Incorrect value! Should be "This is Shakara"!)"
				);
			}

			TEST_METHOD(TokenizeReturnString)
			{
				std::string statement = R"(return "Returned value")";

				std::stringstream stream(statement, std::ios::in);

				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Make sure that we have three tokens
				// for this test statement
				Assert::AreEqual(static_cast<size_t>(2), tokens.size());

				// Next, make sure that each token is
				// the right type
				//
				// The order should be return and then
				// a string
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::RETURN),
					static_cast<uint8_t>(tokens[0].type),
					L"Incorrect type! Expected an PRINT type."
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::STRING),
					static_cast<uint8_t>(tokens[1].type),
					L"Incorrect Type! Expected an STRING type!"
				);

				// The string token's value should be Shakara
				Assert::AreEqual(
					"Returned value",
					tokens[1].value.c_str(),
					LR"(Incorrect value! Should be "Returned value"!)"
				);
			}

			TEST_METHOD(TokenizeIfBoolean)
			{
				std::string statement = R"(
					has_name = true
					name     = "Maxwell"

					if (has_name && name == "Maxwell" || name == "Jack")
					{
						name += "!"
					}
				)";

				std::stringstream stream(statement, std::ios::in);

				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Make sure that we have three tokens
				// for this test statement
				Assert::AreEqual(static_cast<size_t>(23), tokens.size());

				// Next, make sure that each token is
				// the right type
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[0].type),
					L"Incorrect type! Expected an IDENTIFIER type."
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::EQUAL),
					static_cast<uint8_t>(tokens[1].type),
					L"Incorrect Type! Expected an EQUAL type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::BOOLEAN),
					static_cast<uint8_t>(tokens[2].type),
					L"Incorrect Type! Expected a BOOLEAN type!"
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[3].type),
					L"Incorrect type! Expected an IDENTIFIER type."
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::EQUAL),
					static_cast<uint8_t>(tokens[4].type),
					L"Incorrect Type! Expected an EQUAL type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::STRING),
					static_cast<uint8_t>(tokens[5].type),
					L"Incorrect Type! Expected a STRING type!"
				);

				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IF_STATEMENT),
					static_cast<uint8_t>(tokens[6].type),
					L"Incorrect type! Expected an IF_STATEMENT type."
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::BEGIN_ARGS),
					static_cast<uint8_t>(tokens[7].type),
					L"Incorrect Type! Expected a BEGIN_ARGS type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[8].type),
					L"Incorrect Type! Expected an IDENTIFIER type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::AND),
					static_cast<uint8_t>(tokens[9].type),
					L"Incorrect Type! Expected an AND type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[10].type),
					L"Incorrect Type! Expected an IDENTIFIER type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::EQUAL_COMPARISON),
					static_cast<uint8_t>(tokens[11].type),
					L"Incorrect Type! Expected an EQUAL_COMPARISON type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::STRING),
					static_cast<uint8_t>(tokens[12].type),
					L"Incorrect Type! Expected a STRING type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::OR),
					static_cast<uint8_t>(tokens[13].type),
					L"Incorrect Type! Expected an OR type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[14].type),
					L"Incorrect Type! Expected an IDENTIFIER type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::EQUAL_COMPARISON),
					static_cast<uint8_t>(tokens[15].type),
					L"Incorrect Type! Expected an EQUAL_COMPARISON type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::STRING),
					static_cast<uint8_t>(tokens[16].type),
					L"Incorrect Type! Expected a STRING type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::END_ARGS),
					static_cast<uint8_t>(tokens[17].type),
					L"Incorrect Type! Expected an END_ARGS type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::BEGIN_BLOCK),
					static_cast<uint8_t>(tokens[18].type),
					L"Incorrect Type! Expected a BEGIN_BLOCK type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[19].type),
					L"Incorrect Type! Expected an IDENTIFIER type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::PLUS_EQUAL),
					static_cast<uint8_t>(tokens[20].type),
					L"Incorrect Type! Expected a PLUS_EQUAL type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::STRING),
					static_cast<uint8_t>(tokens[21].type),
					L"Incorrect Type! Expected a STRING type!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::END_BLOCK),
					static_cast<uint8_t>(tokens[22].type),
					L"Incorrect Type! Expected a END_BLOCK type!"
				);
			}

			TEST_METHOD(TokenizeWhile)
			{
				std::string statement = R"(
					while (condition)
					{
						print(condition)
					}
				)";

				std::stringstream stream(statement, std::ios::in);

				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Make sure that we have three tokens
				// for this test statement
				Assert::AreEqual(static_cast<size_t>(10), tokens.size());

				// Next, make sure that each token is
				// the right type
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::WHILE_STATEMENT),
					static_cast<uint8_t>(tokens[0].type),
					L"Incorrect token type! Expected WHILE_STATEMENT!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::BEGIN_ARGS),
					static_cast<uint8_t>(tokens[1].type),
					L"Incorrect token type! Expected BEGIN_ARGS!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[2].type),
					L"Incorrect token type! Expected IDENTIFIER!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::END_ARGS),
					static_cast<uint8_t>(tokens[3].type),
					L"Incorrect token type! Expected END_ARGS!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::BEGIN_BLOCK),
					static_cast<uint8_t>(tokens[4].type),
					L"Incorrect token type! Expected BEGIN_BLOCK!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[5].type),
					L"Incorrect token type! Expected IDENTIFIER!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::BEGIN_ARGS),
					static_cast<uint8_t>(tokens[6].type),
					L"Incorrect token type! Expected BEGIN_ARGS!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[7].type),
					L"Incorrect token type! Expected IDENTIFIER!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::END_ARGS),
					static_cast<uint8_t>(tokens[8].type),
					L"Incorrect token type! Expected END_ARGS!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::END_BLOCK),
					static_cast<uint8_t>(tokens[9].type),
					L"Incorrect token type! Expected END_BLOCK!"
				);
			}

			TEST_METHOD(TokenizeArrayBrackets)
			{
				std::string statement = R"(
					array = []
				)";

				std::stringstream stream(statement, std::ios::in);

				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Make sure that we have three tokens
				// for this test statement
				Assert::AreEqual(static_cast<size_t>(4), tokens.size());

				// Next, make sure that each token is
				// the right type
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[0].type),
					L"Incorrect token type! Expected IDENTIFIER!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::EQUAL),
					static_cast<uint8_t>(tokens[1].type),
					L"Incorrect token type! Expected EQUAL!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::LEFT_BRACKET),
					static_cast<uint8_t>(tokens[2].type),
					L"Incorrect token type! Expected LEFT_BRACKET!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::RIGHT_BRACKET),
					static_cast<uint8_t>(tokens[3].type),
					L"Incorrect token type! Expected RIGHT_BRACKET!"
				);
			}

			TEST_METHOD(TokenizeForeach)
			{
				std::string statement = R"(
					foreach (collection)
					{
					}
				)";

				std::stringstream stream(statement, std::ios::in);

				std::vector<Shakara::Token> tokens;

				Shakara::Tokenizer tokenizer;
				tokenizer.Tokenize(stream, tokens);

				// Make sure that we have three tokens
				// for this test statement
				Assert::AreEqual(static_cast<size_t>(6), tokens.size());

				// Next, make sure that each token is
				// the right type
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::FOREACH_STATEMENT),
					static_cast<uint8_t>(tokens[0].type),
					L"Incorrect token type! Expected FOREACH_STATEMENT!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::BEGIN_ARGS),
					static_cast<uint8_t>(tokens[1].type),
					L"Incorrect token type! Expected BEGIN_ARGS!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::IDENTIFIER),
					static_cast<uint8_t>(tokens[2].type),
					L"Incorrect token type! Expected IDENTIFIER!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::END_ARGS),
					static_cast<uint8_t>(tokens[3].type),
					L"Incorrect token type! Expected END_ARGS!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::BEGIN_BLOCK),
					static_cast<uint8_t>(tokens[4].type),
					L"Incorrect token type! Expected BEGIN_BLOCK!"
				);
				Assert::AreEqual(
					static_cast<uint8_t>(Shakara::TokenType::END_BLOCK),
					static_cast<uint8_t>(tokens[5].type),
					L"Incorrect token type! Expected END_BLOCK!"
				);
			}

		};
	}
}