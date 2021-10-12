#ifndef KBL_TOKENS_H
#define KBL_TOKENS_H

#include "operations.h"
#include <string>
#include <vector>

// Forward decs
namespace kbl
{
	struct TreeNode;
}

namespace kbl
{
	struct RawToken
	{
		std::string Value;
		size_t Column;
		size_t Row;
		TokenType Type;

		RawToken(const std::string& v, size_t col, size_t row, TokenType type = TokenType::NONE)
			: Value{ v }, Column{ col }, Row{ row }, Type{ type } {}

		std::string string() const
		{
			return '(' + std::to_string(Row) + ":" + std::to_string(Column) + " \'" + Value + "\' : " + HumanReadableTokenType::TokenTypeToString(Type) + ')';
		}

		bool operator==(const RawToken& other) const noexcept
		{
			return Type == other.Type && Value == other.Value;
		}


		friend std::ostream& operator<<(std::ostream& out, const RawToken& token);
	};

	inline std::ostream& operator<<(std::ostream& out, const RawToken& token)
	{
		out << '(' << token.Row << ":" << token.Column << " \'" << token.Value << "\' : " << HumanReadableTokenType::TokenTypeToString(token.Type) << ')';
		return out;
	}

	class OpToken
	{
	public:
		OpCode Type;
		std::vector<int64_t> Args;

		OpToken();
		OpToken(OpCode code);
		~OpToken() = default;

		friend std::ostream& operator<<(std::ostream& out, const OpToken& token);
	};

	inline std::ostream& operator<<(std::ostream& out, const OpToken& op_token)
	{
		out << HumanReadableOpCode::OpCodeToString(op_token.Type) << " : (";
		size_t i = 0;
		for (const auto& arg : op_token.Args)
		{
			out << arg;
			if (i++ < op_token.Args.size() - 1)
				out << ", ";
		}
		out << ")";

		return out;
	}
}

#endif