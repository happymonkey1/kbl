#include "lexer.h"

#include "core.h"

#include <fstream>
#include <sstream>
#include <algorithm>



namespace kbl
{
	Lexer::Lexer(const std::filesystem::path& filepath)
		: m_filepath{ filepath }, m_line_count{ 0 }, m_lines_of_raw_tokens{}
	{

	}

	void Lexer::Tokenize(const std::string& data)
	{
		std::vector<std::string> lines = {};
		//constexpr const char NEW_LINE_DELIMETER = '\n';
		
		// Get lines from data
		size_t start = 0;
		size_t pos = data.find(HumanReadableOperator::END_OF_STATEMENT, 0);
		size_t line_count = 0;
		while (pos != std::string::npos)
		{
			std::string line = data.substr(start, pos - start + 1);
			// Remove new line
			//line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
			if (!line.empty())
				lines.push_back(line);

			start = pos + 1;
			pos = data.find(HumanReadableOperator::END_OF_STATEMENT, start);
			line_count++;
		}

		m_line_count = line_count;

		if (lines.size() == 0)
			KBL_ERROR("kbl file ", m_filepath, " contained no data to read");


		size_t row = 0;
		for (const auto& line : lines)
			TokenizeLine(line, row++);
	}

	void Lexer::TokenizeLine(const std::string& line, size_t row)
	{
		KBL_ASSERT(line.size() > 0, "lexer tokenizer encountered a line with no data");
		m_lines_of_raw_tokens.push_back(std::vector<RawToken>{});
		size_t len = line.size();
		size_t col = FindColumn(line, 0, [](char c) { return !std::isspace(c); });
		size_t end;
		while (col < len)
		{
			// #TODO wrapping char in string and passing c_str bad
			end = FindColumn(line, col + 1, [](char c) { return std::isspace(c) || IsTextOperator(std::string{ c }.c_str()); });

			KBL_ASSERT(end - col > 0);
			// #TODO replace substring usage
			std::string data = line.substr(col, end - col);
			
			// Check for comment
			if (data.size() >= 2)
			{
				if (data[0] == '/' && data[1] == '/')
					break;
			}

			m_lines_of_raw_tokens[m_lines_of_raw_tokens.size() - 1].emplace_back(RawToken{ data, col, row });

			col = end;
			col = FindColumn(line, col, [](char c) { return !std::isspace(c); });
		}

		if (m_lines_of_raw_tokens.back().size() == 0)
		{
			std::cout << "WARNING: tokenizer did not make any tokens for row=" << row << std::endl;
		}
	}

	void Lexer::LexTokens()
	{
		for (auto& line : m_lines_of_raw_tokens)
		{
			for (auto& token : line)
			{
				KBL_ASSERT((size_t)TokenType::TOTAL == 5, "Exhaustive handling of token types in ", __FUNCTION__);

				if (IsTextLiteral(token.Value.c_str()))
					token.Type = TokenType::LITERAL;
				else if (IsTextKeyword(token.Value.c_str()))
					token.Type = TokenType::KEYWORD;
				else if (IsTextOperator(token.Value.c_str()))
					token.Type = TokenType::OPERATOR;
				else
				{
					// #TODO better parsing for operands vs constants
					try
					{
						int64_t val = std::stoi(token.Value);
						token.Type = TokenType::CONSTANT;
					}
					catch (std::invalid_argument& e)
					{
						
						token.Type = TokenType::OPERAND;
					}
				}

				KBL_ASSERT(token.Type != TokenType::NONE, "probably a compiler bug if token type is still NONE");
			}
		}
	}

	void Lexer::Run()
	{
		std::string data;
		std::ifstream fs{ m_filepath, std::ios::in };
		// #TODO this reads entire file into memory, consider reading line by line
		if (fs)
		{
			fs.seekg(0, std::ios::end);
			data.resize(fs.tellg());
			fs.seekg(0, std::ios::beg);
			fs.read(&data[0], data.size());
			fs.close();
		}
		else
			KBL_ERROR("file \'", m_filepath.string(),"\' could not be opened");

		Tokenize(data);
		LexTokens();

		std::cout << "-------Lexer Tokens-------" << std::endl;
		for (const auto& line : m_lines_of_raw_tokens)
		{
			size_t i = 0;
			for (const auto& token : line)
			{
				std::cout << token;
				if (i++ < line.size() - 1)
					std::cout << ", ";
			}
			std::cout << std::endl;
		}
		std::cout << "--------------------------\n" << std::endl;
	}

}