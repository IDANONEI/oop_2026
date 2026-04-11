#pragma once

#include "Actor.h"
#include "Bank.h"

class BankedActor : public Actor
{
public:
	BankedActor(std::string name, Money cash, Bank& bank); // ссыока
	virtual ~BankedActor() = default;

	[[nodiscard]] AccountId GetAccountId() const noexcept;
	[[nodiscard]] Money GetAccountBalance() const;

	void DepositMoney(Money amount);
	void WithdrawMoney(Money amount);

	void SendMoney(AccountId dstAccountId, Money amount);
	[[nodiscard]] bool TrySendMoney(AccountId dstAccountId, Money amount);

private:
	Bank& m_bank;
	AccountId m_accountId;
};

//доработать архитектуру с крипто кошльками и покзать изменения классов барта апу и лизу

