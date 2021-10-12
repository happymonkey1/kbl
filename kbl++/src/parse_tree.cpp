#include "parse_tree.h"

namespace kbl
{
	ParseTree::ParseTree(const ParseTree& other_tree) noexcept
		: Root{ nullptr }
	{
		if (other_tree.Root == nullptr)
		{
			Root = nullptr;
			return;
		}

		Root = other_tree.Copy();
	}

	ParseTree::ParseTree(ParseTree&& other_tree) noexcept
		: Root{ other_tree.Root }
	{
		other_tree.Root = nullptr;
	}

	ParseTree::~ParseTree()
	{
		DeleteTree(Root);
	}

	ParseTree& ParseTree::operator=(const ParseTree& other_tree) noexcept
	{
		Root = other_tree.Copy();
		return *this;
	}

	ParseTree& ParseTree::operator=(ParseTree&& other_tree) noexcept
	{
		Root = other_tree.Root;
		other_tree.Root = nullptr;
		return *this;
	}

	void ParseTree::DeleteTree(TreeNode* root)
	{
		if (root == nullptr)
			return;

		DeleteTree(root->Left);
		DeleteTree(root->Right);
		delete root;
	}

	size_t ParseTree::SizeOf(TreeNode* root)
	{
		if (root == nullptr)
			return 0;

		return 1 + ParseTree::SizeOf(root->Left) + ParseTree::SizeOf(root->Right);
	}

	void ParseTree::Print(TreeNode* root, uint32_t level)
	{
		for (size_t i = 0; i < (size_t)level * 4; ++i)
			std::cout << ".";

		if (root == nullptr)
		{
			std::cout << "None" << std::endl;
			return;
		}

		std::cout << root->Data << std::endl;
		Print(root->Left, level + 1);
		Print(root->Right, level + 1);
	}

	// Pre-order traversal copy
	TreeNode* ParseTree::Copy(TreeNode* root)
	{
		if (root == nullptr)
			return nullptr;

		return new TreeNode{ root->Data, Copy(root->Left), Copy(root->Right) };
	}

	bool ParseTree::IsTokenInTree(TreeNode* root, const RawToken& token_to_find)
	{
		if (root == nullptr)
			return false;

		bool found = root->Data == token_to_find;
		if (found)
			return true;
		else
			return IsTokenInTree(root->Left, token_to_find) || IsTokenInTree(root->Right, token_to_find);
	}

	bool ParseTree::IsTokenInTree(const RawToken& token_to_find) const
	{
		return ParseTree::IsTokenInTree(Root, token_to_find);
	}

	TreeNode* ParseTree::Copy() const
	{
		return ParseTree::Copy(Root);
	}

	

	size_t ParseTreeView::Size() const
	{
		return ParseTree::SizeOf(Root);
	}

	void ParseTreeView::Print() const
	{
		ParseTree::Print(Root, 0);
	}

	bool ParseTreeView::IsTokenInTree(const RawToken& token_to_find) const
	{
		return ParseTree::IsTokenInTree(Root, token_to_find);
	}

	size_t ParseTree::Size() const
	{
		return ParseTree::SizeOf(Root);
	}

	void ParseTree::Print() const
	{
		ParseTree::Print(Root, 0);
	}
}