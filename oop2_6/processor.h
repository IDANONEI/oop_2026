#pragma once

#include <string>
#include <vector>
#include <map>
#include <istream>

#include "trie.h"

struct Match
{
	std::size_t start = 0;
	std::size_t end = 0;
	std::string key;
	std::string value;
};

std::string ReadAll(std::istream& in);

std::vector<Match> FindMatches(const std::string& text, TrieNode* root);

std::vector<Match> SelectMatches(const std::vector<Match>& matches);

std::string ReplaceMatches(const std::string& text, const std::vector<Match>& matches);

std::string ProcessText(const std::string& text, TrieNode* root);

std::string ExpandTemplate(const std::string& tpl,
						   const std::map<std::string, std::string>& params);