#pragma once

#include <memory>
#include <cwchar>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <climits>

#include <iostream>

class AhoCorasick
{
private:
	class Node
	{
	private:
		wchar_t _letter = 0;
		std::wstring _word = L"";
		std::unordered_map<wchar_t, std::shared_ptr<Node>> _nextNodes;

		std::shared_ptr<Node> _failureLink = nullptr;
		std::shared_ptr<Node> _outputLink = nullptr;

	public:
		Node(wchar_t letter) : _letter(letter) {}
		~Node() {}

	public:
		wchar_t GetLetter() const { return _letter; }
		std::wstring GetWord() const { return _word; }
		void SetWord(std::wstring word) { _word = word; }

		void AddNextNode(wchar_t letter);
		std::shared_ptr<Node> GetNextNode(wchar_t letter);
		std::unordered_map<wchar_t, std::shared_ptr<Node>>& GetNextNodeMap() { return _nextNodes; }

		void SetFailureLink(std::shared_ptr<Node> node) { _failureLink = node; }
		std::shared_ptr<Node> GetFailureLink() { return _failureLink; }

		void SetOutputLink(std::shared_ptr<Node> node) { _outputLink = node; }
		std::shared_ptr<Node> GetOutputLink() { return _outputLink; }
	};

	std::shared_ptr<Node> _rootNode = nullptr;

public:
	AhoCorasick() : _rootNode(std::make_shared<Node>(0)) {}
	~AhoCorasick();

public:
	void Initialize(const std::vector<std::wstring>& words);
	std::vector<std::wstring> Match(const std::wstring& data);

private:
	void MakeTrie(const std::vector<std::wstring>& words);
	void ConnectLink();
	void DisconnectLink();
};
