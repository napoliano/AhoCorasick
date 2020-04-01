
#include <iostream>
#include "AhoCorasick.h"

void Example();

int main(void)
{
	Example();
	return 0;
}

void Example()
{
	std::locale::global(std::locale("kor"));
	std::vector<std::wstring> words{ L"he", L"his" , L"hers", L"she" };

	AhoCorasick patternMatchingMachine;
	patternMatchingMachine.Initialize(words);

	std::wstring data = L"hisheshehers";
	auto patterns = patternMatchingMachine.Match(data);
	for (auto pattern : patterns)
	{
		std::wcout << pattern << std::endl;
	}
}