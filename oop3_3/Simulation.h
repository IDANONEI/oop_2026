#pragma once

#include "Actors.h"
#include "Bank.h"

class Simulation
{
public:
	Simulation();

	void Run(size_t iterations);
	void PrintStats() const;
	[[nodiscard]] bool IsValid() const;

private:
	bool IsNormalState() const;

	Bank m_bank;

	Homer m_homer;
	Marge m_marge;
	Bart m_bart;
	Lisa m_lisa;
	Apu m_apu;
	MrBurns m_burns;

	Actors m_actors;

	Money m_initialCash = 0;
};