#include "Car.h"
#include <stdexcept>

bool Car::IsValidGear(int gear)
{
	return gear >= -1 && gear <= 5;
}

Car::SpeedRange Car::GetSpeedRange(Gear gear)
{
	switch (gear)
	{
	case Gear::Reverse: return { 0, 20 };
	case Gear::First:   return { 0, 30 };
	case Gear::Second:  return { 20, 50 };
	case Gear::Third:   return { 30, 60 };
	case Gear::Fourth:  return { 40, 90 };
	case Gear::Fifth:   return { 50, 150 };
	default:            throw  std::logic_error("Invalid gear");
	}
}

bool Car::IsSpeedInRange(Gear gear, int speed)
{
	if (gear == Gear::Neutral)
	{
		return true;
	}

	SpeedRange range = GetSpeedRange(gear);
	return speed >= range.minSpeed && speed <= range.maxSpeed;
}


bool Car::IsTurnedOn() const
{
	return m_engineCondition == EngineState::On;
}

int Car::GetSpeed() const
{
	return m_speed;
}

Car::Gear Car::GetGear() const
{
	return m_gear;
}

Car::Direction Car::GetDirection() const
{
	if (m_speed == 0)
	{
		return Direction::StandingStill;
	}

	if (m_gear == Gear::Reverse)
	{
		return Direction::Backward;
	}

	if (m_gear != Gear::Neutral)
	{
		return Direction::Forward;
	}

	return m_lastMovementDirection;
}


void Car::TurnOnEngine()
{
	m_engineCondition = EngineState::On;
}

void Car::TurnOffEngine()
{
	if (m_speed != 0)
	{
		throw std::logic_error("You need to stop at 0");
	}

	if (m_gear != Gear::Neutral)
	{
		throw std::logic_error("You need to stop geer - Neutral");
	}
	m_engineCondition = EngineState::Off;
}

void Car::SetGear(int gear)
{
	if (!IsValidGear(gear))
	{
		throw std::logic_error("Invalid gear");
	}

	Gear newGear = static_cast<Gear>(gear);

	if (newGear == m_gear)
	{
		return;
	}

	if (m_engineCondition == EngineState::Off)
	{
		if (newGear == Gear::Neutral)
		{
			m_gear = newGear;
			return;
		}

		throw std::logic_error("When engine is off, only neutral gear is allowed");
	}

	if (newGear == Gear::Reverse && m_speed != 0)
	{
		throw std::logic_error("Reverse gear can be set only when speed is 0");
	}

	if (m_lastMovementDirection == Direction::Backward
		&& m_speed > 0
		&& newGear != Gear::Neutral
		&& newGear != Gear::Reverse)
	{
		throw std::logic_error("Cannot switch to forward gear while moving backward");
	}

	if (!IsSpeedInRange(newGear, m_speed))
	{
		throw std::logic_error("Current speed is out of range for gear");
	}

	m_gear = newGear;
}

void Car::SetSpeed(int speed)
{
	if (speed < 0)
	{
		throw std::logic_error("Speed cannot be negative");
	}

	if (m_engineCondition == EngineState::Off)
	{
		throw std::logic_error("Cannot change speed when engine is off");
	}

	if (m_gear == Gear::Neutral)
	{
		if (speed > m_speed)
		{
			throw std::logic_error("Cannot accelerate on neutral gear");
		}

		m_speed = speed;
		return;
	}

	if (!IsSpeedInRange(m_gear, speed))
	{
		throw std::logic_error("Speed is out of range for current gear");
	}

	m_speed = speed;

	if (m_speed > 0)
	{
		if (m_gear == Gear::Reverse)
		{
			m_lastMovementDirection = Direction::Backward;
		}
		else
		{
			m_lastMovementDirection = Direction::Forward;
		}
	}
}