#include "unitTest.h"
#include "scan.h"
#include "parse.h"

using namespace::loucomp;
using namespace::loucomp::compiler;


TEST_CASE(TestParse_assign)
{
	string code = R"loucomp(
x := 1;
y := 2
)loucomp";

	CodeError::List errors;
	auto codeFile = CodeFile::Scan(code, errors);

	auto syntaxFile = SyntaxFile::Parse(codeFile);

}

TEST_CASE(TestParse_if)
{
	string code = R"loucomp(
v := 0;
if v=0 then
	v := 1;
else
	v := 2;
end;
x := 1;
y := 2
)loucomp";

	CodeError::List errors;
	auto codeFile = CodeFile::Scan(code, errors);

	auto syntaxFile = SyntaxFile::Parse(codeFile);

}

TEST_CASE(TestParse_repeat)
{
	string code = R"loucomp(
v := 0;
repeat
	v := v + 1;
until v = 10;
x := 1;
y := 2
)loucomp";

	CodeError::List errors;
	auto codeFile = CodeFile::Scan(code, errors);

	auto syntaxFile = SyntaxFile::Parse(codeFile);

}