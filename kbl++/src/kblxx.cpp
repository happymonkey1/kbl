#include "kblxx.h"

#include "core.h"
#include "program.h"
#include <iostream>

namespace kbl
{
	void PrintWelcome()
	{
		std::cout << "Compiling with " << COMPILER_NAME << ' ' << COMPILER_VERSION << std::endl;
	}

	void ParseCommandArguments(int argc, char* argv[])
	{
		if (argc <= 1)
			KBL_ERROR("command not specified!");
		
		if (argc < 2)
			KBL_ERROR("filepath not specified!");

		std::string command = argv[1];
		std::string filepath = argv[2];
		
		Program program = Program{ filepath };
		if (command == cmd::COMPILE)
		{
			program.Compile();
		}
		else if (command == cmd::INTERPRET)
		{
			program.Interpret();
		}
		else
		{
			KBL_ERROR("unknown command \'", command, "\' supplied!");
		}
	}

}