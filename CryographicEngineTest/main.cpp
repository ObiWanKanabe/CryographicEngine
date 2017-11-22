#include "..\CryographicEngine\LogManager.h"
#include <CryographicEngine\Timer.h>
#include <CryographicEngine\GameEngine.h>
#include <iostream>

int main(int argc, char *argv[]) {
	LogManager::GetInstance().SetTarget(LogManager::LogType::LOG_TRACE);
	//LogManager::getInstance().setLogFile(std::string("Ihavethebestlogs.log"));
	//LogManager::getInstance().setSeverity(LogManager::LogType::LOG_WARN);
	LogManager::GetInstance().Warn("BEEP BOOP WARNING");
	LogManager::GetInstance().Error("NO ERROR HERE HEHE");
	LogManager::GetInstance().Info("I HAVE NO INFORMATION TO GIVE");
	LogManager::GetInstance().Trace("THE CALVARY IS HERE");
	GameEngine::GetInstance().OnStart();
	return 0;
}