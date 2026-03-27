#pragma once
#include <string>
#include <cstdint>
#include <map>

enum class Protocol
{
	HTTP,
	HTTPS,
	FTP
};

struct UrlComponents
{
	Protocol protocol;
	std::string host;
	std::uint16_t port;
	std::string document;
};

std::optional<UrlComponents> GetParseURL(const std::string& str);
void PrintComponents(const UrlComponents& components);