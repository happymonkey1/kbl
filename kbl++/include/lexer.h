#ifndef KBL_LEXER_H
#define KBL_LEXER_H

#include "operations.h"
#include "tokens.h"

#include <iostream>
#include <string>
#include <vector>
#include <filesystem>
#include <string_view>

namespace kbl
{
	class Lexer
	{
	public:
		Lexer(const std::filesystem::path& filepath);
		~Lexer() = default;

	private:
		void Tokenize(const std::string& data);
		void TokenizeLine(const std::string& line, size_t row);
		void LexTokens();
		template <typename PredicateT>
		size_t FindColumn(const std::string& line, size_t start, PredicateT predicate);
		void Run();
		std::filesystem::path m_filepath;
		size_t m_line_count;
		std::vector<std::vector<RawToken>> m_lines_of_raw_tokens;

		friend class Parser;
	};

	template <typename PredicateT>
	size_t Lexer::FindColumn(const std::string& line, size_t start, PredicateT predicate)
	{
		size_t len = line.size();
		while (start < len && !predicate(line[start]))
			start += 1;
		return start;
	}

}

#endif