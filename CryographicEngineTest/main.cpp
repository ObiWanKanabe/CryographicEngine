#include "..\CryographicEngine\LogManager.h"
#include "..\CryographicEngine\Timer.h"
#include "..\CryographicEngine\GameInterface.h"
#include "..\CryographicEngine\GameEngine.h"

#include <iostream>

int main(int argc, char *argv[]) {
	LogManager::getInstance().setTarget(LogManager::LogType::LOG_TRACE);
	//LogManager::getInstance().setLogFile(std::string("Ihavethebestlogs.log"));
	//LogManager::getInstance().setSeverity(LogManager::LogType::LOG_WARN);
	LogManager::getInstance().warn("BEEP BOOP WARNING");
	LogManager::getInstance().error("NO ERROR HERE HEHE");
	LogManager::getInstance().info("I HAVE NO INFORMATION TO GIVE");
	LogManager::getInstance().trace("THE CALVARY IS HERE");
	GameEngine::getInstance().onStart();
	while (true) {
		std::cout << Timer::getInstance().getSeconds() << std::endl;
	}
	return 0;

	GameEngine::getInstance().onStart();
}