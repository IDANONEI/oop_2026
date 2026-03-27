#include <istream>
#include <ostream>
#include <functional>
#include <map>
#include <string>

class Car;

class CarController
{
public:
	CarController(Car& car, std::istream& input, std::ostream& output);
	bool HandleCommand();

private:
	bool Info(std::istream& args);
	bool EngineOn(std::istream& args);
	bool EngineOff(std::istream& args);
	bool SetGear(std::istream& args);
	bool SetSpeed(std::istream& args);
	void Help();

	using Handler = std::function<bool(std::istream&)>;
	using ActionMap = std::map<std::string, Handler>;

	Car& m_car;
	std::istream& m_input;
	std::ostream& m_output;
	const ActionMap m_actionMap;
};