#include "../stdafx.hpp"
#include "ASTBuilder.hpp"

#include "ASTTypes.hpp"
#include "ASTNode.hpp"
#include "Nodes/ASTAssignmentNode.hpp"
#include "Nodes/ASTIntegerNode.hpp"
#include "Nodes/ASTBinaryOperation.hpp"
#include "Nodes/ASTIdentifierNode.hpp"
#include "Nodes/ASTRootNode.hpp"
#include "Nodes/ASTFunctionDeclarationNode.hpp"
#include "Nodes/ASTFunctionCallNode.hpp"
#include "Nodes/ASTDecimalNode.hpp"
#include "Nodes/ASTStringNode.hpp"
#include "Nodes/ASTReturnNode.hpp"
#include "Nodes/ASTBooleanNode.hpp"
#include "Nodes/ASTIfStatementNode.hpp"
#include "Nodes/ASTWhileStatementNode.hpp"
#include "Nodes/ASTArrayNode.hpp"
#include "Nodes/ASTArrayElementIdentifier.hpp"

#include "../Tokenizer/TokenizerTypes.hpp"

using namespace Shakara;
using namespace Shakara::AST;

void ASTBuilder::Build(
	RootNode*           root,
	std::vector<Token>& tokens,
	size_t              index
)
{
	// Build a new node from the tokens at
	// the current index
	ptrdiff_t next = index + 1;

	_BuildIndividualNode(
		root,
		tokens,
		index,
		&next
	);

	// We still have more portions of the AST to build
	// thus, continue building
	//
	// Otherwise, do some memory saving by resizing
	// the RootNode vector to fit each child without
	// any extra space
	//
	// Though, stop recursively building once we hit
	// a end block token if the flag is set
	if ((next > 0 && static_cast<size_t>(next) < tokens.size()))
		Build(
			root,
			tokens,
			next
		);
	else
		root->ShrinkToFit();
}

bool ASTBuilder::_BuildIndividualNode(
	RootNode*           root,
	std::vector<Token>& tokens,
	size_t              index,
	ptrdiff_t*          next
)
{
	const Token& token = tokens[index];

	// Check if there is anything to do with
	// an identifier first, as most portions
	// of the language deal with identifiers
	if (token.type == TokenType::IDENTIFIER)
	{
		// Check if there's an equal sign
		// after this identifier, and if
		// so, it's an assignment
		if (tokens[index + 1].type == TokenType::EQUAL)
		{
			// Attempt to parse either a function definition
			// or a variable assignment
			//
			// Differentiated by whether or not BEGIN_ARGS is
			// after the equal sign or not
			if (tokens[index + 2].type == TokenType::BEGIN_ARGS)
			{
				_ParseFunctionDefinition(
					root,
					tokens,
					index,
					next
				);
			
				return false;
			}
			else
			{
				_ParseVariableAssignment(
					root,
					tokens,
					index,
					next
				);
			
				return true;
			}
		}
		// For either increment or decrement variables
		else if (_IsIncrementDecrementToken(tokens[index + 1].type))
		{
			_ParseVariableIncrementDecrement(
				root,
				tokens,
				index,
				next
			);

			return true;
		}
		// For any assignment arithmetic operators
		// such as += or -=
		else if (_IsArithmeticAssignmentToken(tokens[index + 1].type))
		{
			_ParseVariableArithmeticAssignment(
				root,
				tokens,
				index,
				next
			);

			return true;
		}
		// Check if this could be a function call
		else if (tokens[index + 1].type == TokenType::BEGIN_ARGS)
		{
			_ParseFunctionCall(
				root,
				tokens,
				index,
				next
			);

			return true;
		}
		// Check if this could be an array element assignment
		//
		// This will create a node for the index of the array
		// that will then be passed to the parse method if it
		// turns out to be an array assignment
		else if (tokens[index + 1].type == TokenType::LEFT_BRACKET)
		{
			ArrayElementIdentifierNode* arrayElement = new ArrayElementIdentifierNode();
			arrayElement->Type(NodeType::ARRAY_ELEMENT_IDENTIFIER);
			
			_ParseArrayElementIdentifierNode(
				arrayElement,
				tokens,
				index,
				next
			);

			// If we have a value and the next token is a right bracket,
			// and the token after that is an equal sign, start parsing
			// the array element assignment
			if (
				arrayElement                                            &&
				tokens[*next].type        == TokenType::RIGHT_BRACKET   &&
				tokens[(*next) + 1].type  == TokenType::EQUAL
			)
			{
				_ParseVariableAssignment(
					root,
					tokens,
					(*next) + 1,
					next,
					arrayElement
				);

				return true;
			}
			else if (
				arrayElement                                     &&
				tokens[*next].type == TokenType::RIGHT_BRACKET   &&
				_IsIncrementDecrementToken(tokens[(*next) + 1].type)
			)
			{
				_ParseVariableIncrementDecrement(
					root,
					tokens,
					(*next) + 1,
					next,
					arrayElement
				);

				return true;
			}
			else if (
				arrayElement                                     &&
				tokens[*next].type == TokenType::RIGHT_BRACKET   &&
				_IsArithmeticAssignmentToken(tokens[(*next) + 1].type)
			)
			{
				_ParseVariableArithmeticAssignment(
					root,
					tokens,
					(*next) + 1,
					next,
					arrayElement
				);

				return true;
			}
			else
			{
				delete arrayElement;

				return false;
			}
		}
	}
	// Parse a return statement
	else if (tokens[index].type == TokenType::RETURN)
	{
		_ParseReturnStatement(
			root,
			tokens,
			index,
			next
		);

		return true;
	
	}
	// Parse an if statement
	else if (tokens[index].type == TokenType::IF_STATEMENT)
	{
		_ParseIfStatement(
			root,
			tokens,
			index,
			next
		);
		
		return true;
	}
	// Parse a while statement
	else if (tokens[index].type == TokenType::WHILE_STATEMENT)
	{
		_ParseWhileStatement(
			root,
			tokens,
			index,
			next
		);

		return true;
	}

	return false;
}

