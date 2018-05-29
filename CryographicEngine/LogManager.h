#ifndef LOGMANAGER_H
#define LOGMANAGER_H

#include <fstream>
#include <iomanip>
#include <string>

// Taught in Game Engine Design Class @ Humber College

class LogManager {

public:
	// Enum list of Log types, in order from high to low severity
	enum LogType { LOG_NONE, LOG_ERROR, LOG_WARN, LOG_TRACE, LOG_INFO };
	std::string defaultLogFileName;

private:
	// Variables referencing the single instance of the log file, and the outstream to be printed to the file
	static LogManager *theInstance;
	std::ofstream *outStream;

	// Severy and target variables to filter or target desired log messages
	LogType currentSeverity;
	LogType currentTarget = LOG_NONE;

	// Private Constructor for the Logfile with the default name of "Logfile.log", and default severity of Error Logs
	LogManager();

public:

	// Deconstructor to call our own close method
	~LogManager();

	// Gives the Logfile your desired file name
	void NameLogFile(std::string &fileName);

	// The main method to get the single instance of the Log Manager, calls the constructor if not made before
	static LogManager& GetInstance();

	// The main method to log messages, given their level
	// If you decide to filter the messages by a certain level or higher, it looks at the currentSeverity and filters everything below, 
	// or if you decide to target a certain type of message, it looks at the currentTarget and ignores all other messages
	void Log(LogType level, std::string msg);

	// Logs a trace message
	void Trace(std::string msg) {
		Log(LOG_TRACE, msg);
	}

	// Logs an info message
	void Info(std::string msg) {
		Log(LOG_INFO, msg);
	}

	// Logs a warning message
	void Warn(std::string msg) {
		Log(LOG_WARN, msg);
	}

	// Logs an error message
	void Error(std::string msg) {
		Log(LOG_ERROR, msg);
	}

	// Sets the log filter to your desired severity level and clears your target
	void SetSeverity(LogType severity) {
		currentSeverity = severity;
		currentTarget = LOG_NONE;
	}

	// Sets the target to your desired log type and clears your severity level
	void SetTarget(LogType target) {
		currentTarget = target;
		currentSeverity = LOG_NONE;
	}

	LogType GetSeverity() {
		return currentSeverity;
	}

	LogType GetTarget() {
		return currentTarget;
	}

	void Close();
};
#endif