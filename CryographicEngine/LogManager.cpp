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
	Close();
	delete theInstance;
	theInstance = nullptr;
}


void LogManager::NameLogFile(std::string &fileName) {
	Close();
	outStream = new std::ofstream(fileName.c_str());
}

LogManager& LogManager::GetInstance() {
	if (theInstance == nullptr) {
		theInstance = new LogManager();
	}
	return *theInstance;
}

void LogManager::Log(LogType level, std::string msg) {
	if (level <= currentSeverity && currentSeverity > LOG_NONE && currentTarget == LOG_NONE) {
		if (outStream == nullptr) {
			NameLogFile(defaultLogFileName);
		}
		(*outStream) << msg << "\n";
		outStream->flush();
	}
	else if (currentTarget != LOG_NONE && currentTarget == level) {
		if (outStream == nullptr) {
			NameLogFile(defaultLogFileName);
		}
		(*outStream) << msg;
		outStream->flush();
	}
}

void LogManager::Close() {
	if (outStream != nullptr) {
		outStream->close();
		delete outStream;
		outStream = nullptr;
	}
}