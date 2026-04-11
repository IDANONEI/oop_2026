#include "Simulation.h"

#include <iostream>

namespace
{
    constexpr Money INIT_CASH_HOMER = 5000;
    constexpr Money INIT_CASH_MARGE = 5000;
    constexpr Money INIT_CASH_BART = 500;
    constexpr Money INIT_CASH_LISA = 500;
    constexpr Money INIT_CASH_APU = 25000;
    constexpr Money INIT_CASH_MR_BURNS = 50000;
}

Simulation::Simulation()
    : m_bank(
        INIT_CASH_HOMER +
        INIT_CASH_MARGE +
        INIT_CASH_BART +
        INIT_CASH_LISA +
        INIT_CASH_APU +
        INIT_CASH_MR_BURNS)
    , m_homer(INIT_CASH_HOMER, m_bank)
    , m_marge(INIT_CASH_MARGE, m_bank)
    , m_bart(INIT_CASH_BART)
    , m_lisa(INIT_CASH_LISA)
    , m_apu(INIT_CASH_APU, m_bank)
    , m_burns(INIT_CASH_MR_BURNS, m_bank)
    , m_initialCash(
        INIT_CASH_HOMER +
        INIT_CASH_MARGE +
        INIT_CASH_BART +
        INIT_CASH_LISA +
        INIT_CASH_APU +
        INIT_CASH_MR_BURNS)
{
    m_actors.homer = &m_homer;
    m_actors.marge = &m_marge;
    m_actors.bart = &m_bart;
    m_actors.lisa = &m_lisa;
    m_actors.apu = &m_apu;
    m_actors.burns = &m_burns;

	m_homer.DepositMoney(3000);
	m_marge.DepositMoney(3000);
	m_apu.DepositMoney(20000);
	m_burns.DepositMoney(40000);
}

void Simulation::Run(size_t iterations)
{
    for (size_t iter = 0; iter < iterations; ++iter)
    {
        std::cout << "Iteration " << (iter + 1) << '\n';

        m_homer.Act(m_actors);
        m_marge.Act(m_actors);
        m_bart.Act(m_actors);
        m_lisa.Act(m_actors);
        m_apu.Act(m_actors);
        m_burns.Act(m_actors);

        std::cout << '\n';
    }

    PrintStats();

    if (IsNormalState())
    {
        std::cout << "Simulation is in normal state\n";
    }
    else
    {
        std::cout << "Simulation is NOT in normal state\n";
    }
}


bool Simulation::IsValid() const
{
	return IsNormalState();
}

bool Simulation::IsNormalState() const
{
    const Money charactersCash =
        m_homer.GetCash() +
        m_marge.GetCash() +
        m_bart.GetCash() +
        m_lisa.GetCash() +
        m_apu.GetCash() +
        m_burns.GetCash();

    const Money charactersBalance =
        m_homer.GetAccountBalance() +
        m_marge.GetAccountBalance() +
        m_apu.GetAccountBalance() +
        m_burns.GetAccountBalance();

    const Money totalMoney = charactersCash + charactersBalance;

    return charactersCash == m_bank.GetCash()
        && totalMoney == m_initialCash;
}

void Simulation::PrintStats() const
{
    const Money charactersCash =
        m_homer.GetCash() +
        m_marge.GetCash() +
        m_bart.GetCash() +
        m_lisa.GetCash() +
        m_apu.GetCash() +
        m_burns.GetCash();

    const Money charactersBalance =
        m_homer.GetAccountBalance() +
        m_marge.GetAccountBalance() +
        m_apu.GetAccountBalance() +
        m_burns.GetAccountBalance();

    const Money totalMoney = charactersCash + charactersBalance;
    std::cout << "Stats:\n";

    std::cout << "Homer: cash = " << m_homer.GetCash()
              << ", balance = " << m_homer.GetAccountBalance() << '\n';

    std::cout << "Marge: cash = " << m_marge.GetCash()
              << ", balance = " << m_marge.GetAccountBalance() << '\n';

    std::cout << "Bart: cash = " << m_bart.GetCash() << '\n';
    std::cout << "Lisa: cash = " << m_lisa.GetCash() << '\n';

    std::cout << "Apu: cash = " << m_apu.GetCash()
              << ", balance = " << m_apu.GetAccountBalance() << '\n';

    std::cout << "MrBurns: cash = " << m_burns.GetCash()
              << ", balance = " << m_burns.GetAccountBalance() << '\n';

    std::cout << "Bank cash: " << m_bank.GetCash() << '\n';
    std::cout << "Characters cash: " << charactersCash << '\n';
    std::cout << "Characters balance: " << charactersBalance << '\n';
    std::cout << "Total money: " << totalMoney << '\n';
    std::cout << "Initial money: " << m_initialCash << '\n';
}