#include <catch2/catch_test_macros.hpp>

#include "Actors.h"
#include "Bank.h"
#include "Simulation.h"

TEST_CASE("Банк")
{
	SECTION("Инициализация банка с положительной суммой")
	{
		constexpr Money initial = 10000;
		Bank bank(initial);

		REQUIRE(bank.GetCash() == initial);
	}

	SECTION("Инициализация с отрицательной суммой")
	{
		REQUIRE_THROWS_AS(Bank(-1), BankOperationError);
	}

	SECTION("Открытие счёта создаёт счёт с нулевым балансом")
	{
		Bank bank(10000);
		const AccountId account = bank.OpenAccount();

		REQUIRE(bank.GetAccountBalance(account) == 0);
	}

	SECTION("Пополнение счёта уменьшает наличные и увеличивает баланс счёта")
	{
		constexpr Money initial = 10000;
		Bank bank(initial);

		const AccountId account = bank.OpenAccount();
		bank.DepositMoney(account, 3000);

		REQUIRE(bank.GetAccountBalance(account) == 3000);
		REQUIRE(bank.GetCash() == 7000);
		REQUIRE(bank.GetCash() + bank.GetAccountBalance(account) == initial);
	}

	SECTION("Нельзя положить на счёт больше наличных, чем есть в обороте")
	{
		Bank bank(10000);
		const AccountId account = bank.OpenAccount();

		REQUIRE_THROWS_AS(bank.DepositMoney(account, 10001), BankOperationError);
		REQUIRE(bank.GetAccountBalance(account) == 0);
		REQUIRE(bank.GetCash() == 10000);
	}

	SECTION("Пополнение счёта отрицательной суммой выбрасывает std::out_of_range")
	{
		Bank bank(10000);
		const AccountId account = bank.OpenAccount();

		REQUIRE_THROWS_AS(bank.DepositMoney(account, -1), std::out_of_range);
	}

	SECTION("Перевод денег между счетами работает корректно")
	{
		Bank bank(10000);

		const AccountId account1 = bank.OpenAccount();
		const AccountId account2 = bank.OpenAccount();

		bank.DepositMoney(account1, 3000);
		bank.DepositMoney(account2, 3000);

		bank.SendMoney(account1, account2, 2000);

		REQUIRE(bank.GetAccountBalance(account1) == 1000);
		REQUIRE(bank.GetAccountBalance(account2) == 5000);
		REQUIRE(bank.GetCash() == 4000);
	}

	SECTION("Перевод с недостатком средств выбрасывает исключение")
	{
		Bank bank(10000);

		const AccountId account1 = bank.OpenAccount();
		const AccountId account2 = bank.OpenAccount();

		bank.DepositMoney(account1, 1000);

		REQUIRE_THROWS_AS(bank.SendMoney(account1, account2, 2000), BankOperationError);
		REQUIRE(bank.GetAccountBalance(account1) == 1000);
		REQUIRE(bank.GetAccountBalance(account2) == 0);
	}

	SECTION("TrySendMoney возвращает false при нехватке денег")
	{
		Bank bank(10000);

		const AccountId account1 = bank.OpenAccount();
		const AccountId account2 = bank.OpenAccount();

		bank.DepositMoney(account1, 1000);

		REQUIRE_FALSE(bank.TrySendMoney(account1, account2, 2000));
		REQUIRE(bank.GetAccountBalance(account1) == 1000);
		REQUIRE(bank.GetAccountBalance(account2) == 0);
	}

	SECTION("Перевод отрицательной суммы выбрасывает std::out_of_range")
	{
		Bank bank(10000);

		const AccountId account1 = bank.OpenAccount();
		const AccountId account2 = bank.OpenAccount();

		REQUIRE_THROWS_AS(bank.SendMoney(account1, account2, -100), std::out_of_range);
		REQUIRE_THROWS_AS(bank.TrySendMoney(account1, account2, -100), std::out_of_range);
	}

	SECTION("Снятие денег со счёта увеличивает наличные в обороте")
	{
		Bank bank(10000);

		const AccountId account = bank.OpenAccount();
		bank.DepositMoney(account, 3000);

		bank.WithdrawMoney(account, 1000);

		REQUIRE(bank.GetAccountBalance(account) == 2000);
		REQUIRE(bank.GetCash() == 8000);
	}

	SECTION("TryWithdrawMoney возвращает false при нехватке денег")
	{
		Bank bank(10000);

		const AccountId account = bank.OpenAccount();
		bank.DepositMoney(account, 1000);

		REQUIRE_FALSE(bank.TryWithdrawMoney(account, 2000));
		REQUIRE(bank.GetAccountBalance(account) == 1000);
		REQUIRE(bank.GetCash() == 9000);
	}

	SECTION("Снятие отрицательной суммы выбрасывает std::out_of_range")
	{
		Bank bank(10000);
		const AccountId account = bank.OpenAccount();

		REQUIRE_THROWS_AS(bank.WithdrawMoney(account, -1), std::out_of_range);
		REQUIRE_THROWS_AS(bank.TryWithdrawMoney(account, -1), std::out_of_range);
	}

	SECTION("Закрытие счёта возвращает деньги в наличный оборот")
	{
		Bank bank(10000);

		const AccountId account = bank.OpenAccount();
		bank.DepositMoney(account, 3000);

		REQUIRE(bank.CloseAccount(account) == 3000);
		REQUIRE(bank.GetCash() == 10000);
		REQUIRE_THROWS_AS(bank.GetAccountBalance(account), BankOperationError);
	}
}

