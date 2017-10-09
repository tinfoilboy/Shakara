#pragma once

#include "targetver.hpp"
#include "CppUnitTest.h"

#include <Windows.h>

#include <vector>
#include <string>
#include <sstream>
#include <functional>
#include <map>

#include "../Shakara/Tokenizer/TokenizerTypes.hpp"
#include "../Shakara/Tokenizer/Tokenizer.hpp"

#include "../Shakara/AST/ASTNode.hpp"
#include "../Shakara/AST/ASTTypes.hpp"
#include "../Shakara/AST/ASTBuilder.hpp"
#include "../Shakara/AST/Nodes/ASTRootNode.hpp"
#include "../Shakara/AST/Nodes/ASTAssignmentNode.hpp"
#include "../Shakara/AST/Nodes/ASTBinaryOperation.hpp"
#include "../Shakara/AST/Nodes/ASTFunctionDeclarationNode.hpp"
#include "../Shakara/AST/Nodes/ASTFunctionCallNode.hpp"
#include "../Shakara/AST/Nodes/ASTStringNode.hpp"
#include "../Shakara/AST/Nodes/ASTDecimalNode.hpp"
#include "../Shakara/AST/Nodes/ASTArrayNode.hpp"
#include "../Shakara/AST/Nodes/ASTArrayElementIdentifier.hpp"
#include "../Shakara/AST/Nodes/ASTForeachStatementNode.hpp"

#include "../Shakara/Interpreter/Interpreter.hpp"