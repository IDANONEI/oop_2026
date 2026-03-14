#include "ParseUrlModules.h"
#include <regex>
#include <algorithm>
#include <optional>
#include <stdexcept>
#include <cctype>
#include <iostream>


namespace
{
	std::string ToLower(const std::string s)
	{
		std::string result = s;
		std::transform(result.begin(), result.end(), result.begin(), ::tolower);
		return result;
	}

	std::optional<Protocol> GetProtocol(const std::string& str)
	{
		if (str == "http") return Protocol::HTTP;
		if (str == "https") return Protocol::HTTPS;
		if (str == "ftp") return Protocol::FTP;

		return std::nullopt;
	}

	std::optional<std::string> GetHost(const std::string& str)
	{
		if (str.empty())
			return std::nullopt;
		return str;
	}

	std::uint16_t GetDefaultPort(Protocol protocol)
	{
		auto it = DEFAULT_PORTS.find(protocol);
		if (it != DEFAULT_PORTS.end())
		{
			return it->second;
		}

		throw std::logic_error("Unknown protocol");
	}

	std::optional<std::uint16_t> GetPort(const std::string& str, const Protocol& protocol)
	{
		if (str.empty())
			return GetDefaultPort(protocol);

		int value = std::stoi(str);
		if (value < START_PORT || value > FINISH_PORT)
			return std::nullopt;

		return static_cast<std::uint16_t>(value);
	}

	std::string GetDocument(const std::string& str)
		{
			if (str.empty())
			{
				return "";
			}

			static const std::regex pathPattern(R"(^([^?#]*)(?:\?[^#]*)?(?:#.*)?$)");
			std::smatch match;

			if (!std::regex_match(str, match, pathPattern))
			{
				return "";
			}

			std::string path = match[1].str();
			if (path.empty())
			{
				return "";
			}

			static const std::regex filePattern(R"((?:.*/)?([^/.][^/]*\.[^/.][^/]*)$)");
			std::smatch fileMatch;

			if (!std::regex_match(path, fileMatch, filePattern))
			{
				return "";
			}

			return fileMatch[1].str();
		}
}

bool ParseURL(const std::string& str, UrlComponents& result)
{
	static const std::regex pattern(R"(^([A-Za-z]+)://([^/:]+)(?::([0-9]+))?(?:/(.*))?$)");
	std::smatch match;

	if (!std::regex_match(str, match, pattern))
	{
		return false;
	}

	auto protocol = GetProtocol(ToLower(match[1].str()));
	auto host = GetHost(match[2].str());

	if (!protocol || !host)
	{
		return false;
	}

	auto port = GetPort(match[3].str(), *protocol);
	if (!port)
	{
		return false;
	}

	auto document = GetDocument(match[4].str());

	result.protocol = *protocol;
	result.host = *host;
	result.port = *port;
	result.document = document;

	return true;
}


void PrintComponents(const UrlComponents& components)
{
	std::cout << "HOST: " << components.host << std::endl;
	std::cout << "PORT: " << components.port << std::endl;
	std::cout << "DOC: " << components.document << std::endl;
}