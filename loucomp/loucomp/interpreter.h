#ifndef LOUCOMP_INTERPRETER_H
#define LOUCOMP_INTERPRETER_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include "scan.h"
#include "parse.h"
#include "symtab.h"

using namespace std;
using namespace loucomp::compiler;

namespace loucomp
{
	namespace interpreter
	{
		struct CodeSymtab
		{
			typedef shared_ptr<CodeSymtab>		Ptr;

			map<string, int>				table;

			bool							SearchForSymbol(string symbol);
			void							AddToTable(string symbol, int value);
			void							ChangeSymbol(string symbol, int value);
			int								ReturnSymbol(string symbol);
		};

		struct Interpreter
		{
			// static void		Execute(SyntaxTreeNode::Ptr syntaxFile, Symtab symtab);
			static void						Execute(SyntaxTreeNode::Ptr syntaxFile, CodeSymtab::Ptr symtab);
			
			static void						ExecuteAssignStmt(SyntaxTreeNode::Ptr assginstmt, CodeSymtab::Ptr symtab);
			static int						ExecuteExpStmt(SyntaxTreeNode::Ptr expstmt, CodeSymtab::Ptr symtab);
			static void						ExecuteWriteStmt(SyntaxTreeNode::Ptr writestmt, CodeSymtab::Ptr symtab);

			static void						ExecuteIfStmt(SyntaxTreeNode::Ptr ifstmt, CodeSymtab::Ptr symtab);
		};
	}
}

#endif