#pragma once
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <optional>
#include <functional>

using TranslationSet = std::unordered_set<std::string>;
using DictionaryMap = std::unordered_map<std::string, TranslationSet>;
struct DictionaryStorage
{
	DictionaryMap direct;
	DictionaryMap reverse;
};
void AddTranslation(DictionaryStorage& dictionary, const std::string& key, const std::string& translation);
using TranslationResult = std::optional<std::reference_wrapper<const TranslationSet>>;//reference_wrapper
TranslationResult FindTranslation(const DictionaryMap& dict, const std::string& key);
void LoadDictionary(const std::string& fileName, DictionaryStorage& dictionary);
void ExportDictionary(const std::string& fileName, const DictionaryStorage& dictionary);
