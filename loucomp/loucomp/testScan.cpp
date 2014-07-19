#include "unitTest.h"
#include "scan.h"

using namespace loucomp;
using namespace loucomp::compiler;

#define FIRST_TOKEN							auto tokenIterator = codeFile->tokens.begin()
#define TOKEN(ROW, COLUMN, VALUE, TYPE)		TEST_ASSERT(tokenIterator->row == ROW); TEST_ASSERT(tokenIterator->column == COLUMN); \
											TEST_ASSERT(tokenIterator->value == VALUE); TEST_ASSERT(tokenIterator->type == TYPE); tokenIterator++
#define LAST_TOKEN							tokenIterator++; TEST_ASSERT(tokenIterator == codeFile->tokens.end())

TEST_CASE(TestScanID)
{
	string code = R"loucomp(
read x;

if s then
x := 1;
end
)loucomp";

	CodeError::List	errors;
	auto codeFile = CodeFile::Scan(code, errors);
	

	FIRST_TOKEN;
	TOKEN(2, 1, "read", TokenType::READ);
	TOKEN(2, 6, "x", TokenType::ID);
	TOKEN(2, 7, ";", TokenType::SEMI);
	TOKEN(4, 1, "if", TokenType::IF);
	TOKEN(4, 4, "s", TokenType::ID);
	TOKEN(4, 6, "then", TokenType::THEN);
	TOKEN(5, 1, "x", TokenType::ID);
	TOKEN(5, 3, ":=", TokenType::ASSIGN);
	TOKEN(5, 6, "1", TokenType::NUM);
	TOKEN(5, 7, ";", TokenType::SEMI);
	TOKEN(6, 1, "end", TokenType::END);
	LAST_TOKEN;
	
}


#define FIRST_ERROR							auto errorIterator = errors.begin()
#define ERROR(ROW, COLUMN, VALUE, TYPE)		TEST_ASSERT(errorIterator->position.row == ROW); TEST_ASSERT(errorIterator->position.column == COLUMN); \
											TEST_ASSERT(errorIterator->position.value == VALUE); TEST_ASSERT(errorIterator->position.type == TYPE); errorIterator++
#define LAST_ERROR							TEST_ASSERT(errorIterator == errors.end())

TEST_CASE(TestErrors)
{
	string code = R"loucomp(
read x;

if s then
x :< 1;
end
)loucomp";

	CodeError::List errors;
	auto codeFile = CodeFile::Scan(code, errors);
	
	FIRST_ERROR;
	ERROR(5, 3, ":<", TokenType::UNKNOWN);
	LAST_ERROR;
}