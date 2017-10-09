#pragma once

#include "../ASTNode.hpp"

namespace Shakara
{
	namespace AST
	{
		class ForeachStatement : public Node
		{
		public:
			~ForeachStatement()
			{
				delete m_collection;

				delete m_itemName;

				delete m_indexName;

				delete m_body;
			}

			inline ForeachStatement& Collection(Node* node)
			{
				m_collection = node;

				return *this;
			}

			inline ForeachStatement& ItemName(Node* node)
			{
				m_itemName = node;

				return *this;
			}

			inline ForeachStatement& IndexName(Node* node)
			{
				m_indexName = node;

				return *this;
			}

			inline ForeachStatement& Body(Node* node)
			{
				node->Parent(this);

				m_body = node;

				return *this;
			}

			inline Node* Collection()
			{
				return m_collection;
			}

			inline Node* ItemName()
			{
				return m_itemName;
			}

			inline Node* IndexName()
			{
				return m_indexName;
			}

			inline Node* Body()
			{
				return m_body;
			}

		private:
			Node* m_collection = nullptr;
			Node* m_itemName   = nullptr;
			Node* m_indexName  = nullptr;

			Node* m_body = nullptr;

		};
	}
}