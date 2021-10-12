#include "parser.h"

namespace kbl
{
	Parser::Parser(const std::filesystem::path& filepath)
		: m_lexer{ filepath }, m_op_tokens{ std::vector<OpToken>{} }
	{
		m_lexer.Run();


		BuildTrees();

		std::cout << "-------Parser Trees-------" << std::endl;
		for (const auto& tree : m_trees)
		{
			std::cout << "-----Tree-----\n" << std::endl;
			tree.Print();
			std::cout << "\n--------------" << std::endl;
		}
		std::cout << "---------------------------" << std::endl;

		m_variable_map = {};
		m_offset_count = 0;
		for (const auto& tree : m_trees)
		{
			
			OpToken token;
			ParseTreeToOpToken(tree.Root, token);
			m_op_tokens.push_back(token);
		}

		std::cout << "\n----Op Tokens----\n" << std::endl;
		for (const auto& token : m_op_tokens)
			std::cout << token << std::endl;
		std::cout << "\n-----------------\n" << std::endl;

	}

	void Parser::BuildTrees()
	{
		KBL_ASSERT(m_lexer.m_lines_of_raw_tokens.size() > 0, "lexer did not pass parser any lines of tokens!");
		for (const auto& line : m_lexer.m_lines_of_raw_tokens)
		{
			auto tokens_copy = line;
			//std::reverse(tokens_copy.begin(), tokens_copy.end());

			TreeNode* root = nullptr;
			while (tokens_copy.size() > 0)
			{
				auto token = tokens_copy.back();
				tokens_copy.pop_back();

				root = ParseToken(root, token);
			}

			//KBL_ASSERT(root != nullptr, "no tree was built from line!");
			if (root != nullptr)
				m_trees.emplace_back(ParseTree{ root });
		}
	}