void ASTBuilder::_ParseVariableAssignment(
	RootNode*           root,
	std::vector<Token>& tokens,
	size_t              index,
	ptrdiff_t*			next,
	Node*               identifier
)
{
	// Start by setting the next token
	// as the index passed in
	*next = index;

	// Create the assignment node to be
	// added to the AST
	AssignmentNode* assignment = new AssignmentNode();
	assignment->Type(NodeType::ASSIGN);

	// Create a new identifier from the
	// first token if we don't already
	// have an identifier
	if (!identifier)
	{
		identifier = new IdentifierNode();
		identifier->Type(NodeType::IDENTIFIER);

		static_cast<IdentifierNode*>(identifier)->Value(tokens[(*next)].value);
	
		// Move onto the equal sign
		(*next)++;
	}

	identifier->Parent(assignment);
	assignment->Identifier(identifier);

	// Move past the equal sign
	(*next)++;

	Node* value = _GetPassableNode(
		tokens,
		*next,
		next
	);

	value->Parent(assignment);
	assignment->Assignment(value);

	root->Insert(assignment);
}

void ASTBuilder::_ParseReturnStatement(
	RootNode*           root,
	std::vector<Token>& tokens,
	size_t              index,
	ptrdiff_t*			next
)
{
	// Start by setting the next token
	// as the index passed in
	*next = index;

	// Create the assignment node to be
	// added to the AST
	ReturnNode* retStatement = new ReturnNode();
	retStatement->Type(NodeType::RETURN);

	// Move on to the data that is going to
	// be returned
	(*next)++;

	Node* value = _GetPassableNode(
		tokens,
		*next,
		next
	);

	value->Parent(retStatement);
	retStatement->Returned(value);

	root->Insert(retStatement);
}

void ASTBuilder::_ParseIfStatement(
	RootNode*           root,
	std::vector<Token>& tokens,
	size_t              index,
	ptrdiff_t*          next
)
{
	IfStatement* statement = new IfStatement();
	statement->Type(NodeType::IF_STATEMENT);

	_ParseIfStatement(
		statement,
		tokens,
		index,
		next
	);

	root->Insert(statement);
}

void ASTBuilder::_ParseIfStatement(
	IfStatement*        statement,
	std::vector<Token>& tokens,
	size_t              index,
	ptrdiff_t*          next
)
{
	// Start by setting the next token
	// as the index passed in
	*next = index;

	// Move on to trying to parse the conditions
	(*next)++;

	// Move on if this token is the beginning
	// of the conditions
	if (tokens[*next].type == TokenType::BEGIN_ARGS)
		(*next)++;

	Node* value = _GetPassableNode(
		tokens,
		*next,
		next
	);

	value->Parent(statement);
	statement->Condition(value);

	// If we are at the end of the condition, move on
	if (tokens[*next].type == TokenType::END_ARGS)
		(*next)++;

	// Once we are done with the condition, check if there's a
	// BEGIN_BLOCK token at the current location and if so
	// start parsing the if statement block
	if (tokens[*next].type == TokenType::BEGIN_BLOCK)
	{
		RootNode* body = new RootNode();

		(*next)++;

		// Now, like with functions, do a while loop
		// to build an if body
		while (static_cast<size_t>((*next)) < tokens.size())
		{
			if (tokens[*next].type == TokenType::END_BLOCK)
			{
				(*next)++;

				break;
			}

			// Attempt to build a new node for the
			// function, if one is not able to be
			// made, continue to the next token
			if (!_BuildIndividualNode(
				body,
				tokens,
				*next,
				next
			))
				(*next)++;
		}

		// Now add the body statements to the declaration
		// and add the declaration to the root
		statement->Body(body);
	}
	// This might be a little bit funky but, I usually omit braces
	// in an if statement if it is only one line, so therefore, I'm
	// going to make it so that, if there is no BEGIN_BLOCK, a statement
	// will be yanked ahead and put into the body, this could be a cause
	// for errors in a user's code, but that's more operator error, I feel
	else
	{
		RootNode* body = new RootNode();

		bool madeNew = _BuildIndividualNode(
			body,
			tokens,
			*next,
			next
		);

		if (!madeNew)
			(*next)++;

		// Set the body to the root node with
		// only one expression inside and then
		// continue
		statement->Body(body);
	}

	// If we have an else/else if to parse
	bool hasElse = false;

	// Now, check if there is an else keyword, and if there
	// is, move on to the next keyword
	if (static_cast<size_t>((*next)) < tokens.size() && tokens[*next].type == TokenType::ELSE_STATEMENT)
	{
		(*next)++;

		hasElse = true;
	}

	// This must be an else if in this case, thus make
	// a new if statement node and attach it to the
	// current if statement
	if (static_cast<size_t>((*next)) < tokens.size() && tokens[*next].type == TokenType::IF_STATEMENT && hasElse)
	{
		IfStatement* elseIf = new IfStatement();
		elseIf->Type(NodeType::IF_STATEMENT);

		_ParseIfStatement(
			elseIf,
			tokens,
			*next,
			next
		);

		statement->ElseIfCondition(elseIf);
	}
	// Otherwise, see if there is a begin block and
	// if so, parse the block
	else if (static_cast<size_t>((*next)) < tokens.size() && tokens[*next].type == TokenType::BEGIN_BLOCK && hasElse)
	{
		RootNode* body = new RootNode();

		(*next)++;

		// Now, like with functions, do a while loop
		// to build an else body
		while (static_cast<size_t>((*next)) < tokens.size())
		{
			if (tokens[*next].type == TokenType::END_BLOCK)
			{
				(*next)++;

				break;
			}

			// Attempt to build a new node for the
			// function, if one is not able to be
			// made, continue to the next token
			if (!_BuildIndividualNode(
				body,
				tokens,
				*next,
				next
			))
				(*next)++;
		}

		// Now, set the else body to that of the
		// generated body
		statement->ElseBlock(body);
	}
	// Otherwise, just parse a single statement and
	// use that as the else block
	else if (hasElse)
	{
		RootNode* body = new RootNode();

		bool madeNew = _BuildIndividualNode(
			body,
			tokens,
			*next,
			next
		);

		if (!madeNew)
			(*next)++;

		// Set the body to the root node with
		// only one expression inside and then
		// continue
		statement->ElseBlock(body);
	}
}

