#pragma once

namespace Shakara
{
	enum class TokenizeError : uint8_t
	{
		NONE       = 0x00,
		BAD_STREAM = 0x01
	};

	enum class TokenType : uint8_t
	{
		UNKNOWN                 = 0x00,
		IDENTIFIER              = 0x01,

		/**
		 * Arithmetic Tokens
		 */
		PLUS                    = 0x02,
		MINUS                   = 0x03,
		MULTIPLY                = 0x04,
		DIVIDE                  = 0x05,
		MODULUS                 = 0x06,
		EQUAL                   = 0x07,

		/**
		 * Semantic types
		 */
		BEGIN_ARGS              = 0x08,
		ARG_SEPERATOR           = 0x09,
		END_ARGS                = 0x0A,
		BEGIN_BLOCK             = 0x0B,
		END_BLOCK               = 0x0C,
		LEFT_BRACKET            = 0x0D,
		RIGHT_BRACKET           = 0x0E,

		/**
		 * Type Tokens
		 */
		INTEGER                 = 0x0F,
		DECIMAL                 = 0x10,
		STRING                  = 0x11,

		/**
		 * Unary Tokens
		 */
		INCREMENT               = 0x12,
		DECREMENT               = 0x13,
		PLUS_EQUAL              = 0x14,
		MINUS_EQUAL             = 0x15,
		MULTIPLY_EQUAL          = 0x16,
		DIVIDE_EQUAL            = 0x17,
		MODULUS_EQUAL           = 0x18,

		/**
		 * Specially used keywords, like
		 * return
		 */
		RETURN                  = 0x19,

		/**
		 * Boolean and Logical Tokens
		 */
		BOOLEAN                 = 0x1A,
		AND                     = 0x1B,
		OR                      = 0x1C,
		NOT                     = 0x1D,
		EQUAL_COMPARISON        = 0x1E,
		NOTEQUAL_COMPARISON     = 0x1F,
		LESS_COMPARISON         = 0x20,
		GREATER_COMPARISON      = 0x21,
		LESSEQUAL_COMPARISON    = 0x22,
		GREATEREQUAL_COMPARISON = 0x23,

		/**
		 * Statements
		 */
		IF_STATEMENT            = 0x24,
		ELSE_STATEMENT          = 0x25,
		WHILE_STATEMENT         = 0x26,
		FOREACH_STATEMENT       = 0x27
	};

	struct Token
	{
		TokenType   type   = TokenType::UNKNOWN;
		std::string value  = "";
		uint32_t    line   = 1;
		uint32_t    column = 0;
	};

	static inline bool IsBinaryType(const TokenType& type)
	{
		return (type == TokenType::PLUS)                 ||
			   (type == TokenType::MINUS)                ||
			   (type == TokenType::MULTIPLY)             ||
			   (type == TokenType::DIVIDE)               ||
			   (type == TokenType::MODULUS)              ||
			   (type == TokenType::EQUAL_COMPARISON)     ||
			   (type == TokenType::NOTEQUAL_COMPARISON)  ||
			   (type == TokenType::LESS_COMPARISON)      ||
			   (type == TokenType::GREATER_COMPARISON)   ||
			   (type == TokenType::LESSEQUAL_COMPARISON) ||
			   (type == TokenType::GREATEREQUAL_COMPARISON);
	}

	static inline bool IsUnaryType(const TokenType& type)
	{
		return (type == TokenType::INCREMENT)            ||
			   (type == TokenType::DECREMENT)            ||
			   (type == TokenType::PLUS_EQUAL)           ||
			   (type == TokenType::MINUS_EQUAL)          ||
			   (type == TokenType::MULTIPLY_EQUAL)       ||
			   (type == TokenType::DIVIDE_EQUAL)         ||
			   (type == TokenType::EQUAL_COMPARISON)     ||
			   (type == TokenType::NOTEQUAL_COMPARISON)  ||
			   (type == TokenType::LESS_COMPARISON)      ||     
			   (type == TokenType::GREATER_COMPARISON)   ||
			   (type == TokenType::LESSEQUAL_COMPARISON) ||
			   (type == TokenType::GREATEREQUAL_COMPARISON);
	}
}