TEST_CASE("Акторы")
{
	SECTION("Один шаг всех персонажей сохраняет корректную логику операций")
	{
		constexpr Money initial = 12000;
		Bank bank(initial);

		Homer homer(2000, bank);
		Marge marge(2000, bank);
		Bart bart(500);
		Lisa lisa(500);
		Apu apu(2000, bank);
		MrBurns burns(5000, bank);

		Actors actors;
		actors.homer = &homer;
		actors.marge = &marge;
		actors.bart = &bart;
		actors.lisa = &lisa;
		actors.apu = &apu;
		actors.burns = &burns;

		homer.DepositMoney(1000);
		marge.DepositMoney(1000);
		apu.DepositMoney(1000);
		burns.DepositMoney(3000);

		homer.Act(actors);
		REQUIRE(bank.GetAccountBalance(marge.GetAccountId()) == 1020);
		REQUIRE(bank.GetAccountBalance(burns.GetAccountId()) == 3015);
		REQUIRE(bart.GetCash() == 505);
		REQUIRE(lisa.GetCash() == 505);
		REQUIRE(bank.GetAccountBalance(homer.GetAccountId()) == 955);

		marge.Act(actors);
		REQUIRE(bank.GetAccountBalance(marge.GetAccountId()) == 1002);
		REQUIRE(bank.GetAccountBalance(apu.GetAccountId()) == 1018);

		bart.Act(actors);
		REQUIRE(apu.GetCash() == 1003);
		REQUIRE(bart.GetCash() == 502);

		lisa.Act(actors);
		REQUIRE(apu.GetCash() == 1005);
		REQUIRE(lisa.GetCash() == 503);

		apu.Act(actors);
		REQUIRE(apu.GetCash() == 985);
		REQUIRE(bank.GetAccountBalance(apu.GetAccountId()) == 1028);
		REQUIRE(bank.GetAccountBalance(burns.GetAccountId()) == 3025);

		burns.Act(actors);
		REQUIRE(bank.GetAccountBalance(burns.GetAccountId()) == 3000);
		REQUIRE(bank.GetAccountBalance(homer.GetAccountId()) == 980);
	}

	SECTION("Действия акторов не нарушают сохранение денег")
	{
		constexpr Money initial = 20000;
		Bank bank(initial);

		Homer homer(3000, bank);
		Marge marge(3000, bank);
		Bart bart(500);
		Lisa lisa(500);
		Apu apu(3000, bank);
		MrBurns burns(10000, bank);

		Actors actors;
		actors.homer = &homer;
		actors.marge = &marge;
		actors.bart = &bart;
		actors.lisa = &lisa;
		actors.apu = &apu;
		actors.burns = &burns;

		homer.DepositMoney(1500);
		marge.DepositMoney(1500);
		apu.DepositMoney(1500);
		burns.DepositMoney(5000);

		homer.Act(actors);
		marge.Act(actors);
		bart.Act(actors);
		lisa.Act(actors);
		apu.Act(actors);
		burns.Act(actors);

		const Money totalCash =
			homer.GetCash() +
			marge.GetCash() +
			bart.GetCash() +
			lisa.GetCash() +
			apu.GetCash() +
			burns.GetCash();

		const Money totalBalances =
			homer.GetAccountBalance() +
			marge.GetAccountBalance() +
			apu.GetAccountBalance() +
			burns.GetAccountBalance();

		REQUIRE(totalCash == bank.GetCash());
		REQUIRE(totalCash + totalBalances == initial);
	}
}

TEST_CASE("Симуляция")
{
	SECTION("Короткая симуляция выполняется без нарушения инвариантов")
	{
		Simulation simulation;
		REQUIRE_NOTHROW(simulation.Run(10));
		REQUIRE(simulation.IsValid());
	}

	SECTION("Длинная симуляция выполняется без нарушения инвариантов")
	{
		Simulation simulation;
		REQUIRE_NOTHROW(simulation.Run(1000));
		REQUIRE(simulation.IsValid());
	}
}