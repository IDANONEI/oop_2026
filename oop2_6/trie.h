#pragma once

#include <string>
#include <map>

struct TrieNode
{
	char letter = '\0';
	bool isKey = false;
	std::string key;
	std::string value;

	std::map<char, TrieNode*> next;
	TrieNode* fail = nullptr;
};

TrieNode* CreateNode(char letter = '\0');

bool AddKey(TrieNode* root, const std::string& key, const std::string& value);

TrieNode* BuildTrie(const std::map<std::string, std::string>& params);

void BuildFailureLinks(TrieNode* root);

void DeleteTrie(TrieNode* root);