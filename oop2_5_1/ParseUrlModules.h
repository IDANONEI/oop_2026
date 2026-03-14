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

inline const std::uint16_t START_PORT = 1;
inline const std::uint16_t FINISH_PORT = 65535;

inline const std::map<Protocol, std::uint16_t> DEFAULT_PORTS =
{
	{Protocol::HTTP, 80},
	{Protocol::HTTPS, 443},
	{Protocol::FTP, 21}
};

bool ParseURL(const std::string& url, UrlComponents& components);
void PrintComponents(const UrlComponents& components);