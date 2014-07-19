// #include <iostream>
#include "scan.h"

namespace loucomp
{
	namespace compiler
	{
		CodeFile::Ptr CodeFile::Scan(const string & code, CodeError::List & errors)
		{
			auto codeFile = make_shared<CodeFile>();
			enum class State
			{
				START,
				INASSIGN,
				INCOMMENT,
				INNUM,
				INID,
				DONE,
			};

			const char * reading = code.c_str();
			const char * begin = nullptr;
			const char * rowBegin = reading;
			int rowNumber = 1;
			State state = State::START;

			auto AddToken = [&](int length, TokenType type)
			{
				auto tokenBegin = begin ? begin : reading;
				string value(tokenBegin, tokenBegin + length);

				switch (type)
				{
				case TokenType::ID:
					type =
						value == "if" ? TokenType::IF :
						value == "then" ? TokenType::THEN :
						value == "else" ? TokenType::ELSE :
						value == "end" ? TokenType::END :
						value == "repeat" ? TokenType::REPEAT :
						value == "until" ? TokenType::UNTIL :
						value == "read" ? TokenType::READ :
						value == "write" ? TokenType::WRITE :
						TokenType::ID;
					break;
				}

				CodeToken token;
				token.type = type;
				token.row = rowNumber;
				token.column = tokenBegin - rowBegin + 1;
				token.value = value;

				codeFile->tokens.push_back(token);
			};

			auto AddError = [&](int length, const string & message)
			{
				auto tokenBegin = begin ? begin : reading;
				string value(tokenBegin, tokenBegin + length);

				CodeToken token;
				token.type = TokenType::UNKNOWN;
				token.row = rowNumber;
				token.column = tokenBegin - rowBegin + 1;
				token.value = value;

				CodeError error = 
				{
					token,
					message
				};

				errors.push_back(error);
			};

			while (auto c = *reading)
			{
				switch (state)
				{
				case State::START:
					switch (c) 
					{
					case '(':
						AddToken(1, TokenType::LPAREN);
						break;
					case ')':
						AddToken(1, TokenType::RPAREN);
						break;
					case ';':
						AddToken(1, TokenType::SEMI);
						break;
					case '+':
						AddToken(1, TokenType::PLUS);
						break;
					case '-':
						AddToken(1, TokenType::MINUS);
						break;
					case '*':
						AddToken(1, TokenType::TIMES);
						break;
					case '/':
						AddToken(1, TokenType::OVER);
						break;
					case '#':
						begin = reading;
						state = State::INCOMMENT;
						break;
					case '=':
						AddToken(1, TokenType::EQ);
						break;
					case '<':
						AddToken(1, TokenType::LT);
						break;
					case ':':
						switch (reading[1])
						{
						case '=':
							AddToken(2, TokenType::ASSIGN);
							reading++;
							break;
						default:
							AddError(2, "unknown character");
						}
						break;
					case ' ':case '\t':case '\r':
						break;
					case '\n':
						rowNumber++;
						rowBegin = reading + 1;
						break;
					default:
						if ('0' <= c && c <= '9')
						{
							begin = reading;
							state = State::INNUM;
						}
						else if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_')
						{
							begin = reading;
							state = State::INID;
						}
						else
						{
							AddError(1, "unknown character: " + string(reading, reading+1));
						}
					}
					break;

				case State::INCOMMENT:
					switch (c) 
					{
					case '\n':
						state = State::START;
						begin = nullptr;
						reading--;
						break;
					}
					break;

				case State::INNUM:
					if ('0' <= c && c <= '9')
					{

					}
					else
					{
						AddToken(reading - begin, TokenType::NUM);
						state = State::START;
						reading--;
						begin = nullptr;
					}
					break;

				case State::INID:
					if ('a' <= c && c <= 'z' || 'A' <= c && c <= 'Z' || c == '_' || c == '.' || c == '-')
					{

					}
					else
					{
						AddToken(reading - begin, TokenType::ID);
						state = State::START;
						reading--;
						begin = nullptr;
					}
					break;
				}
				reading++;
			}

			switch (state)
			{
			case State::INNUM:
				AddToken(reading - begin, TokenType::NUM);
				break;
			case State::INID:
				AddToken(reading - begin, TokenType::ID);
				break;

			}

			CodeToken endToken;
			endToken.type = TokenType::ENDFILE;
			codeFile->tokens.push_back(endToken);
			return codeFile;
		}
	}
}

