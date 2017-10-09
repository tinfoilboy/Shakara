#pragma once

namespace std
{
	class cout;
}

namespace Shakara
{
	namespace AST
	{
		class Node;

		class RootNode;

		class FunctionCall;

		class AssignmentNode;

		class BinaryOperation;

		class FunctionDeclaration;

		class WhileStatement;

		class BooleanNode;

		class IfStatement;
		
		class ArrayNode;

		class ArrayElementIdentifierNode;
	}

	/**
	 * A structure containing a map of variables to identifiers,
	 * as well as a parent scope.
	 *
	 * Allows for recursive scope searching, and dynamic scope
	 * creation.
	 */
	struct Scope
	{
		Scope*                                  parent = nullptr;
		std::map<const std::string, AST::Node*> variables;

		~Scope();

		/**
		 * Attempt to insert a node into the current scope.
		 *
		 * Will also check the parent's ability to update
		 * beforehand.
		 */
		void Insert(const std::string& identifier, AST::Node* node);

		/**
		 * Attempt to update a node upward.
		 *
		 * If no update is made, false is returned.
		 */
		bool Update(const std::string& identifier, AST::Node* node);

		AST::Node* Search(const std::string& identifier);

		AST::Node* operator[](const std::string& identifier)
		{
			return variables[identifier];
		}
	};

	class Interpreter
	{
	public:
		Interpreter();

		Interpreter(std::ostream& output);

		void ErrorHandler(std::function<void()> handler)
		{
			m_errorHandle = handler;
		}

		/**
		 * Pass in a AST root to start interpreting
		 * and actually executing code.
		 *
		 * The final three arguments are used for
		 * executing function bodies, and thus can
		 * be ignored for the end-user.
		 */
		void Execute(
			AST::RootNode* root,
			bool           function=false,
			AST::Node**    returned=nullptr,
			Scope*         scope=nullptr
		);

		/**
		 * Append a single argument to this interpreter
		 * before executing
		 */
		inline void AppendCommandArgument(const char* argument)
		{
			m_arguments.push_back(argument);
		}

	private:
		/**
		 * Arguments to be pushed off to the
		 * Shakara file being interpreted.
		 */
		std::vector<const char*> m_arguments;

		/**
		 * Output stream used for printing
		 */
		std::ostream& m_output;

		/**
		 * Callback function to be used for
		 * error handling
		 */
		std::function<void()> m_errorHandle;

		/**
		 * The global scope for each assignment and definition.
		 */
		Scope m_globalScope;
		
		/**
		 * Take in an assignment node, and attempt to
		 * assign (and subsequently store) a variable.
		 *
		 * The local and scope arguments are used for
		 * functions, as to insert variables into local
		 * scope, preferably.
		 */
		void _ExecuteAssign(
			AST::AssignmentNode* assign,
			Scope&               scope
		);

		/**
		 * Take in an if statement node, evaluate its condition
		 * and subsequently, if true, run its body.
		 *
		 * This function takes in whether or not its run within
		 * a function, as well as a returned node and a scope.
		 *
		 * The returned node is only for within functions, and is
		 * passed back to the function if not null.
		 */
		void _ExecuteIfStatement(
			AST::IfStatement* statement,
			bool              function,
			AST::Node**       returned,
			Scope&            scope
		);

		/**
		 * Take in a while statement node, evaluate its condition
		 * and, if false, run the body, and keep running until the
		 * condition evaluates to true.
		 *
		 * This function takes in whether or not its run within
		 * a function, as well as a returned node and a scope.
		 *
		 * The returned node is only for within functions, and is
		 * passed back to the function if not null.
		 */
		void _ExecuteWhileStatement(
			AST::WhileStatement* statement,
			bool              function,
			AST::Node**       returned,
			Scope&            scope
		);

		/**
		 * Take in a function declaration node and store
		 * it as a variable in global scope.
		 */
		void _ExecuteFunctionDeclaration(AST::FunctionDeclaration* declaration);

		/**
		 * Take in an array definition and evaluate all of the nodes within
		 * and subsequently store it in the passed in scope.
		 */
		void _ExecuteArrayDeclaration(
			AST::AssignmentNode* arrayAssignment,
			Scope&               scope
		);