void ASTBuilder::_ParseWhileStatement(
	RootNode*           root,
	std::vector<Token>& tokens,
	size_t              index,
	ptrdiff_t*          next
)
{
	// Start by setting the next token
	// as the index passed in
	*next = index;

	// Create the while statement node for
	// the root node
	WhileStatement* whileStatement = new WhileStatement();
	whileStatement->Type(NodeType::WHILE_STATEMENT);

	// Move on to trying to parse the conditions
	(*next)++;

	// Move on if this token is the beginning
	// of the conditions
	if (tokens[*next].type == TokenType::BEGIN_ARGS)
		(*next)++;

	// Parse a node to be used as the condition
	// for the while statement
	Node* value = _GetPassableNode(
		tokens,
		*next,
		next
	);

	value->Parent(whileStatement);
	whileStatement->Condition(value);

	// If we are at the end of the condition, move on
	if (tokens[*next].type == TokenType::END_ARGS)
		(*next)++;

	RootNode* body = new RootNode();

	// Once we are done with the condition, check if there's a
	// BEGIN_BLOCK token at the current location
	if (tokens[*next].type == TokenType::BEGIN_BLOCK)
	{
		(*next)++;

		// Now, like with functions, do a while loop
		// to build an if body
		while (static_cast<size_t>((*next)) < tokens.size())
		{
			if (tokens[*next].type == TokenType::END_BLOCK)
			{
				(*next)++;

				break;
			}

			// Attempt to build a new node for the
			// function, if one is not able to be
			// made, continue to the next token
			if (!_BuildIndividualNode(
				body,
				tokens,
				*next,
				next
			))
			(*next)++;
		}

		// Now add the body statements to the declaration
		// and add the declaration to the root
		whileStatement->Body(body);

		root->Insert(whileStatement);
	}
	// This might be a little bit funky but, I usually omit braces
	// in an if statement if it is only one line, so therefore, I'm
	// going to make it so that, if there is no BEGIN_BLOCK, a statement
	// will be yanked ahead and put into the body, this could be a cause
	// for errors in a user's code, but that's more operator error, I feel
	else
	{
		bool madeNew = _BuildIndividualNode(
			body,
			tokens,
			*next,
			next
		);

		if (!madeNew)
			(*next)++;

		// Set the body to the root node with
		// only one expression inside and then
		// continue
		whileStatement->Body(body);

		root->Insert(whileStatement);
	}
}

