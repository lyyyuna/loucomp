#include <iostream>
#include <string>

#include "interpreter.h"

using namespace loucomp;
using namespace loucomp::compiler;
using namespace loucomp::interpreter;

int main()
{
#ifdef LOUCOMP_RELEASE
	string code = R"loucomp(






x := 1;
y := 2;
if x = y then
	write 3
else
	write 19191
end







)loucomp";

	CodeError::List errors;
	auto codeFile = CodeFile::Scan(code, errors);

	auto syntaxFile = SyntaxFile::Parse(codeFile);

	auto symtab = make_shared<CodeSymtab>();
	Interpreter::Execute(syntaxFile, symtab);


#endif

	return 0;
}