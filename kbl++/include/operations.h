#ifndef KBL_OPERATIONS_H
#define KBL_OPERATIONS_H

#include "core.h"
#include <iostream>
#include <string>

namespace kbl
{
	enum class OpCode
	{
		STORE_OFFSET = 0,			// offset, &
		STORE_IM,					// offset, immediate 
		STORE_FROM_STACK,			// offset, sp (passed in automatically)
		LOAD_OFFSET_TO_STACK,		// offset
		STACK_ADD_OFF_TO_OFF,		// offset, offset
		STACK_ADD_IM_TO_OFF,		// offset, immediate 
		STACK_ADD_IM_TO_IM,			// immediate, immediate
		PRINT_FROM_OFFSET,			// offset

		TOTAL,
		NONE
	};

	enum class IntrinsicCode
	{
		PLUS = 0,
		MINUS,
		EQ,

		TOTAL
	};

	enum class LiteralType
	{
		I32,

		TOTAL,
		NONE
	};

	enum class ConstantType
	{
		NUMBER = 0,

		TOTAL,
		NONE
	};

	enum class TokenType
	{
		OPERAND = 0,
		CONSTANT,
		LITERAL,
		KEYWORD,
		OPERATOR,

		TOTAL,
		NONE
	};

	enum class OperatorType
	{
		ASSIGMENT = 0,
		PLUS,
		END_OF_STATEMENT,

		TOTAL,
		NONE
	};

	enum class KeywordType
	{
		LET = 0,
		PRINT,

		TOTAL,
		NONE
	};

	// #TODO introduce operand types, Variable, Literal, Macro?

	struct HumanReadableTokenType
	{
		static constexpr const char* OPERAND	= "operand";
		static constexpr const char* CONSTANT	= "constant";
		static constexpr const char* LITERAL	= "literal";
		static constexpr const char* KEYWORD	= "keyword";
		static constexpr const char* OPERATOR	 = "operator";

		static const char* TokenTypeToString(TokenType type);
	};

	struct HumanReadableConstantType
	{
		static constexpr const char* NUMBER = "number";

		static const char* TokenTypeToString(ConstantType type);
	};

	struct HumanReadableLiteral
	{
		static constexpr const char* I32 = "i32";

		static LiteralType StringToLiteralType(const char* word);
	};

	struct HumanReadableKeyword
	{
		static constexpr const char* LET = "let";
		static constexpr const char* PRINT = "print";

		static KeywordType StringToOperatorType(const char* word);
	};

	struct HumanReadableOperator
	{
		static constexpr const char* PLUS = "+";
		static constexpr const char* ASSIGMENT = "<-";
		static constexpr const char* END_OF_STATEMENT = ";";

		static OperatorType StringToTokenType(const char* word);
	};

	struct HumanReadableOpCode
	{
		static constexpr const char* STORE_OFFSET = "store (offset, address) [stores value from address to offset address]";
		static constexpr const char* STORE_IM = "store immediate (offset, im_val) [store immediate value at offset]";
		static constexpr const char* STORE_FROM_STACK = "store from stack (offset) [stores value from stack to offset]";
		static constexpr const char* LOAD_OFFSET_TO_STACK = "load (offset) [loads value from offset to the stack]";
		static constexpr const char* STACK_ADD_OFF_TO_OFF = "add (offset, offset) [adds values from offsets and places value on stack]";
		static constexpr const char* STACK_ADD_IM_TO_IM = "add immediate (im_val, im_val) [adds immediate values and places value on stack]";
		static constexpr const char* STACK_ADD_IM_TO_OFF = "add immediate and offset (offset, im_val) [adds immediate value to value of offset and places value on stack]";
		static constexpr const char* PRINT_FROM_OFFSET = "print (offset) [prints value at offset]";

		static std::string OpCodeToString(OpCode code);
	};

	inline bool IsTextLiteral(const char* word)
	{
		KBL_ASSERT((size_t)LiteralType::TOTAL == 1, "Exhaustive handling of literals in IsTextLiteral");
		std::string_view word_view = { word };

		if (word_view == std::string_view{ HumanReadableLiteral::I32 })
			return true;
		else
			return false;
	}

	inline bool IsTextKeyword(const char* word)
	{
		KBL_ASSERT((size_t)(KeywordType::TOTAL) == 2, "Exhaustive handling of keywords in IsTextKeyword");
		std::string_view word_view = { word };

		if (word_view == std::string_view{ HumanReadableKeyword::LET })
			return true;
		else if (word_view == std::string_view{ HumanReadableKeyword::PRINT })
			return true;
		else
			return false;
	}

	inline bool IsTextOperator(const char* word)
	{
		KBL_ASSERT((size_t)(OperatorType::TOTAL) == 3, "Exhaustive handling of operators in IsTextOperator");
		std::string_view word_view = { word };

		if (word_view == std::string_view{ HumanReadableOperator::ASSIGMENT })
			return true;
		else if (word_view == std::string_view{ HumanReadableOperator::PLUS })
			return true;
		else if (word_view == std::string_view{ HumanReadableOperator::END_OF_STATEMENT })
			return true;
		else
			return false;
	}

	inline bool IsTextOperand(const char* word)
	{
		return !(IsTextKeyword(word) | IsTextKeyword(word) | IsTextLiteral(word));
	}
}

#endif