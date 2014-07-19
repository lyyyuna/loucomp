#include "unitTest.h"
#include "interpreter.h"

using namespace loucomp;
using namespace loucomp::compiler;
using namespace loucomp::interpreter;

TEST_CASE(TestInterpreter_assign)
{
	string code = R"loucomp(
x := 1;
y := 2 + 1
)loucomp";

	CodeError::List errors;
	auto codeFile = CodeFile::Scan(code, errors);

	auto syntaxFile = SyntaxFile::Parse(codeFile);

	auto symtab = make_shared<CodeSymtab>();
	Interpreter::Execute(syntaxFile, symtab);
}

TEST_CASE(TestInterpreter_write)
{
	string code = R"loucomp(
write 10
)loucomp";

	CodeError::List errors;
	auto codeFile = CodeFile::Scan(code, errors);

	auto syntaxFile = SyntaxFile::Parse(codeFile);

	auto symtab = make_shared<CodeSymtab>();
	Interpreter::Execute(syntaxFile, symtab);
}

TEST_CASE(TestInterpreter_write_assign)
{
	string code = R"loucomp(
x := 2 * 4
y := x + 1
write y
)loucomp";

	CodeError::List errors;
	auto codeFile = CodeFile::Scan(code, errors);

	auto syntaxFile = SyntaxFile::Parse(codeFile);

	auto symtab = make_shared<CodeSymtab>();
	Interpreter::Execute(syntaxFile, symtab);
}

TEST_CASE(TestInterpreter_if_true)
{
	string code = R"loucomp(
if 1 then
	write 3
end
)loucomp";

	CodeError::List errors;
	auto codeFile = CodeFile::Scan(code, errors);

	auto syntaxFile = SyntaxFile::Parse(codeFile);

	auto symtab = make_shared<CodeSymtab>();
	Interpreter::Execute(syntaxFile, symtab);
}

TEST_CASE(TestInterpreter_if_false)
{
	string code = R"loucomp(
if 2 = 1 then
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
}