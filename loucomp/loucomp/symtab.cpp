#include "symtab.h"
#include "interpreter.h"

namespace loucomp
{
	namespace interpreter
	{
		void CodeSymtab::AddToTable(string symbol, int value)
		{
			table[symbol] = value;
		}

		bool CodeSymtab::SearchForSymbol(string symbol)
		{
			if (table.find(symbol) != table.end())
			{
				return true;
			}
			else
			{
				return false;
			}
		}

		void CodeSymtab::ChangeSymbol(string symbol, int value)
		{
			table[symbol] = value;
		}

		int CodeSymtab::ReturnSymbol(string symbol)
		{
			return table[symbol];
		}
	}
}