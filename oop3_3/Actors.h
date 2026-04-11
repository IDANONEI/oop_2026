#pragma once

#include "Actor.h"
#include "BankedActor.h"

class Homer;
class Marge;
class Bart;
class Lisa;
class Apu;
class MrBurns;

struct Actors
{
	Homer* homer = nullptr;
	Marge* marge = nullptr;
	Bart* bart = nullptr;
	Lisa* lisa = nullptr;
	Apu* apu = nullptr;
	MrBurns* burns = nullptr;
};

class Homer : public BankedActor
{
public:
	explicit Homer(Money cash, Bank& bank)
		: BankedActor("Homer", cash, bank)
	{
	}

	void Act(Actors& actors) override;
};

class Marge : public BankedActor
{
public:
	explicit Marge(Money cash, Bank& bank)
		: BankedActor("Marge", cash, bank)
	{
	}

	void Act(Actors& actors) override;
};

class Bart : public Actor
{
public:
	explicit Bart(Money cash)
		: Actor("Bart", cash)
	{
	}

	void Act(Actors& actors) override;
};

class Lisa : public Actor
{
public:
	explicit Lisa(Money cash)
		: Actor("Lisa", cash)
	{
	}

	void Act(Actors& actors) override;
};

class Apu : public BankedActor
{
public:
	explicit Apu(Money cash, Bank& bank)
		: BankedActor("Apu", cash, bank)
	{
	}

	void Act(Actors& actors) override;
};

class MrBurns : public BankedActor
{
public:
	explicit MrBurns(Money cash, Bank& bank)
		: BankedActor("MrBurns", cash, bank)
	{
	}

	void Act(Actors& actors) override;
};