	TreeNode* Parser::ParseToken(TreeNode* root, const RawToken& token)
	{
		KBL_ASSERT((uint32_t)TokenType::TOTAL == 5, "Exhaustive handling of TokenType in ", __FUNCSIG__);
		switch (token.Type)
		{
		case TokenType::OPERAND:
		{
			if (root != nullptr)
			{
				// #TODO i don't like how we decide where to position operands. figure out better way
				if (root->Data.Type == TokenType::KEYWORD && root->Data.Value == std::string_view{ HumanReadableKeyword::LET })
				{
					TreeNode* new_root = new TreeNode{ token };
					root = new_root;
				}
				else if (root->Data.Type == TokenType::OPERATOR && root->Data.Value == std::string_view{ HumanReadableOperator::ASSIGMENT })
				{
					TreeNode* node = new TreeNode{ token };
					if (root->Left == nullptr)
						root->Left = node;
					else
						root->Left->Left = node;
				}
				else if (root->Data.Type == TokenType::OPERATOR && root->Data.Value == std::string_view{ HumanReadableOperator::PLUS })
				{
					TreeNode* node = new TreeNode{ token };
					if (root->Left == nullptr)
						root->Left = node;
					else
						root->Left->Left = node;
				}
				else
				{
					KBL_ERROR("malformed expression, found token=", token.string());
				}
			}
			else
			{
				TreeNode* new_root = new TreeNode{ token };
				root = new_root;
			}

			return root;
		}
		case TokenType::CONSTANT:
		{
			if (root == nullptr)
			{
				TreeNode* new_root = new TreeNode{ token };
				root = new_root;
			}
			else
			{
				TreeNode* node = new TreeNode{ token };
				root->Left = node;
			}

			return root;
		}
		case TokenType::LITERAL:
		{
			if (root == nullptr)
				KBL_ERROR("Literal encountered empty tree.");

			//TreeNode* new_root = new TreeNode{ token, nullptr, root };
			//root = new_root;

			// #TODO i don't like how we decide where to position operands. figure out better way
			if (root->Data.Type == TokenType::KEYWORD && root->Data.Value == std::string_view{ HumanReadableKeyword::LET })
			{
				TreeNode* new_root = new TreeNode{ token };
				root = new_root;
			}
			else if (root->Data.Type == TokenType::OPERATOR && root->Data.Value == std::string_view{ HumanReadableOperator::ASSIGMENT })
			{
				TreeNode* node = new TreeNode{ token };
				if (root->Left == nullptr)
					root->Left = node;
				else
					root->Left->Left = node;
			}
			else if (root->Data.Type == TokenType::OPERATOR && root->Data.Value == std::string_view{ HumanReadableOperator::PLUS })
			{
				TreeNode* node = new TreeNode{ token };
				if (root->Left == nullptr)
					root->Left = node;
				else
					root->Left->Left = node;
			}
			else
			{
				std::cout << "\n------Debug Print for tree being build------\n";

				ParseTreeView tree{ root };
				tree.Print();

				std::cout << "\n--------------------------------------------" << std::endl;
				KBL_ERROR("compiler bug in literal switch, token=", token.string());
			}

			return root;
		}
		case TokenType::KEYWORD:
		{
			KBL_ASSERT((size_t)KeywordType::TOTAL == 2, "Exhaustive handling of KeywordType in ", __FUNCTION__, " in keyword parsing");
			auto operator_type = HumanReadableKeyword::StringToOperatorType(token.Value.c_str());
			switch (operator_type)
			{
			case KeywordType::LET:
			{
				/*if (root != nullptr)
					KBL_ERROR("tree still on stack");*/
				ParseTreeView tree_view = { root };
				if (tree_view.IsTokenInTree(token))
				{
					KBL_ERROR("malformed line, duplicate \'", token.Value, "\' found!");
				}

				TreeNode* new_root = new TreeNode{ token, root };
				


				root = new_root;

				break;
			}
			case KeywordType::PRINT:
			{
				ParseTreeView tree_view = { root };
				if (tree_view.IsTokenInTree(token))
				{
					KBL_ERROR("malformed line, duplicate \'", token.Value, "\' found!");
				}

				TreeNode* new_root = new TreeNode{ token, root };
				root = new_root;
				break;
			}
			default:
				KBL_ERROR("Cannot parse text \'", token.Value, "\'");
			}

			return root;
		}
		case TokenType::OPERATOR:
		{
			KBL_ASSERT((size_t)OperatorType::TOTAL == 3, "Exhaustive handling of OperatorType in ", __FUNCTION__, " in operator parsing");
			auto operator_type = HumanReadableOperator::StringToTokenType(token.Value.c_str());

			switch (operator_type)
			{
			case OperatorType::ASSIGMENT:
				[[fallthrough]];
			case OperatorType::PLUS:
			{
				if (root == nullptr)
					KBL_ERROR("no tree on stack");

				TreeNode* new_root = new TreeNode{ token, nullptr, root };
				root = new_root;
				break;
			}
			case OperatorType::END_OF_STATEMENT:
			{
				
				/*if (root != nullptr)
				{
					m_trees.push_back(root);
					root = nullptr;
				}
				else
				{
					std::cout << "WARNING: encountered eol operator with nothing on the stack!" << std::endl;
				}*/
				
				return root;
				break;
			}
			case OperatorType::NONE:
				[[fallthrough]];
			default:
				KBL_ERROR("Cannot parse text \'", token.Value, "\'");
			}

			return root;
		}
		case TokenType::NONE:
		{
			KBL_ASSERT(false, "probably a compiler bug if the token type is NONE");
			return nullptr;
		}
		default:
			KBL_ASSERT(false, "Unhandled token type!");
			return nullptr;
		}

		KBL_ASSERT(false, "unreachable");
		return nullptr;
	}

