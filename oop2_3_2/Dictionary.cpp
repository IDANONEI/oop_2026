#include "Dictionary.h"

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <stdexcept>
#include <filesystem>
#include <cwctype>

namespace
{
	std::wstring ToWString(const std::string& str)
	{
		try
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
			return conv.from_bytes(str);
		}
		catch (const std::range_error&)
		{
			throw std::runtime_error("Ошибка преобразования строки");
		}
	}

	std::string ToString(const std::wstring& wstr)
	{
		try
		{
			std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
			return conv.to_bytes(wstr);
		}
		catch (const std::range_error&)
		{
			throw std::runtime_error("Ошибка преобразования строки");
		}
	}

	std::string NormalizeWord(const std::string& input)
	{
		std::wstring w = ToWString(input);

		for (wchar_t& ch : w)
		{
			ch = std::towlower(ch);
		}

		return ToString(w);
	}

	std::vector<std::string> SplitByTab(const std::string& line)
	{
		std::vector<std::string> parts;
		std::string part;
		std::istringstream iss(line);

		while (std::getline(iss, part, '\t'))
		{
			parts.push_back(part);
		}

		return parts;
	}
}

void AddTranslation(DictionaryMap& dict, DictionaryMap& reverseDict, const std::string& key, const std::string& translation)
{
	dict[NormalizeWord(key)].insert(translation);
	reverseDict[NormalizeWord(translation)].insert(key);
}

const std::unordered_set<std::string>* FindTranslation(const DictionaryMap& dict, const std::string& key)
{
	auto it = dict.find(NormalizeWord(key));
	if (it != dict.end())
	{
		return &it->second;
	}

	return nullptr;
}

void LoadDictionary(const std::string& fileName, DictionaryMap& dict, DictionaryMap& reverseDict)
{
	if (!std::filesystem::exists(fileName))
	{
		return;
	}

	std::ifstream file(fileName);
	if (!file.is_open())
	{
		throw std::runtime_error("Не удалось открыть файл словаря: " + fileName);
	}

	std::string line;
	while (std::getline(file, line))
	{
		if (line.empty())
		{
			continue;
		}

		auto parts = SplitByTab(line);
		if (parts.size() < 2)
		{
			continue;
		}

		const std::string& key = parts[0];
		for (size_t i = 1; i < parts.size(); ++i)
		{
			if (!parts[i].empty())
			{
				AddTranslation(dict, reverseDict, key, parts[i]);
			}
		}
	}

	if (!file.eof() && file.fail())
	{
		throw std::runtime_error("Ошибка чтения файла словаря: " + fileName);
	}
}

void ExportDictionary(const std::string& fileName, const DictionaryMap& dict)
{
	std::ofstream file(fileName);
	if (!file.is_open())
	{
		throw std::runtime_error("Не удалось открыть файл для записи: " + fileName);
	}

	for (const auto& [normalizedKey, translations] : dict)
	{
		file << normalizedKey;
		if (!file)
		{
			throw std::runtime_error("Ошибка записи в файл словаря: " + fileName);
		}

		for (const auto& translation : translations)
		{
			file << '\t' << translation;
			if (!file)
			{
				throw std::runtime_error("Ошибка записи в файл словаря: " + fileName);
			}
		}
		file << '\n';
		if (!file)
		{
			throw std::runtime_error("Ошибка записи в файл словаря: " + fileName);
		}
	}
}