Node* ASTBuilder::_ParseLogicalOperation(
	std::vector<Token>& tokens,
	size_t              index,
	ptrdiff_t*          next
)
{
	// Start by setting the next token
	// as the index passed in
	*next = index;

	// Now what we should first try and
	// build a passable node of sorts,
	// once that's done, figure out if
	// we actually have to create a
	// logical operation
	Node* value = _GetPassableNode(
		tokens,
		*next,
		next,
		true
	);

	// Now, we check and see if the next
	// token is either an AND or an OR
	//
	// If it is, create a LogicalOperation
	// and set the left side to the value
	// and the right side to a nested
	// _ParseLogicalOperation
	//
	// Otherwise, just return the value grabbed
	if (
		static_cast<size_t>(*next) < tokens.size() &&
		(tokens[*next].type == TokenType::AND ||
		tokens[*next].type == TokenType::OR)
	)
	{
		// Move next so that it will parse the first
		// non logical node
		(*next)++;

		// The underlying type is a BinaryOperation, but it
		// is a special LOGICAL_OP type, that differentiates
		// it in the interpretation stage
		BinaryOperation* operation = new BinaryOperation();
		operation->Type(NodeType::LOGICAL_OP);

		value->Parent(operation);
		operation->LeftHand(value);
		
		// Set the operation type based on the token type
		if (tokens[(*next) - 1].type == TokenType::AND)
			operation->Operation(NodeType::ADD);
		else
			operation->Operation(NodeType::OR);

		// Get the right hand of the operation from
		// the nested call to this function
		Node* right = _ParseLogicalOperation(
			tokens,
			*next,
			next
		);
		right->Parent(operation);

		operation->RightHand(right);

		return operation;
	}

	return value;
}

void ASTBuilder::_ParseFunctionCall(
	RootNode*           root,
	std::vector<Token>& tokens,
	size_t              index,
	ptrdiff_t*			next
)
{
	FunctionCall* call = new FunctionCall();
	call->Type(NodeType::CALL);

	_ParseFunctionCall(
		call,
		tokens,
		index,
		next
	);

	root->Insert(call);
}

void ASTBuilder::_ParseFunctionCall(
	FunctionCall*       call,
	std::vector<Token>& tokens,
	size_t              index,
	ptrdiff_t*			next
)
{
	// Start by setting the next token
	// as the index passed in
	*next = index;

	// Set the identifier as the first token
	// at the passed in index
	IdentifierNode* identifier = new IdentifierNode();
	identifier->Type(NodeType::IDENTIFIER);

	identifier->Value(tokens[(*next)].value);

	identifier->Parent(call);

	call->Identifier(identifier);

	// Set up the special call flags based on the
	// identifier name
	call->SetFlags(_GetCallFlagFromIdentifier(tokens[(*next)].value));

	// We know that there's a begin args
	// after this identifier, so just
	// run next twice to skip it
	(*next)++;
	(*next)++;

	// Now, we have a while loop to try
	// and grab each argument inside of
	// the call
	while (static_cast<size_t>((*next)) < tokens.size())
	{
		if (tokens[*next].type == TokenType::END_ARGS)
		{
			(*next)++;

			break;
		}

		if (tokens[*next].type == TokenType::ARG_SEPERATOR)
		{
			(*next)++;

			continue;
		}

		Node* value = _GetPassableNode(
			tokens,
			*next,
			next
		);

		value->Parent(call);
		call->InsertArgument(value);
	}
}

void ASTBuilder::_ParseVariableIncrementDecrement(
	RootNode*           root,
	std::vector<Token>& tokens,
	size_t              index,
	ptrdiff_t*          next,
	Node*               identifier
)
{
	// Start by setting the next token
	// as the index passed in
	*next = index;

	// Create an assignment node to increment
	// the variable
	//
	// Technically I could create an increment
	// or deincrement node, but it seems nicer
	// to me to just implement it as an assignment
	// to a binary operation
	AssignmentNode* assignment = new AssignmentNode();
	assignment->Type(NodeType::ASSIGN);

	// Create an identifier node if one is not
	// already created
	if (!identifier)
	{
		identifier = new IdentifierNode();
		identifier->Type(NodeType::IDENTIFIER);

		static_cast<IdentifierNode*>(identifier)->Value(tokens[(*next)].value);

		(*next)++;
	}

	identifier->Parent(assignment);
	assignment->Identifier(identifier);

	// Create a separate increment decrement operation
	// for this variable decrement increment
	BinaryOperation* operation = _ParseIncrementDecrementOperation(
		identifier,
		tokens,
		next
	);

	operation->Parent(assignment);

	assignment->Assignment(operation);

	root->Insert(assignment);
}

BinaryOperation* ASTBuilder::_ParseIncrementDecrementOperation(
	Node*               identifier,
	std::vector<Token>& tokens,
	ptrdiff_t*          next
)
{
	// Create the binary operation for this
	// operation
	BinaryOperation* operation = new BinaryOperation();
	operation->Type(NodeType::BINARY_OP);

	// Create the left hand side of the operation
	// with a copy of the current identifier
	//
	// This is kind of a hacky workaround, as each
	// node recursively deletes, and will try to
	// double delete if I use the same identifier
	// instance
	Node* leftIdentifier = identifier->Clone();
	leftIdentifier->Parent(operation);

	operation->LeftHand(leftIdentifier);

	// Figure out if this is a increment or
	// a decrement so that I can assign a
	// correct binary operation
	if (tokens[*next].type == TokenType::INCREMENT)
		operation->Operation(NodeType::ADD);
	else if (tokens[*next].type == TokenType::DECREMENT)
		operation->Operation(NodeType::SUBTRACT);

	// Move on for the next call
	(*next)++;

	// Create the right hand of the binary operation
	//
	// Initialize this to one as we are only incrementing
	// or decrementing by one
	IntegerNode* one = new IntegerNode();
	one->Type(NodeType::INTEGER);
	one->Value(false, 1);
	one->Parent(operation);

	operation->RightHand(one);

	return operation;
}

