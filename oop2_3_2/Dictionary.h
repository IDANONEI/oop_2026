#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>

using DictionaryMap = std::unordered_map<std::string, std::unordered_set<std::string>>;
void AddTranslation(DictionaryMap& dict, DictionaryMap& reverseDict, const std::string& key, const std::string& translation);
const std::unordered_set<std::string>* FindTranslation(const DictionaryMap& dict, const std::string& key);
void LoadDictionary(const std::string& fileName, DictionaryMap& dict, DictionaryMap& reverseDict);
void ExportDictionary(const std::string& fileName, const DictionaryMap& dict);
