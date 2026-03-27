#pragma once

class Car
{
public:
	enum class Direction
	{
		StandingStill,
		Forward,
		Backward,
	};

	enum class Gear
	{
		Reverse = -1,
		Neutral = 0,
		First = 1,
		Second = 2,
		Third = 3,
		Fourth = 4,
		Fifth = 5
	};

	Car() = default;

	bool IsTurnedOn() const;
	int GetSpeed() const;
	Gear GetGear() const;
	Direction GetDirection() const;

	void TurnOnEngine();
	void TurnOffEngine();
	void SetGear(int gear);
	void SetSpeed(int speed);

private:
	enum class EngineState
	{
		On,
		Off,
	};

	struct SpeedRange
	{
		int minSpeed;
		int maxSpeed;
	};

	static bool IsValidGear(int gear);
	static SpeedRange GetSpeedRange(Gear gear);
	static bool IsSpeedInRange(Gear gear, int speed);

	EngineState m_engineCondition = EngineState::Off;
	Gear m_gear = Gear::Neutral;
	int m_speed = 0;
	Direction m_lastMovementDirection = Direction::StandingStill;
};