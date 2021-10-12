#include "operations.h"
#include "parse_tree.h"

namespace kbl
{
	const char* HumanReadableTokenType::TokenTypeToString(TokenType type)
	{
		KBL_ASSERT((size_t)TokenType::TOTAL == 5, "Exhaustive handling of TokenType in TokenTypeToString");
		switch (type)
		{
		case TokenType::OPERAND:	return HumanReadableTokenType::OPERAND;
		case TokenType::CONSTANT:	return HumanReadableTokenType::CONSTANT;
		case TokenType::KEYWORD:	return HumanReadableTokenType::KEYWORD;
		case TokenType::LITERAL:	return HumanReadableTokenType::LITERAL;
		case TokenType::OPERATOR:	return HumanReadableTokenType::OPERATOR;
		default:					KBL_ASSERT(false, "Unhandled TokenType in TokenTypeToString"); return "";
		}
	}

	const char* HumanReadableConstantType::TokenTypeToString(ConstantType type)
	{
		KBL_ASSERT((size_t)ConstantType::TOTAL == 1, "Exhaustive handling of ConstantType in TokenTypeToString");
		switch (type)
		{
		case ConstantType::NUMBER:		return HumanReadableConstantType::NUMBER;
		default:						KBL_ASSERT(false, "Unhandled TokenType in ", __FUNCTION__); return "";
		}
	}

	LiteralType HumanReadableLiteral::StringToLiteralType(const char* word)
	{
		KBL_ASSERT((size_t)LiteralType::TOTAL == 1, "Exhaustive handling of literal in ", __FUNCTION__);
		std::string_view word_view = { word };
		if (word_view == std::string_view{ HumanReadableLiteral::I32 })
			return LiteralType::I32;
		else
		{
			KBL_ERROR("Unknown text \'", word, "\' passed to ", __FUNCTION__);
			return LiteralType::NONE;
		}
	}

	KeywordType HumanReadableKeyword::StringToOperatorType(const char* word)
	{
		KBL_ASSERT((size_t)KeywordType::TOTAL == 2, "Exhaustive handling of keywords in ", __FUNCTION__);
		std::string_view word_view = { word };
		if (word_view == std::string_view{ HumanReadableKeyword::LET })
			return KeywordType::LET;
		else if (word_view == std::string_view{ HumanReadableKeyword::PRINT })
			return KeywordType::PRINT;
		else
		{
			KBL_ERROR("Unknown text \'", word, "\' passed to ", __FUNCTION__);
			return KeywordType::NONE;
		}
	}

	OperatorType HumanReadableOperator::StringToTokenType(const char* word)
	{
		KBL_ASSERT((size_t)OperatorType::TOTAL == 3, "Exhaustive handling of OperatorType in ", __FUNCTION__);

		std::string_view word_view = { word };
		if (word_view == std::string_view{ HumanReadableOperator::ASSIGMENT })
			return OperatorType::ASSIGMENT;
		else if (word_view == std::string_view{ HumanReadableOperator::PLUS })
			return OperatorType::PLUS;
		else if (word_view == std::string_view{ HumanReadableOperator::END_OF_STATEMENT })
			return OperatorType::END_OF_STATEMENT;
		else
		{
			KBL_ERROR("Unknown text \'", word, "\' passed to ", __FUNCTION__);
			return OperatorType::NONE;
		}

	}

	std::string HumanReadableOpCode::OpCodeToString(OpCode code)
	{
		KBL_ASSERT((size_t)OpCode::TOTAL == 8, "Exhaustive handling of OpCode in ", __FUNCTION__);

		switch (code)
		{
		case OpCode::STORE_OFFSET:					return HumanReadableOpCode::STORE_OFFSET;
		case OpCode::STORE_IM:						return HumanReadableOpCode::STORE_IM;
		case OpCode::STORE_FROM_STACK:				return HumanReadableOpCode::STORE_FROM_STACK;
		case OpCode::LOAD_OFFSET_TO_STACK:			return HumanReadableOpCode::LOAD_OFFSET_TO_STACK;
		case OpCode::STACK_ADD_OFF_TO_OFF:			return HumanReadableOpCode::STACK_ADD_OFF_TO_OFF;
		case OpCode::STACK_ADD_IM_TO_IM:			return HumanReadableOpCode::STACK_ADD_IM_TO_IM;
		case OpCode::STACK_ADD_IM_TO_OFF:			return HumanReadableOpCode::STACK_ADD_IM_TO_OFF;
		case OpCode::PRINT_FROM_OFFSET:				return HumanReadableOpCode::PRINT_FROM_OFFSET;
		case OpCode::NONE:							KBL_ASSERT("None type OpCode detected!"); return "";
		default:									KBL_ASSERT("unreachable"); return "";
		}
	}
}