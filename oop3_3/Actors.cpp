#include "Actors.h"

#include <iostream>
#include <algorithm>

namespace
{
	constexpr Money HOMER_TO_MARGE = 20;
	constexpr Money HOMER_ELECTRICITY_PAYMENT = 15;
	constexpr Money HOMER_TO_BART_CASH = 5;
	constexpr Money HOMER_TO_LISA_CASH = 5;

	constexpr Money MARGE_TO_APU = 18;

	constexpr Money BART_TO_APU_CASH = 3;
	constexpr Money LISA_TO_APU_CASH = 2;

	constexpr Money APU_TO_BURNS = 10;
	constexpr Money APU_DEPOSIT_LIMIT = 20;

	constexpr Money BURNS_TO_HOMER_SALARY = 25;
}

void Homer::Act(Actors& actors)
{
	if (actors.marge != nullptr)
	{
		if (TrySendMoney(actors.marge->GetAccountId(), HOMER_TO_MARGE))
		{
			std::cout << "Homer sent " << HOMER_TO_MARGE << " to Marge\n";
		}
		else
		{
			std::cout << "Homer skipped transfer to Marge\n";
		}
	}

	if (actors.burns != nullptr)
	{
		if (TrySendMoney(actors.burns->GetAccountId(), HOMER_ELECTRICITY_PAYMENT))
		{
			std::cout << "Homer paid " << HOMER_ELECTRICITY_PAYMENT << " for electricity to MrBurns\n";
		}
		else
		{
			std::cout << "Homer skipped electricity payment\n";
		}
	}

	if (actors.bart != nullptr)
	{
		try
		{
			WithdrawMoney(HOMER_TO_BART_CASH);

			if (TrySpendCash(HOMER_TO_BART_CASH))
			{
				actors.bart->AddCash(HOMER_TO_BART_CASH);
				std::cout << "Homer gave " << HOMER_TO_BART_CASH << " cash to Bart\n";
			}
			else
			{
				std::cout << "Homer could not give cash to Bart\n";
			}
		}
		catch (const std::exception&)
		{
			std::cout << "Homer could not give cash to Bart\n";
		}
	}

	if (actors.lisa != nullptr)
	{
		try
		{
			WithdrawMoney(HOMER_TO_LISA_CASH);

			if (TrySpendCash(HOMER_TO_LISA_CASH))
			{
				actors.lisa->AddCash(HOMER_TO_LISA_CASH);
				std::cout << "Homer gave " << HOMER_TO_LISA_CASH << " cash to Lisa\n";
			}
			else
			{
				std::cout << "Homer could not give cash to Lisa\n";
			}
		}
		catch (const std::exception&)
		{
			std::cout << "Homer could not give cash to Lisa\n";
		}
	}
}

void Marge::Act(Actors& actors)
{
	if (actors.apu == nullptr)
	{
		return;
	}

	if (TrySendMoney(actors.apu->GetAccountId(), MARGE_TO_APU))
	{
		std::cout << "Marge bought groceries from Apu for " << MARGE_TO_APU << '\n';
	}
	else
	{
		std::cout << "Marge skipped grocery shopping\n";
	}
}

void Bart::Act(Actors& actors)
{
	if (actors.apu == nullptr)
	{
		return;
	}

	if (TrySpendCash(BART_TO_APU_CASH))
	{
		actors.apu->AddCash(BART_TO_APU_CASH);
		std::cout << "Bart paid cash to Apu: " << BART_TO_APU_CASH << '\n';
	}
	else
	{
		std::cout << "Bart skipped purchase\n";
	}
}

void Lisa::Act(Actors& actors)
{
	if (actors.apu == nullptr)
	{
		return;
	}

	if (TrySpendCash(LISA_TO_APU_CASH))
	{
		actors.apu->AddCash(LISA_TO_APU_CASH);
		std::cout << "Lisa paid cash to Apu: " << LISA_TO_APU_CASH << '\n';
	}
	else
	{
		std::cout << "Lisa skipped purchase\n";
	}
}

void Apu::Act(Actors& actors)
{
	if (actors.burns != nullptr)
	{
		if (TrySendMoney(actors.burns->GetAccountId(), APU_TO_BURNS))
		{
			std::cout << "Apu paid electricity bill to MrBurns: " << APU_TO_BURNS << '\n';
		}
		else
		{
			std::cout << "Apu skipped electricity payment\n";
		}
	}

	const Money cashToDeposit = std::min(GetCash(), APU_DEPOSIT_LIMIT);
	if (cashToDeposit > 0)
	{
		try
		{
			DepositMoney(cashToDeposit);
			std::cout << "Apu deposited cash to bank: " << cashToDeposit << '\n';
		}
		catch (const std::exception&)
		{
			std::cout << "Apu could not deposit cash\n";
		}
	}
	else
	{
		std::cout << "Apu has no cash to deposit\n";
	}
}

void MrBurns::Act(Actors& actors)
{
	if (actors.homer == nullptr)
	{
		return;
	}

	if (TrySendMoney(actors.homer->GetAccountId(), BURNS_TO_HOMER_SALARY))
	{
		std::cout << "MrBurns paid salary to Homer: " << BURNS_TO_HOMER_SALARY << '\n';
	}
	else
	{
		std::cout << "MrBurns skipped salary payment\n";
	}
}