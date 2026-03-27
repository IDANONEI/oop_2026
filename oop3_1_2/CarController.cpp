#include "CarController.h"
#include "Car.h"

#include <sstream>
#include <string>
#include <functional>
#include <map>
#include <stdexcept>

CarController::CarController(Car& car, std::istream& input, std::ostream& output)
	: m_car(car)
	, m_input(input)
	, m_output(output)
	, m_actionMap({
		{ "Info",      std::bind(&CarController::Info, this, std::placeholders::_1) },
		{ "EngineOn",  std::bind(&CarController::EngineOn, this, std::placeholders::_1) },
		{ "EngineOff", std::bind(&CarController::EngineOff, this, std::placeholders::_1) },
		{ "SetGear",   std::bind(&CarController::SetGear, this, std::placeholders::_1) },
		{ "SetSpeed",  std::bind(&CarController::SetSpeed, this, std::placeholders::_1) }
	})
{
}

bool CarController::HandleCommand()
{
	std::string line;
	if (!std::getline(m_input, line))
	{
		return false;
	}

	std::istringstream stream(line);

	std::string command;
	stream >> command;

	auto it = m_actionMap.find(command);
	if (it == m_actionMap.end())
	{
		m_output << "Unknown command" << std::endl;
		return true;
	}

	try
	{
		return it->second(stream);
	}
	catch (const std::exception& e)
	{
		m_output << e.what() << std::endl;
		return true;
	}
}

bool CarController::Info(std::istream& args)
{
	(void)args;

	m_output << "Engine: " << (m_car.IsTurnedOn() ? "on" : "off") << std::endl;

	switch (m_car.GetDirection())
	{
	case Car::Direction::Forward:
		m_output << "Direction: forward" << std::endl;
		break;
	case Car::Direction::Backward:
		m_output << "Direction: backward" << std::endl;
		break;
	case Car::Direction::StandingStill:
		m_output << "Direction: standing still" << std::endl;
		break;
	}

	m_output << "Speed: " << m_car.GetSpeed() << std::endl;
	m_output << "Gear: " << static_cast<int>(m_car.GetGear()) << std::endl;

	return true;
}

void CarController::Help()
{
	m_output << "Available commands:\n"
			 << "  Info                - Show car state\n"
			 << "  EngineOn            - Turn on the engine\n"
			 << "  EngineOff           - Turn off the engine\n"
			 << "  SetGear <gear>      - Set gear (-1 to 5)\n"
			 << "  SetSpeed <speed>    - Set speed (>= 0)\n"
			 << "  Help                - Show this help\n";
}

bool CarController::EngineOn(std::istream& args)
{
	(void)args;
	m_car.TurnOnEngine();
	return true;
}

bool CarController::EngineOff(std::istream& args)
{
	(void)args;
	m_car.TurnOffEngine();
	return true;
}

bool CarController::SetGear(std::istream& args)
{
	int gear;
	if (!(args >> gear))
	{
		m_output << "Invalid command argument" << std::endl;
		return true;
	}

	m_car.SetGear(gear);
	return true;
}

bool CarController::SetSpeed(std::istream& args)
{
	int speed;
	if (!(args >> speed))
	{
		m_output << "Invalid command argument" << std::endl;
		return true;
	}

	m_car.SetSpeed(speed);
	return true;
}