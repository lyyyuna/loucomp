#include "parse.h"

namespace loucomp
{
	namespace compiler
	{
		CodeToken::List				SyntaxFile::tokens;
		CodeToken::List::iterator	SyntaxFile::tokenIterator;

		// abstract syntax tree
		SyntaxTreeNode::Ptr SyntaxFile::Parse(CodeFile::Ptr codeFile)
		{
			tokens = codeFile->tokens;

			tokenIterator = tokens.begin();
			auto t = stmt_sequence();
			if (tokenIterator->type != TokenType::ENDFILE)
			{

			}

			return t;
		}

		CodeToken::List::iterator SyntaxFile::getToken()
		{
			return (tokenIterator++);
		}

		void SyntaxFile::match(TokenType expected)
		{
			if (tokenIterator->type == expected)
			{
				getToken();
			}
			else
			{

			}
		}

		SyntaxTreeNode::Ptr	SyntaxFile::stmt_sequence()
		{
			auto t = statement();
			auto p = t;

			while (tokenIterator->type != TokenType::ENDFILE &&
				   tokenIterator->type != TokenType::END &&
				   tokenIterator->type != TokenType::ELSE &&
				   tokenIterator->type != TokenType::UNTIL)
			{
				auto q = make_shared<SyntaxTreeNode>();
				match(TokenType::SEMI);
				q = statement();
				if (q != nullptr)
				{
					if (nullptr == t)
						t = p = q;
					else
					{
						p->sibling = q;
						p = q;
					}
				}

			}

			return t;
		}

		SyntaxTreeNode::Ptr	SyntaxFile::statement(void)
		{
			auto t = make_shared<SyntaxTreeNode>();

			auto tokentype = tokenIterator->type;
			switch (tokentype)
			{
			case TokenType::ID:
				t = assign_stmt();
				break;

			case TokenType::IF:
				t = if_stmt();
				break;

			case TokenType::REPEAT:
				t = repeat_stmt();
				break;

			case TokenType::READ:
				t = read_stmt();
				break;

			case TokenType::WRITE:
				t = write_stmt();
				break;

			default:
				break;

			}

			return t;
		}

		SyntaxTreeNode::Ptr	SyntaxFile::if_stmt(void)
		{
			SyntaxTreeNode ifstmt;
			ifstmt.stmttype = StatementType::IF_STATEMENT;
			auto t = make_shared<SyntaxTreeNode>(ifstmt);

			match(TokenType::IF);

			t->child.push_back(exp());

			match(TokenType::THEN);

			t->child.push_back(stmt_sequence());

			if (tokenIterator->type == TokenType::ELSE)
			{
				match(TokenType::ELSE);

				t->child.push_back(stmt_sequence());
			}

			return t;
		}

		SyntaxTreeNode::Ptr	SyntaxFile::repeat_stmt()
		{
			SyntaxTreeNode repeatstmt;
			repeatstmt.stmttype = StatementType::REPEAT_STATEMENT;
			auto t = make_shared<SyntaxTreeNode>(repeatstmt);

			match(TokenType::REPEAT);

			t->child.push_back(stmt_sequence());

			match(TokenType::UNTIL);

			t->child.push_back(exp());

			return t;
		}

		SyntaxTreeNode::Ptr SyntaxFile::assign_stmt(void)
		{
			SyntaxTreeNode newassign;
			newassign.stmttype = StatementType::ASSIGNMENT_STATEMENT;
			auto t = make_shared<SyntaxTreeNode>(newassign);

			if ((t != nullptr) && (tokenIterator->type == TokenType::ID))
				t->token = *tokenIterator;

			match(TokenType::ID);
			match(TokenType::ASSIGN);

			if (t != nullptr)
				t->child.push_back(exp());

			return t;
		}

		SyntaxTreeNode::Ptr SyntaxFile::read_stmt()
		{
			SyntaxTreeNode readstmt;
			readstmt.stmttype = StatementType::READ_STATEMENT;
			auto t = make_shared<SyntaxTreeNode>(readstmt);

			// if ()

			match(TokenType::READ);
			t->token = *tokenIterator;
			match(TokenType::ID);

			return t;
		}

		SyntaxTreeNode::Ptr SyntaxFile::write_stmt()
		{
			SyntaxTreeNode writestmt;
			writestmt.stmttype = StatementType::WRITE_STATEMENT;
			auto t = make_shared<SyntaxTreeNode>(writestmt);

			match(TokenType::WRITE);
			t->child.push_back(exp());

			return t;
		}

		SyntaxTreeNode::Ptr	SyntaxFile::exp()
		{
			auto t = simple_exp();

			if ((tokenIterator->type == TokenType::LT) || (tokenIterator->type == TokenType::EQ))
			{
				SyntaxTreeNode newexp;
				newexp.stmttype = StatementType::EXPRESSION_STATEMENT;
				newexp.exptype = ExpressionType::OP_EXPRESSION;
				auto p = make_shared<SyntaxTreeNode>(newexp);
				if (p != nullptr)
				{
					p->child.push_back(t);
					p->token = *tokenIterator;
					t = p;
				}
				match(tokenIterator->type);
				if (t != nullptr)
					t->child.push_back(simple_exp());
			}

			return t;
		}

		SyntaxTreeNode::Ptr	SyntaxFile::simple_exp()
		{
			auto t = term();

			while ((tokenIterator->type == TokenType::PLUS) || (tokenIterator->type == TokenType::MINUS))
			{
				SyntaxTreeNode newexp;
				newexp.stmttype = StatementType::EXPRESSION_STATEMENT;
				newexp.exptype = ExpressionType::OP_EXPRESSION;
				auto p = make_shared<SyntaxTreeNode>(newexp);
				if (p != nullptr)
				{
					p->child.push_back(t);
					p->token = *tokenIterator;
					t = p;
					// match();
					match(tokenIterator->type);
					t->child.push_back(term());
					
				}
			}

			return t;
		}

		SyntaxTreeNode::Ptr SyntaxFile::term()
		{
			auto t = factor();

			while ((tokenIterator->type == TokenType::TIMES) || (tokenIterator->type == TokenType::OVER))
			{
				SyntaxTreeNode newexp;
				newexp.stmttype = StatementType::EXPRESSION_STATEMENT;
				newexp.exptype = ExpressionType::OP_EXPRESSION;
				auto p = make_shared<SyntaxTreeNode>(newexp);
				if (p != nullptr)
				{
					p->child.push_back(t);
					p->token = *tokenIterator;
					t = p;
					match(tokenIterator->type);
					p->child.push_back(factor());
				}
			}

			return t;
		}

		SyntaxTreeNode::Ptr SyntaxFile::factor()
		{
			auto t = make_shared<SyntaxTreeNode>();
			switch (tokenIterator->type)
			{
			case TokenType::NUM:
				t->token = *tokenIterator;
				t->stmttype = StatementType::EXPRESSION_STATEMENT;
				t->exptype = ExpressionType::INT_EXPRESSION;
				t->val = atoi(tokenIterator->value.c_str());

				match(TokenType::NUM);
				break;

			case TokenType::ID:
				t->token = *tokenIterator;
				t->stmttype = StatementType::EXPRESSION_STATEMENT;
				t->exptype = ExpressionType::IDENTIFIER_EXPRESSION;
				// t->val = tokenIterator->value;

				match(TokenType::ID);
				break;

			case TokenType::LPAREN:
				match(TokenType::LPAREN);
				t = exp();
				match(TokenType::RPAREN);
				break;

			default:
				;
			}

			return t;
		}

		
	}
}