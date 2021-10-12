#include "tokens.h"
#include "parse_tree.h"

namespace kbl
{
	OpToken::OpToken()
		: Type{ OpCode::NONE }, Args{ }
	{

	}

	OpToken::OpToken(OpCode code)
		: Type{ code }, Args{ }
	{

	}
}