void ASTBuilder::_ParseVariableArithmeticAssignment(
	RootNode*           root,
	std::vector<Token>& tokens,
	size_t              index,
	ptrdiff_t*          next,
	Node*               identifier
)
{
	// Start by setting the next token
	// as the index passed in
	*next = index;

	// Create an assignment node to preform a
	// math operation on the variable
	//
	// The reason for using an AssignmentNode
	// as opposed to a new node type for these
	// operators is for code and project simplicity
	AssignmentNode* assignment = new AssignmentNode();
	assignment->Type(NodeType::ASSIGN);

	// Create an identifier if one is not passed
	if (!identifier)
	{
		identifier = new IdentifierNode();
		identifier->Type(NodeType::IDENTIFIER);

		static_cast<IdentifierNode*>(identifier)->Value(tokens[(*next)].value);

		(*next)++;
	}

	identifier->Parent(assignment);
	assignment->Identifier(identifier);

	// Create the binary operation for this
	// operation
	BinaryOperation* operation = _ParseArithmeticAssignmentOperation(
		identifier,
		tokens,
		next
	);
	operation->Parent(assignment);

	assignment->Assignment(operation);

	root->Insert(assignment);
}

BinaryOperation* ASTBuilder::_ParseArithmeticAssignmentOperation(
	Node*               identifier,
	std::vector<Token>& tokens,
	ptrdiff_t*          next
)
{
	// Create the binary operation for this
	// operation
	BinaryOperation* operation = new BinaryOperation();
	operation->Type(NodeType::BINARY_OP);

	// Create the left hand side of the operation
	// with a copy of the current identifier
	//
	// This is kind of a hacky workaround, as each
	// node recursively deletes, and will try to
	// double delete if I use the same identifier
	// instance
	Node* leftIdentifier = identifier->Clone();
	leftIdentifier->Parent(operation);

	operation->LeftHand(leftIdentifier);

	// Figure out if this is a increment or
	// a decrement so that I can assign a
	// correct binary operation
	if (tokens[*next].type == TokenType::PLUS_EQUAL)
		operation->Operation(NodeType::ADD);
	else if (tokens[*next].type == TokenType::MINUS_EQUAL)
		operation->Operation(NodeType::SUBTRACT);
	else if (tokens[*next].type == TokenType::MULTIPLY_EQUAL)
		operation->Operation(NodeType::MULTIPLY);
	else if (tokens[*next].type == TokenType::DIVIDE_EQUAL)
		operation->Operation(NodeType::DIVIDE);
	else if (tokens[*next].type == TokenType::MODULUS_EQUAL)
		operation->Operation(NodeType::DIVIDE);

	// Move on for the next call
	(*next)++;

	Node* value = _GetPassableNode(
		tokens,
		*next,
		next
	);
	value->Parent(operation);
	operation->RightHand(value);

	return operation;
}

void ASTBuilder::_ParseFunctionDefinition(
	RootNode*           root,
	std::vector<Token>& tokens,
	size_t              index,
	ptrdiff_t*          next
)
{
	// Start by setting the next token
	// as the index passed in
	*next = index;

	// Create the function declaration node
	// to be added to the root
	FunctionDeclaration* declaration = new FunctionDeclaration();
	declaration->Type(NodeType::FUNCTION);

	// Set the identifier using the
	// index as the first node
	IdentifierNode* identifier = new IdentifierNode();
	identifier->Type(NodeType::IDENTIFIER);

	identifier->Value(tokens[*next].value);

	identifier->Parent(declaration);

	declaration->Identifier(identifier);

	// Move on to trying to parse arguments
	(*next)++;

	// Move on if this token is an equal sign
	if (tokens[*next].type == TokenType::EQUAL)
		(*next)++;

	// Move on if this token is the beginning
	// of the args
	if (tokens[*next].type == TokenType::BEGIN_ARGS)
		(*next)++;

	// Start parsing arguments by using a
	// while loop until an END_ARGS is found
	// or until the end of the tokens
	while (static_cast<size_t>((*next)) < tokens.size())
	{
		if (tokens[*next].type == TokenType::END_ARGS)
		{
			(*next)++;

			break;
		}

		// Function definition arguments can only contain
		// identifiers!
		if (tokens[*next].type != TokenType::IDENTIFIER)
		{
			(*next)++;

			continue;
		}

		IdentifierNode* argument = new IdentifierNode();
		argument->Type(NodeType::IDENTIFIER);

		argument->Value(tokens[*next].value);

		argument->Parent(declaration);

		declaration->InsertArgument(argument);

		(*next)++;
	}

	// Once we are done with the arguments, check if there's a
	// BEGIN_BLOCK token at the current location
	if (tokens[*next].type == TokenType::BEGIN_BLOCK)
		(*next)++;

	// Now, we can do the same kind of while loop for the
	// arguments, but for statements within the body
	RootNode* body = new RootNode();
	
	while (static_cast<size_t>((*next)) < tokens.size())
	{
		if (tokens[*next].type == TokenType::END_BLOCK)
			break;

		// Attempt to build a new node for the
		// function, if one is not able to be
		// made, continue to the next token
		if (!_BuildIndividualNode(
			body,
			tokens,
			*next,
			next
		))
			(*next)++;
	}

	// Now add the body statements to the declaration
	// and add the declaration to the root
	declaration->Body(body);

	root->Insert(declaration);
}

