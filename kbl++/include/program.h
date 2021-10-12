#ifndef KBL_PROGRAM_H
#define KBL_PROGRAM_H

#include "core.h"
#include "operations.h"
#include "parser.h"

#include <fstream>

namespace kbl
{
	// Size in words
	constexpr const size_t INT_STACK_CAP = 1024;
	// Size in bytes
	constexpr const size_t INT_STACK_SIZE = INT_STACK_CAP * sizeof(uintptr_t);

	class Program
	{
	public:
		Program(const std::filesystem::path& filepath);
		~Program();

		void Interpret();
		void Compile();
	private:
		void WriteAssemblyToFile(std::ofstream& out);
		std::filesystem::path m_filepath;
		Parser m_parser;
		std::vector<OpToken> m_op_stack;

		int64_t** m_stack_base;
		int64_t** m_stack;
	};
}

#endif