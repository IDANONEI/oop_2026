#pragma once

#include <string>
using Money = long long;

struct Actors;

class Actor
{
public:
	explicit Actor(const std::string& name, Money cash);
	virtual ~Actor() = default;

	virtual void Act(Actors& actors) = 0;

	[[nodiscard]] Money GetCash() const noexcept;
	[[nodiscard]] const std::string& GetName() const noexcept;
	void AddCash(Money cash);
	[[nodiscard]] bool TrySpendCash(Money cash);

private:
	std::string m_name;
	Money m_cash = 0;
};