Node* ASTBuilder::_GetPassableNode(
	std::vector<Token>& tokens,
	size_t              index,
	ptrdiff_t*          next,
	bool                ignoreLogic
)
{
	// Start by setting the next token
	// as the index passed in
	*next = index;
	
	// Start with a check to see if the tokens would create a function call
	// 
	// Subsequently, check if it could be a binary operation of any type
	// 
	// After the binary operation, check if this could be an array definition
	if (
		static_cast<size_t>((*next) + 1) < tokens.size()  &&
		tokens[(*next)].type     == TokenType::IDENTIFIER &&
		tokens[(*next) + 1].type == TokenType::BEGIN_ARGS
	)
	{
		FunctionCall* call = new FunctionCall();
		call->Type(NodeType::CALL);

		_ParseFunctionCall(
			call,
			tokens,
			*next,
			next
		);

		// Parse a binary operation if a op token
		// is found
		if (
			static_cast<size_t>((*next)) < tokens.size() &&
			IsBinaryType(tokens[*next].type)
		)
			return _GetPassableBinaryOperation(
				tokens,
				*next,
				next,
				ignoreLogic,
				call
			);

		if (
			static_cast<size_t>((*next)) < tokens.size() &&
			(tokens[*next].type == TokenType::AND || tokens[*next].type == TokenType::OR) &&
			!ignoreLogic
		)
		{
			delete call;

			Node* logical = _ParseLogicalOperation(
				tokens,
				index,
				next
			);

			return logical;
		}

		return call;
	}
	else if (
		static_cast<size_t>((*next) + 1) < tokens.size() &&
		IsBinaryType(tokens[(*next) + 1].type)
	)
	{
		return _GetPassableBinaryOperation(
			tokens,
			index,
			next,
			ignoreLogic
		);
	}
	// Parse as an array definition
	else if (tokens[(*next)].type == TokenType::LEFT_BRACKET)
	{
		ArrayNode* arrayNode = new ArrayNode();
		arrayNode->Type(NodeType::ARRAY);

		_ParseArrayNode(
			arrayNode,
			tokens,
			*next,
			next
		);

		if (
			static_cast<size_t>((*next)) < tokens.size() &&
			(tokens[*next].type == TokenType::AND || tokens[*next].type == TokenType::OR) &&
			!ignoreLogic
		)
		{
			delete arrayNode;

			Node* logical = _ParseLogicalOperation(
				tokens,
				index,
				next
			);

			return logical;
		}

		return arrayNode;
	}
	// Parse as an array access
	else if (
		static_cast<size_t>((*next) + 1) < tokens.size()  &&
		tokens[(*next)].type     == TokenType::IDENTIFIER &&
		tokens[(*next + 1)].type == TokenType::LEFT_BRACKET
	)
	{
		ArrayElementIdentifierNode* identifier = new ArrayElementIdentifierNode();
		identifier->Type(NodeType::ARRAY_ELEMENT_IDENTIFIER);

		_ParseArrayElementIdentifierNode(
			identifier,
			tokens,
			*next,
			next
		);

		// Move past the right bracket
		if (tokens[*next].type == TokenType::RIGHT_BRACKET)
			(*next)++;

		// Parse this element identifier as being
		// a part of a binary operation
		if (
			static_cast<size_t>((*next)) < tokens.size() &&
			IsBinaryType(tokens[*next].type)
		)
			return _GetPassableBinaryOperation(
				tokens,
				*next,
				next,
				ignoreLogic,
				identifier
			);

		if (
			static_cast<size_t>((*next)) < tokens.size() &&
			(tokens[*next].type == TokenType::AND || tokens[*next].type == TokenType::OR) &&
			!ignoreLogic
		)
		{
			delete identifier;

			Node* logical = _ParseLogicalOperation(
				tokens,
				index,
				next
			);

			return logical;
		}

		return identifier;
	}
	else if (
		tokens[*next].type == TokenType::IDENTIFIER ||
		tokens[*next].type == TokenType::INTEGER    ||
		tokens[*next].type == TokenType::DECIMAL    ||
		tokens[*next].type == TokenType::STRING     ||
		tokens[*next].type == TokenType::BOOLEAN
	)
	{
		Node* value = nullptr;
		_CreateSingleNodeFromToken(
			&value,
			tokens[(*next)]
		);

		(*next)++;
	
		if (
			static_cast<size_t>((*next)) < tokens.size() &&
			(tokens[*next].type == TokenType::AND || tokens[*next].type == TokenType::OR) &&
			!ignoreLogic
		)
		{
			delete value;

			Node* logical = _ParseLogicalOperation(
				tokens,
				index,
				next
			);

			return logical;
		}

		return value;
	}

	return nullptr;
}