		/**
		 * Take in a function call node, and attempt to
		 * assign parameters and run the function.
		 *
		 * Returns a Node that represents the return of
		 * the function, can be nullptr.
		 */
		AST::Node* _ExecuteFunction(
			AST::FunctionCall* call,
			Scope&             scope
		);

		/**
		 * Take in a special print function call and
		 * print out the arguments
		 *
		 * The scope argument is used for functions
		 * to grab variables defined within.
		 */
		void _ExecutePrint(
			AST::FunctionCall* print,
			Scope&             scope
		);

		/**
		 * Take in a special type function call and
		 * return the string representation of the
		 * type.
		 *
		 * The scope argument is used for functions
		 * to grab variables defined within.
		 */
		AST::Node* _ExecuteType(
			AST::FunctionCall* type,
			Scope&             scope
		);

		/**
		 * Take in a special amount function call and
		 * return the integer size of the collection.
		 *
		 * The scope argument is used for functions
		 * to grab variables defined within.
		 */
		AST::Node* _ExecuteAmount(
			AST::FunctionCall* amount,
			Scope&             scope
		);

		/**
		 * Take in a special exit function call and
		 * close out of the current program. An optional
		 * code can also be passed in.
		 *
		 * The scope argument is used for functions
		 * to grab variables defined within.
		 */
		AST::Node* _ExecuteExit(
			AST::FunctionCall* exitCall,
			Scope&             scope
		);

		/**
		 * Take in a special amount function call and
		 * return the integer size of the collection.
		 *
		 * The scope argument is used for functions
		 * to grab variables defined within.
		 */
		AST::Node* _ExecutePush(
			AST::FunctionCall* push,
			Scope&             scope
		);

		/**
		 * Take a special pop function with the arguments
		 * of an array and the index to pop
		 *
		 * The scope argument is used for functions
		 * to grab variables defined within.
		 */
		AST::Node* _ExecutePop(
			AST::FunctionCall* pop,
			Scope&             scope
		);

		/**
		 * Take in any kind of built-in type value
		 * and try to cast it to an integer.
		 *
		 * The scope argument is used for functions
		 * to grab variables defined within.
		 */
		AST::Node* _ExecuteIntegerCast(
			AST::FunctionCall* caster,
			Scope&             scope
		);

		/**
		 * Take in any kind of built-in type value
		 * and try to cast it to a decimal.
		 *
		 * The scope argument is used for functions
		 * to grab variables defined within.
		 */
		AST::Node* _ExecuteDecimalCast(
			AST::FunctionCall* caster,
			Scope&             scope
		);

		/**
		 * Take in any kind of built-in type value
		 * and try to cast it to a string.
		 *
		 * The scope argument is used for functions
		 * to grab variables defined within.
		 */
		AST::Node* _ExecuteStringCast(
			AST::FunctionCall* caster,
			Scope&             scope
		);

		/**
	 	 * Take in any kind of built-in type value
		 * and try to cast it to a boolean.
		 *
		 * The scope argument is used for functions
		 * to grab variables defined within.
		 */
		AST::Node* _ExecuteBooleanCast(
			AST::FunctionCall* caster,
			Scope&             scope
		);

		/**
		 * Checks the type of a node and prints it
		 * accordingly.
		 */
		void _PrintTypedNode(
			AST::Node* node,
			bool       stringQuotes=false,
			bool       commaAfter=false
		);

		/**
		 * Execute a binary operation and return a
		 * singular Node from it
		 */
		AST::Node* _ExecuteBinaryOperation(
			AST::BinaryOperation* operation,
			Scope&                scope
		);

		/**
		 * Execute a logical operation and return a
		 * singular BooleanNode from it
		 */
		AST::BooleanNode* _ExecuteLogicalOperation(
			AST::BinaryOperation* operation,
			Scope&                scope
		);

		/**
		 * Grab a single node from an array element
		 * identifier
		 */
		AST::Node* _GetArrayElement(
			AST::ArrayElementIdentifierNode* identifier,
			Scope&                           scope
		);

		/**
		 * Create an array instance for the command
		 * arguments and insert it into global scope.
		 */
		void _CreateCommandArgumentsArray();

	};
}