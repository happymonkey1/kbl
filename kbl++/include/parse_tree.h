#ifndef KBL_PARSE_TREE_H
#define KBL_PARSE_TREE_H

#include "tokens.h"

namespace kbl
{
	struct TreeNode
	{
		RawToken Data;
		TreeNode* Left = nullptr;
		TreeNode* Right = nullptr;
		TreeNode(RawToken ptr, TreeNode* left = nullptr, TreeNode* right = nullptr)
			: Data{ ptr }, Left{ left }, Right{ right } {}

		bool IsLeafNode() const
		{
			return Left == nullptr && Right == nullptr;
		}

		bool operator==(const TreeNode* other) const noexcept
		{
			return IsSameTokenType(other) && IsSameValue(other);
		}

		bool IsSameTokenType(const TreeNode* other) const noexcept
		{
			return Data.Type == other->Data.Type;
		}

		bool IsSameValue(const TreeNode* other) const noexcept
		{
			return Data.Value == other->Data.Value;
		}
	};

	class ParseTree
	{
	public:
		TreeNode* Root;
		ParseTree(TreeNode* ptr)
			: Root{ ptr } {}
		ParseTree(const ParseTree& other_tree) noexcept;
		ParseTree(ParseTree&& other_tree) noexcept;
		~ParseTree();

		ParseTree& operator=(const ParseTree& other_tree) noexcept;
		ParseTree& operator=(ParseTree&& other_tree) noexcept;

		static void DeleteTree(TreeNode* root);
		static size_t SizeOf(TreeNode* root);
		static void Print(TreeNode* root, uint32_t level = 0);
		static TreeNode* Copy(TreeNode* root);
		static bool IsTokenInTree(TreeNode* root, const RawToken& token_to_find);

		template <typename VisitFuncT>
		static void PreOrderTraversal(TreeNode* root, VisitFuncT visitor)
		{
			if (root == nullptr)
				return;

			visitor(root);
			InOrderTraversal(root->Left);
			InOrderTraversal(root->Right);
		}

		template <typename VisitFuncT>
		static void InOrderTraversal(TreeNode* root, VisitFuncT visitor)
		{
			if (root == nullptr)
				return;

			InOrderTraversal(root->Left);
			visitor(root);
			InOrderTraversal(root->Right);
		}
		
		size_t Size() const;
		void Print() const;
		bool IsTokenInTree(const RawToken& token_to_find) const;
	private:
		TreeNode* Copy() const;
	};

	struct ParseTreeView
	{
	public:
		TreeNode* Root;
		ParseTreeView(TreeNode* ptr)
			: Root{ ptr } {}
		~ParseTreeView() = default;

		size_t Size() const;
		void Print() const;
		bool IsTokenInTree(const RawToken& token_to_find) const;

		friend std::ostream& operator<<(std::ostream& out, const ParseTreeView& tree);
	};

	inline std::ostream& operator<<(std::ostream& out, const ParseTreeView& tree)
	{
		tree.Print();
		return out;
	}
}

#endif
