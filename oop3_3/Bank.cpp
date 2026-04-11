#include "Bank.h"

Bank::Bank(Money cash)
	: m_cashInCirculation(cash)
{
	if (cash < 0)
	{
		throw BankOperationError("Initial cash cannot be negative");
	}
}

void Bank::SendMoney(AccountId srcAccountId, AccountId dstAccountId, Money amount)
{
	if (amount < 0)
	{
		throw std::out_of_range("Amount cannot be negative");
	}

	auto srcIt = GetAccountOrThrow(srcAccountId);
	auto dstIt = GetAccountOrThrow(dstAccountId);

	if (srcIt->second < amount)
	{
		throw BankOperationError("Insufficient funds on source account");
	}

	srcIt->second -= amount;
	dstIt->second += amount;
}

bool Bank::TrySendMoney(AccountId srcAccountId, AccountId dstAccountId, Money amount)  //try catch
{
	try
	{
		SendMoney(srcAccountId, dstAccountId, amount);
		return true;
	}
	catch (const BankOperationError&)
	{
		return false;
	}
}

Money Bank::GetCash() const
{
	return m_cashInCirculation;
}

Money Bank::GetAccountBalance(AccountId accountId) const
{
	auto it = m_accounts.find(accountId);
	if (it == m_accounts.end())
	{
		throw BankOperationError("Account does not exist");
	}

	return it->second;
}

void Bank::WithdrawMoney(AccountId account, Money amount)
{
	if (amount < 0)
	{
		throw std::out_of_range("Amount cannot be negative");
	}

	auto it = m_accounts.find(account);
	if (it == m_accounts.end())
	{
		throw BankOperationError("Account does not exist");
	}

	if (it->second < amount)
	{
		throw BankOperationError("Insufficient funds on account");
	}

	it->second -= amount;
	m_cashInCirculation += amount;
}

bool Bank::TryWithdrawMoney(AccountId account, Money amount)
{
	if (amount < 0)
	{
		throw std::out_of_range("Amount cannot be negative");
	}

	auto it = m_accounts.find(account);
	if (it == m_accounts.end())
	{
		throw BankOperationError("Account does not exist");
	}

	if (it->second < amount)
	{
		return false;
	}

	it->second -= amount;
	m_cashInCirculation += amount;
	return true;
}

void Bank::DepositMoney(AccountId account, Money amount)
{
	if (amount < 0)
	{
		throw std::out_of_range("Amount cannot be negative");
	}

	auto it = m_accounts.find(account);
	if (it == m_accounts.end())
	{
		throw BankOperationError("Account does not exist");
	}

	if (m_cashInCirculation < amount)
	{
		throw BankOperationError("Insufficient cash in circulation");
	}

	m_cashInCirculation -= amount;
	it->second += amount;
}

AccountId Bank::OpenAccount()
{
	const AccountId newAccountId = m_nextAccountId;

	auto [it, inserted] = m_accounts.emplace(newAccountId, 0);
	if (!inserted)
	{
		throw BankOperationError("Failed to open account");
	}

	++m_nextAccountId;
	return newAccountId;
}

Money Bank::CloseAccount(AccountId accountId)
{
	auto it = m_accounts.find(accountId);
	if (it == m_accounts.end())
	{
		throw BankOperationError("Account does not exist");
	}

	const Money balance = it->second;
	m_cashInCirculation += balance;
	m_accounts.erase(it);

	return balance;
}


Bank::AccountIterator Bank::GetAccountOrThrow(AccountId id)
{
	auto it = m_accounts.find(id);
	if (it == m_accounts.end())
	{
		throw BankOperationError("Account does not exist");
	}
	return it;
}