	void Parser::ParseTreeToOpToken(TreeNode* root, OpToken& op_token)
	{
		if (root == nullptr)
			return;

		auto root_token_type = root->Data.Type;

		KBL_ASSERT((size_t)TokenType::TOTAL == 5, "Exhaustive handling of TokenType in EvaluateTree");
		switch (root_token_type)
		{
		case TokenType::OPERAND:
		{

			const auto& operand_name = root->Data.Value;
		
			if (m_variable_map.find(operand_name) == m_variable_map.end())
				m_variable_map[operand_name] = m_offset_count++;
			

			op_token.Args.push_back(m_variable_map[operand_name]);
			
			KBL_ASSERT(root->IsLeafNode(), "Malformed tree detected in ", __FUNCTION__);

			return;
		}
		case TokenType::CONSTANT:
		{
			const auto& constant_val = root->Data.Value;
			try
			{
				int64_t val = std::stoi(constant_val.c_str());
				//KBL_ASSERT(op_token.Args.size() == 1, "pushing immediate value to op token without offset!");
				op_token.Args.push_back(val);
			}
			catch (std::invalid_argument& e)
			{
				KBL_ERROR("unable to parse constant \'", constant_val, "\' in", __FUNCTION__);
			}
			catch (...)
			{
				KBL_ERROR("unhandled exception having to do with constant \'", constant_val, "\' in ", __FUNCTION__);
			}
		
			KBL_ASSERT(root->IsLeafNode(), "Malformed tree detected in ", __FUNCTION__);

			return;
		}
		case TokenType::LITERAL:
		{
			KBL_WARN("literal parsing not implemented in ", __FUNCTION__);

			ParseTreeToOpToken(root->Left, op_token);
			ParseTreeToOpToken(root->Right, op_token);

			return;
		}
		case TokenType::KEYWORD:
		{
			KBL_ASSERT((size_t)KeywordType::TOTAL == 2, "Exhaustive handling of KeywordType in ", __FUNCTION__);
			auto keyword_type = HumanReadableKeyword::StringToOperatorType(root->Data.Value.c_str());
			switch (keyword_type)
			{
				case KeywordType::LET:
				{
					// #TODO make this better
					if (root->Left->Right->Data.Type == TokenType::CONSTANT)
						op_token.Type = OpCode::STORE_IM;
					else
						op_token.Type = OpCode::STORE_FROM_STACK;

					break;
				}
				case KeywordType::PRINT:
				{
					op_token.Type = OpCode::PRINT_FROM_OFFSET;
					break;
				}
			}

			KBL_ASSERT(root->Left != nullptr, "Malformed tree detected in Keyword case in ", __FUNCTION__);
			KBL_ASSERT(root->Right == nullptr, "Malformed tree detected in Keyword case in ", __FUNCTION__);

			ParseTreeToOpToken(root->Left, op_token);

			return;
		}
		case TokenType::OPERATOR:
		{
			KBL_ASSERT((size_t)OperatorType::TOTAL == 3, "Exhaustive handling of OperatorType in ", __FUNCTION__);
			auto operator_type = HumanReadableOperator::StringToTokenType(root->Data.Value.c_str());
			switch (operator_type)
			{
			case OperatorType::ASSIGMENT:
			{
				ParseTreeToOpToken(root->Left, op_token);
				ParseTreeToOpToken(root->Right, op_token);

				break;
			}
			case OperatorType::PLUS:
			{
				auto left_node_type = root->Left->Data.Type;
				auto right_node_type = root->Right->Data.Type;

				OpToken add_op;
				if (left_node_type == TokenType::OPERATOR)
				{
					ParseTreeToOpToken(root->Left, add_op);
				}


				if (left_node_type == TokenType::CONSTANT && right_node_type == TokenType::CONSTANT)
					add_op.Type = OpCode::STACK_ADD_IM_TO_IM;
				else if (left_node_type == TokenType::OPERAND && right_node_type == TokenType::OPERAND)
					add_op.Type = OpCode::STACK_ADD_OFF_TO_OFF;
				else if (left_node_type == TokenType::CONSTANT && right_node_type == TokenType::OPERAND
					|| left_node_type == TokenType::OPERAND && right_node_type == TokenType::CONSTANT)
				{
					add_op.Type = OpCode::STACK_ADD_IM_TO_OFF;
				}
				else
				{
					KBL_ERROR("compiler does not support adding [", HumanReadableTokenType::TokenTypeToString(left_node_type), ", ",
						HumanReadableTokenType::TokenTypeToString(right_node_type), "]");
				}
				
				ParseTreeToOpToken(root->Left, add_op);
				ParseTreeToOpToken(root->Right, add_op);

				// OpCode STACK_ADD_IM_TO_OFF specifies (addr, im) so we need to swap args
				if (left_node_type == TokenType::CONSTANT && right_node_type == TokenType::OPERAND)
				{
					auto tmp = add_op.Args[1];
					add_op.Args[1] = add_op.Args[0];
					add_op.Args[0] = tmp;
				}

				m_op_tokens.push_back(add_op);

				break;
			}
			case OperatorType::END_OF_STATEMENT:
			{
				// Does nothing
				break;
			}
			default:
				KBL_ERROR("unhandled type in", __FUNCTION__);
			}
			
			return;
		}
		default:
			KBL_ERROR("unreachable");
		}
		
		KBL_ASSERT(false, "unreachable");
	}

	OpCode Parser::ParseTreeExpressionOpCodeType(TreeNode* root)
	{
		return OpCode::NONE;
	}

}