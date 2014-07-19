#ifndef LOUCOMP_PARSE_H
#define LOUCOMP_PARSE_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "scan.h"

using namespace std;

namespace loucomp
{
	namespace compiler
	{

		enum class ExpressionType
		{
			INT_EXPRESSION,
			IDENTIFIER_EXPRESSION,
			PLUS_EXPRESSION,
			MINUS_EXPRESSION,
			MUL_EXPRESSION,
			OVER_EXPRESSION,
			EQ_EXPRESSION,
			LT_EXPRESSION,

			NONONONONO,

			OP_EXPRESSION
		};

		enum class StatementType
		{
			EXPRESSION_STATEMENT,
			IF_STATEMENT,
			REPEAT_STATEMENT,
			ASSIGNMENT_STATEMENT,
			READ_STATEMENT,
			WRITE_STATEMENT
		};

		struct SyntaxTreeNode
		{
			typedef shared_ptr<SyntaxTreeNode>	Ptr;

			vector<Ptr>							child;
			Ptr									sibling;

			StatementType						stmttype;
			ExpressionType						exptype;

			int									val;
			CodeToken							token;
		};

		struct SyntaxFile
		{
			static SyntaxTreeNode::Ptr			Parse(CodeFile::Ptr codeFile);

			static CodeToken::List				tokens;
			static CodeToken::List::iterator	tokenIterator;

			static CodeToken::List::iterator	getToken();
			static SyntaxTreeNode::Ptr			stmt_sequence();
			static SyntaxTreeNode::Ptr			statement();
			static void							match(TokenType expected);
			static SyntaxTreeNode::Ptr			assign_stmt();
			static SyntaxTreeNode::Ptr			exp();
			static SyntaxTreeNode::Ptr			simple_exp();
			static SyntaxTreeNode::Ptr			term();
			static SyntaxTreeNode::Ptr			factor();

			static SyntaxTreeNode::Ptr			if_stmt();
			static SyntaxTreeNode::Ptr			repeat_stmt();
			static SyntaxTreeNode::Ptr			read_stmt();
			static SyntaxTreeNode::Ptr			write_stmt();
		};


	}
}

#endif