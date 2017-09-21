#include "LogManager.h"

// Init the single instance as nullptr so functions can continue
LogManager* LogManager::theInstance = nullptr;

LogManager::LogManager() {
	outStream = nullptr;
	defaultLogFileName = "logfile.log";
	currentSeverity = LOG_ERROR;
	currentTarget = LOG_NONE;
}

LogManager::~LogManager() {
	close();
}


void LogManager::nameLogFile(std::string &fileName) {
	close();
	outStream = new std::ofstream(fileName.c_str());
}

LogManager& LogManager::getInstance() {
	if (theInstance == nullptr) {
		theInstance = new LogManager();
	}
	return *theInstance;
}

void LogManager::log(LogType level, std::string msg) {
	if (level <= currentSeverity && currentSeverity > LOG_NONE && currentTarget == LOG_NONE) {
		if (outStream == nullptr) {
			nameLogFile(defaultLogFileName);
		}
		(*outStream) << msg << "\n";
		outStream->flush();
	}
	else if (currentTarget != LOG_NONE && currentTarget == level) {
		if (outStream == nullptr) {
			nameLogFile(defaultLogFileName);
		}
		(*outStream) << msg << "\n";
		outStream->flush();
	}
}

void LogManager::close() {
	if (outStream != nullptr) {
		outStream->close();
		delete outStream;
		outStream = nullptr;
	}
}