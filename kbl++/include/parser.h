#ifndef KBL_PARSER_H
#define KBL_PARSER_H

#include "core.h"
#include "operations.h"
#include "lexer.h"
#include "parse_tree.h"
#include "tokens.h"

#include <unordered_map>

namespace kbl
{
	class Parser
	{
	public:
		Parser(const std::filesystem::path& filepath);
		~Parser() = default;

	private:
		void BuildTrees();
		TreeNode* ParseToken(TreeNode* root, const RawToken& token);
		void ParseTreeToOpToken(TreeNode* root, OpToken& op_token);

		OpCode ParseTreeExpressionOpCodeType(TreeNode* root);

		Lexer m_lexer;
		std::vector<ParseTree> m_trees;
		std::vector<OpToken> m_op_tokens;

		std::unordered_map<std::string, size_t> m_variable_map;
		size_t m_offset_count;

		friend class Program;
	};
}

#endif
