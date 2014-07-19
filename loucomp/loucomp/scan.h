#ifndef LOUCOMP_SCAN_H
#define LOUCOMP_SCAN_H

#include <iostream>
#include <vector>
#include <string>
#include <memory>

using namespace std;

namespace loucomp
{
	namespace compiler
	{
		enum class TokenType
		{
			// bool  keeping tokens
			ENDFILE,
			UNKNOWN,
			// reserved 
			IF,
			THEN,
			ELSE,
			END,
			REPEAT,
			UNTIL,
			READ,
			WRITE,
			// multi-character
			ID,
			NUM,
			FLOAT,
			// special symbols
			ASSIGN,
			EQ,
			LT,
			PLUS,
			MINUS,
			TIMES,
			OVER,
			LPAREN,
			RPAREN,
			SEMI
		};

		struct CodeToken
		{
			typedef vector<CodeToken>	List;

			TokenType	type = TokenType::UNKNOWN;
			int			row = -1;
			int			column = -1;
			string		value;
		};

		struct CodeError
		{
			typedef	vector<CodeError>	List;

			CodeToken	position;
			string		message;
		};

		struct CodeFile
		{
			typedef shared_ptr<CodeFile>	Ptr;
			
			CodeToken::List					tokens;
			static	CodeFile::Ptr			Scan(const string & code, CodeError::List & errors);
		};

	}
}


#endif
