#include "interpreter.h"
#include "symtab.h"

namespace loucomp
{
	namespace interpreter
	{
		using namespace loucomp::compiler;

		void Interpreter::Execute(SyntaxTreeNode::Ptr syntaxFile, CodeSymtab::Ptr symtab)
		{

			for (auto stmt = syntaxFile; stmt != nullptr; stmt = stmt->sibling)
			{
				switch (stmt->stmttype)
				{
				case StatementType::ASSIGNMENT_STATEMENT:
					ExecuteAssignStmt(stmt, symtab);
					break;

				case StatementType::WRITE_STATEMENT:
					ExecuteWriteStmt(stmt, symtab);
					break;

				case StatementType::IF_STATEMENT:
					ExecuteIfStmt(stmt, symtab);
					break;

				}
			}
		}

		void Interpreter::ExecuteAssignStmt(SyntaxTreeNode::Ptr assginstmt, CodeSymtab::Ptr symtab)
		{
			int result = ExecuteExpStmt(assginstmt->child[0], symtab);

			auto token = assginstmt->token;
			auto symbol = token.value;
			if (symtab->SearchForSymbol(symbol))
			{
				symtab->ChangeSymbol(symbol, result);
			}
			else
			{
				symtab->AddToTable(symbol, result);
			}
		}

		int Interpreter::ExecuteExpStmt(SyntaxTreeNode::Ptr expstmt, CodeSymtab::Ptr symtab)
		{
			int result = 0;

			auto nodetype = expstmt->token.type;
			auto symbol = expstmt->token.value;

			switch (nodetype)
			{
			case TokenType::NUM:
				result = expstmt->val;
				break;

			case TokenType::ID:
				
				if (symtab->SearchForSymbol(symbol))
				{
					return symtab->ReturnSymbol(symbol);
				}
				else
				{
					// error
				}
				break;

			case TokenType::LT:
				result = ExecuteExpStmt(expstmt->child[0], symtab) < ExecuteExpStmt(expstmt->child[1], symtab);
				break;

			case TokenType::EQ:
				result = ExecuteExpStmt(expstmt->child[0], symtab) == ExecuteExpStmt(expstmt->child[1], symtab);
				break;

			case TokenType::PLUS:
				result = ExecuteExpStmt(expstmt->child[0], symtab) + ExecuteExpStmt(expstmt->child[1], symtab);
				break;

			case TokenType::MINUS:
				result = ExecuteExpStmt(expstmt->child[0], symtab) - ExecuteExpStmt(expstmt->child[1], symtab);
				break;

			case TokenType::TIMES:
				result = ExecuteExpStmt(expstmt->child[0], symtab) * ExecuteExpStmt(expstmt->child[1], symtab);
				break;

			case TokenType::OVER:
				result = ExecuteExpStmt(expstmt->child[0], symtab) / ExecuteExpStmt(expstmt->child[1], symtab);
				break;

			default:
				// error
				break;
			}

			return result;
		}

		void Interpreter::ExecuteWriteStmt(SyntaxTreeNode::Ptr writestmt, CodeSymtab::Ptr symtab)
		{
			cout << ExecuteExpStmt(writestmt->child[0], symtab) << endl;
		}

		void Interpreter::ExecuteIfStmt(SyntaxTreeNode::Ptr ifstmt, CodeSymtab::Ptr symtab)
		{
			int condition = ExecuteExpStmt( ifstmt->child[0], symtab );

			if (0 != condition)
			{
				Execute(ifstmt->child[1], symtab);
			}
			else
			{
				auto len = ifstmt->child.size();
				if (3 == len)
				{
					Execute(ifstmt->child[2], symtab);
				}
			}
		}
	}
}