Node* ASTBuilder::_GetPassableBinaryOperation(
	std::vector<Token>& tokens,
	size_t              index,
	ptrdiff_t*          next,
	bool                ignoreLogic,
	Node*               leftHand
)
{
	BinaryOperation* operation = new BinaryOperation();
	operation->Type(NodeType::BINARY_OP);

	_ParseBinaryOperation(
		operation,
		tokens,
		*next,
		next,
		leftHand
	);

	// Try and find comparison operations inside
	// of this binary operation
	Node* currentHand = operation;

	while (currentHand->Type() == NodeType::BINARY_OP)
	{
		BinaryOperation* op = static_cast<BinaryOperation*>(currentHand);

		// If we've found something that we need to convert to have
		// a correct left and right hand, start doing it
		if (
			IsLogicalOperation(op->Operation()) &&
			op->Parent() != nullptr   &&
			op->Parent()->Type() == NodeType::BINARY_OP
		)
		{
			// The new operation to return
			BinaryOperation* overall = new BinaryOperation();
			overall->Type(NodeType::BINARY_OP);
			overall->Operation(op->Operation());

			// The overall operation to perform for the
			// left of the comparison, will also depend
			// on if there are binary operation parents
			// to the parent
			BinaryOperation* leftOp = new BinaryOperation();
			leftOp->Parent(overall);
			leftOp->Type(NodeType::BINARY_OP);
			leftOp->LeftHand(static_cast<BinaryOperation*>(op->Parent())->GetLeftHand()->Clone());

			// Since we cloned it, delete the original node
			delete static_cast<BinaryOperation*>(op->Parent())->GetLeftHand();

			leftOp->GetLeftHand()->Parent(leftOp);
			leftOp->Operation(static_cast<BinaryOperation*>(op->Parent())->Operation());
			leftOp->RightHand(op->GetLeftHand()->Clone());

			// Again, delete the original node
			delete op->GetLeftHand();
			op->LeftHand(nullptr);

			leftOp->GetRightHand()->Parent(leftOp);

			overall->LeftHand(leftOp);
			overall->RightHand(op->GetRightHand()->Clone());

			// Delete the final cloned node
			delete op->GetRightHand();
			op->RightHand(nullptr);

			overall->GetRightHand()->Parent(overall);

			if (op->Parent()->Parent() && op->Parent()->Parent()->Type() == NodeType::BINARY_OP)
			{
				BinaryOperation* parentOp = static_cast<BinaryOperation*>(op->Parent()->Parent());
				overall->Parent(overall);

				parentOp->RightHand(overall);

				delete op;

				break;
			}
			else
			{
				delete op;

				return overall;
			}
		}

		currentHand = op->GetRightHand();
	}

	if (
		static_cast<size_t>((*next)) < tokens.size() &&
		(tokens[*next].type == TokenType::AND || tokens[*next].type == TokenType::OR) &&
		!ignoreLogic
	)
	{
		delete operation;

		Node* logical = _ParseLogicalOperation(
			tokens,
			index,
			next
		);

		return logical;
	}

	return operation;
}

void ASTBuilder::_ParseBinaryOperation(
	BinaryOperation*    operation,
	std::vector<Token>& tokens,
	size_t              index,
	ptrdiff_t*          next,
	Node*               leftHand
)
{
	// Start by setting the next token
	// as the index passed in
	*next = index;

	// Determine the type of node
	// to create for the left hand
	// side of the operation
	if (!leftHand)
	{
		_CreateSingleNodeFromToken(
			&leftHand,
			tokens[*next]
		);
		
		// Move to the op type from
		// the converted node
		(*next)++;
	}

	leftHand->Parent(operation);
	operation->LeftHand(leftHand);

	// Now get the operation type
	const TokenType& type = tokens[*next].type;
	NodeType         op   = NodeType::ROOT;

	switch (type)
	{
	case TokenType::PLUS:
		op = NodeType::ADD;
		break;
	case TokenType::MINUS:
		op = NodeType::SUBTRACT;
		break;
	case TokenType::MULTIPLY:
		op = NodeType::MULTIPLY;
		break;
	case TokenType::DIVIDE:
		op = NodeType::DIVIDE;
		break;
	case TokenType::MODULUS:
		op = NodeType::MODULUS;
		break;
	case TokenType::EQUAL_COMPARISON:
		op = NodeType::EQUAL_COMPARISON;
		break;
	case TokenType::NOTEQUAL_COMPARISON:
		op = NodeType::NOTEQUAL_COMPARISON;
		break;
	case TokenType::LESS_COMPARISON:
		op = NodeType::LESS_COMPARISON;
		break;
	case TokenType::GREATER_COMPARISON:
		op = NodeType::GREATER_COMPARISON;
		break;
	case TokenType::LESSEQUAL_COMPARISON:
		op = NodeType::LESSEQUAL_COMPARISON;
		break;
	case TokenType::GREATEREQUAL_COMPARISON:
		op = NodeType::GREATEREQUAL_COMPARISON;
		break;
	default:
		break;
	}

	operation->Operation(op);

	(*next)++;

	// Ignore parsing a LogicalOperation within
	// this binary operation, otherwise, it messes
	// with the flow of the operation, with the logical
	// operation becoming the right hand as opposed to
	// the actual other part of the operation
	Node* value = _GetPassableNode(
		tokens,
		*next,
		next,
		true
	);

	value->Parent(operation);
	operation->RightHand(value);
}

