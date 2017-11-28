#include "..\CryographicEngine\LogManager.h"
#include <CryographicEngine\Timer.h>
#include <CryographicEngine\GameEngine.h>
#include <iostream>

int main(int argc, char *argv[]) {
	LogManager::GetInstance().SetTarget(LogManager::LogType::LOG_ERROR);
	GameEngine::GetInstance().OnStart();
	return 0;
}