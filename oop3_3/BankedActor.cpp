
#include "BankedActor.h"

#include <utility>
#include <stdexcept>

BankedActor::BankedActor(std::string name, Money cash, Bank& bank)
	: Actor(std::move(name), cash)
	, m_bank(bank)
	, m_accountId(m_bank.OpenAccount())
{
}

AccountId BankedActor::GetAccountId() const noexcept
{
	return m_accountId;
}

Money BankedActor::GetAccountBalance() const
{
	return m_bank.GetAccountBalance(m_accountId);
}

void BankedActor::DepositMoney(Money amount)
{
	if (amount < 0)
	{
		throw std::out_of_range("Amount cannot be negative");
	}

	if (!TrySpendCash(amount))
	{
		throw BankOperationError("Not enough cash to deposit");
	}

	try
	{
		m_bank.DepositMoney(m_accountId, amount);
	}
	catch (...)
	{
		AddCash(amount);
		throw;
	}
}

void BankedActor::WithdrawMoney(Money amount)
{
	if (amount < 0)
	{
		throw std::out_of_range("Amount cannot be negative");
	}

	m_bank.WithdrawMoney(m_accountId, amount);

	try
	{
		AddCash(amount);
	}
	catch (...)
	{
		m_bank.DepositMoney(m_accountId, amount);
		throw;
	}
}

void BankedActor::SendMoney(AccountId dstAccountId, Money amount)
{
	m_bank.SendMoney(m_accountId, dstAccountId, amount);
}

bool BankedActor::TrySendMoney(AccountId dstAccountId, Money amount)
{
	return m_bank.TrySendMoney(m_accountId, dstAccountId, amount);
}