void ASTBuilder::_ParseArrayNode(
	ArrayNode*          arrayNode,
	std::vector<Token>& tokens,
	size_t              index,
	ptrdiff_t*          next
)
{
	// Start by setting the next token
	// as the index passed in
	*next = index;

	// Move on to the right bracket
	// or any other type that could
	// be there
	(*next)++;

	// If we actually have a non ending
	// node here, parse as a passable
	// node for a fixed size
	if (tokens[*next].type != TokenType::RIGHT_BRACKET)
	{
		Node* capacity = _GetPassableNode(
			tokens,
			*next,
			next
		);

		arrayNode->Fixed(true);
		arrayNode->Capacity(capacity);
	}

	// Continue past the right bracket
	(*next)++;

	// If we don't have elements defined
	// for the array, just return out
	if (tokens[*next].type != TokenType::BEGIN_BLOCK)
		return;

	// Otherwise, we have to parse these
	// elements, move on to the first
	(*next)++;

	// Parse this the same as say, a function
	// call's arguments
	//
	// Move through each node until either the
	// end is reached, or we run out of tokens
	while (static_cast<size_t>((*next)) < tokens.size())
	{
		if (tokens[*next].type == TokenType::END_BLOCK)
		{
			(*next)++;

			break;
		}

		if (tokens[*next].type == TokenType::ARG_SEPERATOR)
		{
			(*next)++;

			continue;
		}

		Node* value = _GetPassableNode(
			tokens,
			*next,
			next
		);

		arrayNode->Insert(value);
	}
}

void ASTBuilder::_ParseArrayElementIdentifierNode(
	ArrayElementIdentifierNode* identifier,
	std::vector<Token>&         tokens,
	size_t                      index,
	ptrdiff_t*                  next
)
{
	// Start by setting the next token
	// as the index passed in
	*next = index;
	
	// First, parse the identifier string
	// for the array element node
	//
	// Since we only call this if the first token
	// is an identifier, just grab the token value
	identifier->ArrayIdentifier(tokens[*next].value);

	// Move on past the identifier and the left
	// bracket to get to the index
	(*next)++;
	(*next)++;

	// Now parse the index as a passable node
	Node* element = _GetPassableNode(
		tokens,
		*next,
		next
	);

	identifier->Index(element);
}

void ASTBuilder::_CreateSingleNodeFromToken(
	Node**       node,
	const Token& token
)
{
	switch (token.type)
	{
	case TokenType::IDENTIFIER:
	{
		*node = new IdentifierNode();
		static_cast<IdentifierNode*>(*node)->Value(token.value);

		(*node)->Type(NodeType::IDENTIFIER);

		break;
	}
	case TokenType::INTEGER:
	{
		*node = new IntegerNode();
		static_cast<IntegerNode*>(*node)->Value(false, static_cast<int32_t>(std::stoi(token.value)));

		(*node)->Type(NodeType::INTEGER);

		break;
	}
	case TokenType::DECIMAL:
	{
		*node = new DecimalNode();
		static_cast<DecimalNode*>(*node)->Value(false, static_cast<float>(std::stof(token.value)));

		(*node)->Type(NodeType::DECIMAL);

		break;
	}
	case TokenType::STRING:
	{
		*node = new StringNode();
		static_cast<StringNode*>(*node)->Value(token.value);

		(*node)->Type(NodeType::STRING);

		break;
	}
	case TokenType::BOOLEAN:
	{
		*node = new BooleanNode();
		static_cast<BooleanNode*>(*node)->Value(token.value == "true");

		(*node)->Type(NodeType::BOOLEAN);

		break;
	}
	default:
		break;
	}
}

bool ASTBuilder::_IsIncrementDecrementToken(const TokenType& type)
{
	return (type == TokenType::INCREMENT) ||
		   (type == TokenType::DECREMENT);
}

bool ASTBuilder::_IsArithmeticAssignmentToken(const TokenType& type)
{
	return (type == TokenType::PLUS_EQUAL)     ||
		   (type == TokenType::MINUS_EQUAL)    ||
		   (type == TokenType::MULTIPLY_EQUAL) ||
		   (type == TokenType::DIVIDE_EQUAL)   ||
		   (type == TokenType::MODULUS_EQUAL);
}

CallFlags ASTBuilder::_GetCallFlagFromIdentifier(const std::string& identifier)
{
	if (identifier == "print")
		return CallFlags::PRINT;
	else if (identifier == "type")
		return CallFlags::TYPE;
	else if (identifier == "amt")
		return CallFlags::AMOUNT;
	else if (identifier == "exit")
		return CallFlags::EXIT;
	else if (identifier == "integer")
		return CallFlags::INTEGER_CAST;
	else if (identifier == "decimal")
		return CallFlags::DECIMAL_CAST;
	else if (identifier == "string")
		return CallFlags::STRING_CAST;
	else if (identifier == "boolean")
		return CallFlags::BOOLEAN_CAST;
	else if (identifier == "push")
		return CallFlags::PUSH_COLLECTION;
	else if (identifier == "pop")
		return CallFlags::POP_COLLECTION;

	return CallFlags::NONE;
}