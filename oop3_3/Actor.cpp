#include "Actor.h"

#include <utility>
#include <stdexcept>

Actor::Actor(const std::string& name, Money cash)
	: m_name(name)
	, m_cash(cash)
{
	if (cash < 0)
	{
		throw std::out_of_range("Actor cash cannot be negative");
	}
}

Money Actor::GetCash() const noexcept
{
	return m_cash;
}

const std::string& Actor::GetName() const noexcept
{
	return m_name;
}

void Actor::AddCash(Money cash)
{
	if (cash < 0)
	{
		throw std::out_of_range("Cannot add negative cash");
	}

	m_cash += cash;
}

bool Actor::TrySpendCash(Money cash)
{
	if (cash < 0)
	{
		throw std::out_of_range("Cannot spend negative cash");
	}

	if (m_cash < cash)
	{
		return false;
	}

	m_cash -= cash;
	return true;
}