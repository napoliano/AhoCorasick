#include "AhoCorasick.h"


std::shared_ptr<AhoCorasick::Node> AhoCorasick::Node::GetNextNode(wchar_t letter)
{
	if (0 == _nextNodes.count(letter))
		return nullptr;

	return _nextNodes[letter];
}

void AhoCorasick::Node::AddNextNode(wchar_t letter)
{
	if (0 == _nextNodes.count(letter))
	{
		_nextNodes.emplace(letter, std::make_shared<Node>(letter));
	}
}

AhoCorasick::~AhoCorasick()
{
	DisconnectLink();
}

void AhoCorasick::Initialize(const std::vector<std::wstring>& words)
{
	MakeTrie(words);

	ConnectLink();
}

std::vector<std::wstring> AhoCorasick::Match(const std::wstring& data)
{
	std::vector<std::wstring> words;

	auto currentNode = _rootNode;
	const size_t length = data.length();
	for (int i = 0; i < length; ++i)
	{
		const wchar_t letter = data[i];

		bool isExist = true;
		auto nextNode = currentNode->GetNextNode(letter);
		while (nullptr == nextNode)
		{
			if (&(*_rootNode) == &(*currentNode))
			{
				isExist = false;
				break;
			}

			currentNode = currentNode->GetFailureLink();
			nextNode = currentNode->GetNextNode(letter);
		}

		if (false == isExist)
			continue;

		const auto word = nextNode->GetWord();
		if (L"" != word)
			words.emplace_back(word);

		auto outputLink = nextNode->GetOutputLink();
		while (nullptr != outputLink)
		{
			words.emplace_back(outputLink->GetWord());
			outputLink = outputLink->GetOutputLink();
		}

		currentNode = nextNode;
	}

	return words;
}

void AhoCorasick::MakeTrie(const std::vector<std::wstring>& words)
{
	for (const auto& word : words)
	{
		auto currentNode = _rootNode;
		for (const auto& letter : word)
		{
			currentNode->AddNextNode(letter);
			currentNode = currentNode->GetNextNode(letter);
		}

		currentNode->SetWord(word);
	}
}

void AhoCorasick::ConnectLink()
{
	std::queue<std::shared_ptr<Node>> bfsQueue;
	_rootNode->SetFailureLink(_rootNode);

	const auto& rootNextNodeMap = _rootNode->GetNextNodeMap();
	for (const auto& pair : rootNextNodeMap)
	{
		const auto rootNextNode = pair.second;
		bfsQueue.push(rootNextNode);

		rootNextNode->SetFailureLink(_rootNode);
	}

	while (false == bfsQueue.empty())
	{
		const auto p = bfsQueue.front();
		bfsQueue.pop();

		const auto& nextNodeMap = p->GetNextNodeMap();
		for (const auto& pair : nextNodeMap)
		{
			const auto q = pair.second;
			bfsQueue.push(q);

			auto pf = p->GetFailureLink();
			for (int i = 0; i < INT_MAX; ++i)
			{
				const auto r = pf->GetNextNode(q->GetLetter());
				if (nullptr != r)
				{
					q->SetFailureLink(r);
					break;
				}

				if (&(*_rootNode) == &(*pf))
				{
					q->SetFailureLink(pf);
					break;
				}

				pf = pf->GetFailureLink();
			}

			const auto failureLink = q->GetFailureLink();
			const auto outputLink = (L"" != failureLink->GetWord()) ? failureLink : failureLink->GetOutputLink();
			q->SetOutputLink(outputLink);
		}
	}
}

void AhoCorasick::DisconnectLink()
{
	std::queue<std::shared_ptr<Node>> bfsQueue;
	bfsQueue.push(_rootNode);

	while (false == bfsQueue.empty())
	{
		const auto currentNode = bfsQueue.front();
		bfsQueue.pop();

		currentNode->SetFailureLink(nullptr);
		currentNode->SetOutputLink(nullptr);

		const auto& nextNodeMap = currentNode->GetNextNodeMap();
		for (const auto& pair : nextNodeMap)
		{
			bfsQueue.push(pair.second);
		}
	}
}