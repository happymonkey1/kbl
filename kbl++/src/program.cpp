#include "program.h"

#include <algorithm>
#include <unordered_map>
#include <filesystem>

#include <string>

namespace kbl
{
	Program::Program(const std::filesystem::path& filepath)
		: m_filepath{ filepath }, m_parser { m_filepath }, m_op_stack{ std::vector<OpToken>{} }, m_stack{ nullptr }, m_stack_base{ nullptr }
	{
		
		



	}

	Program::~Program()
	{
		if (m_stack_base != nullptr)
			delete[] (*m_stack_base);
	}

	void Program::Interpret()
	{
		KBL_ASSERT(sizeof(int64_t) == sizeof(uintptr_t), "compiler assumes int64_t == uintptr_t");

		m_stack_base = new int64_t* [INT_STACK_CAP] {0};
		m_stack = m_stack_base + m_parser.m_offset_count;

		auto op_codes_copy = m_parser.m_op_tokens;
		std::reverse(op_codes_copy.begin(), op_codes_copy.end());

		while (op_codes_copy.size() > 0)
		{
			auto token = op_codes_copy.back();
			op_codes_copy.pop_back();

			//std::cout << "[Interpreter] Handling token: " << token << std::endl;

			KBL_ASSERT((size_t)OpCode::TOTAL == 8, "Exhaustive handling of OpCode in ", __FUNCTION__);
			switch (token.Type)
			{
				case OpCode::STORE_OFFSET:
				{
					KBL_ASSERT(token.Args.size() == 2, "Malformed store token, args_count=", token.Args.size(), " instead of >= 1!");

					
					int64_t* val = (int64_t*)token.Args[1];
					

					*(m_stack_base + token.Args[0]) = val;

					break;
				}
				case OpCode::STORE_IM:
				{
					KBL_ASSERT(token.Args.size() == 2, "Malformed store token, args_count=", token.Args.size(), " instead of 2!");
					
					int64_t* val = new int64_t{ token.Args[1] };
					*(m_stack_base + token.Args[0]) = val;

					break;
				}
				case OpCode::STORE_FROM_STACK:
				{
					KBL_ASSERT(token.Args.size() == 1, "Malformed store token, args_count=", token.Args.size(), " instead of 1!");

					int64_t* val = *m_stack;
					m_stack--;
					*(m_stack_base + token.Args[0]) = val;

					break;
				}
				case OpCode::LOAD_OFFSET_TO_STACK:
				{
					KBL_ASSERT(token.Args.size() == 1, "Malformed load token, args_count=", token.Args.size(), " instead of 1!");

					// Load value to top of stack
					*(m_stack++) = *(m_stack_base + token.Args[0]);

					break;
				}
				case OpCode::STACK_ADD_OFF_TO_OFF:
				{
					KBL_ASSERT(token.Args.size() == 2, "Malformed load token, args_count=", token.Args.size(), " instead of 1!");

					// Load value from offset
					int64_t* a = *(m_stack_base + token.Args[0]);
					int64_t* b = *(m_stack_base + token.Args[1]);

					int64_t* sum_ptr = new int64_t{ *a + *b };

					// Store on top of stack
					*(++m_stack) = sum_ptr;

					break;
				}
				case OpCode::STACK_ADD_IM_TO_IM:
				{
					KBL_ASSERT(token.Args.size() == 2, "Malformed load token, args_count=", token.Args.size(), " instead of 1!");

					// Load value from offset
					int64_t a = token.Args[0];
					int64_t b = token.Args[1];

					int64_t* sum_ptr = new int64_t{ a + b };

					// Store on top of stack
					*(++m_stack) = sum_ptr;

					break;
				}
				case OpCode::STACK_ADD_IM_TO_OFF:
				{
					KBL_ASSERT(token.Args.size() == 2, "Malformed load token, args_count=", token.Args.size(), " instead of 1!");

					// Load value from offset
					int64_t* a = *(m_stack_base + token.Args[0]);
					int64_t b = token.Args[1];

					int64_t* sum_ptr = new int64_t{ *a + b };

					// Store on top of stack
					*(++m_stack) = sum_ptr;

					break;
				}
				case OpCode::PRINT_FROM_OFFSET:
				{
					KBL_ASSERT(token.Args.size() <= 1, "Malformed print token, args_count=", token.Args.size(), " instead of 1!");
					
					std::cout << "[Interpreter]: ";
					if (token.Args.size() == 1)
						std::cout << **(m_stack_base + token.Args[0]) << std::endl;
					else
						std::cout << **(m_stack) << std::endl;
					break;
				}
				default:
				{
					KBL_ASSERT(false, "unreachable code in ", __FUNCTION__);
				}
			}
		}

		exit(0);
	}

	void Program::Compile()
	{
		std::cout << "--------Writing Assmebly--------" << std::endl;

		std::filesystem::path filepath = m_filepath.parent_path();
		std::filesystem::path filename = m_filepath.stem().string();
		std::filesystem::path assembly_filename = filename.string() + ".asm";
		std::filesystem::path out_path = filepath / assembly_filename;
		std::ofstream out{ out_path, std::ios::out };
		
		WriteAssemblyToFile(out);

		out.close();

		std::cout << "--------Compiling--------" << std::endl;
		std::filesystem::path assembly_path = filepath / assembly_filename.string();
		std::filesystem::path out_object_path = filepath / std::filesystem::path{ filename.string() + ".o" };
		


		std::string nasm_cmd = std::string{ "nasm -f win64 " } + assembly_path.string() + std::string{ " -o " } + out_object_path.string();
		system(nasm_cmd.c_str());

		std::filesystem::path exec_path = filepath / std::filesystem::path{ filename.string() + ".exe" };
		std::string linker_cmd = std::string{ "link " } + out_object_path.string() + std::string{ " /subsystem:console /libpath:\"libs/\" /machine:x64 /out:" } + exec_path.string() + std::string{ " kernel32.Lib" };
		std::cout << linker_cmd << std::endl;
		system(linker_cmd.c_str());

		std::cout << "--------Running--------" << std::endl;
		std::string run_cmd = std::string{ exec_path.string() };
		system(run_cmd.c_str());
		system("echo %errorlevel%");
		system("pause");
	}

	void Program::WriteAssemblyToFile(std::ofstream& out)
	{
		out <<
			"bits 64\n"
			"default rel\n"
			"section .data\n\n"
			"msg db \"Hello World!\", 0xd, 0x0a, 0\n"
			"msg_len equ $ - msg\n"
			"stdout_query equ - 11\n\n"
			"stdout dw 0\n"
			"bytes_written dw 0\n\n"
			"section .text\n\n"
			"global mainCRTStartup\n"
			"extern ExitProcess\n"
			"extern WriteFile\n"
			"extern GetStdHandle\n\n"
			"mainCRTStartup:\n"
			" mov rcx, stdout_query\n"
			" call GetStdHandle\n"
			" mov[rel stdout], rax\n"
			" mov  rcx, [rel stdout]\n"
			" mov  rdx, msg\n"
			" mov  r8, msg_len\n"
			" mov  r9, bytes_written\n"
			" push qword 0\n"
			" call WriteFile\n"
			" xor rcx, rcx\n"
			" call ExitProcess\n";
	}

}