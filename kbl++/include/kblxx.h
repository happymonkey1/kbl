#ifndef KBL_KBLXX_H
#define KBL_KBLXX_H

namespace kbl
{
	constexpr const char* COMPILER_NAME = "kbl++";
	constexpr const char* COMPILER_VERSION = "v0.0.1";

	namespace cmd
	{
		constexpr const char* COMPILE = "com";
		constexpr const char* INTERPRET = "int";
	}

	void PrintWelcome();
	void ParseCommandArguments(int argc, char* argv[]);
}

#endif
