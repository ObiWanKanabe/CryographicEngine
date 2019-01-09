#include "Settings.h"

Settings* Settings::theInstance = nullptr;

Settings::Settings() {
	videoSettingList.push_back(Video(FULLSCREEN, "FullScreen", false));
	videoSettingList.push_back(Video(MSAA, "MSAA", true));
	videoSettingList.push_back(Video(EXPOSURE, "Exposure", false));
	videoSettingList.push_back(Video(BLOOM, "Bloom", false));
}


Settings::~Settings() {

}

Settings* Settings::GetInstance() {
	if (theInstance == nullptr) {
		theInstance = new Settings();
	}
	return theInstance;
}

void Settings::ToggleVideoSetting(VideoSetting _setting) {
	if (videoSettingList.at(_setting).state) {
		SetVideoSetting(_setting, false);
	}
	else if (!videoSettingList.at(_setting).state) {
		SetVideoSetting(_setting, true);
	}
}

void Settings::SetVideoSetting(VideoSetting _setting, bool _result) {
	videoSettingList.at(_setting).state = _result;
}

const bool Settings::GetVideoSettingState(VideoSetting _setting) {
	return videoSettingList.at(_setting).state;
}

VideoPreset Settings::GetVideoPreset() {
	return videoPreset;
}

void Settings::SetVideoPreset(VideoPreset _preset) {
	videoPreset = _preset;
}

void Settings::ReadFile() {
	std::ifstream configFile; // Variable to store the config file
	std::string setting; // The setting we're evaluating
	std::string evaluation; // The equal sign temporarily stored for ease of use
	int value; // The value of the setting currently being read

	configFile.open("../config.txt");

	// Making sure our file was opened correctly, if not just use default settings
	if (!configFile.is_open()) {
		std::cout << "Unable to read config file. A default file will been created.\n";
		return;
	}
	else {
		// Making sure the file isn't empty, writing default settings if it is
		if (configFile.peek() == std::ifstream::traits_type::eof()) {
			std::cout << "Config file was empty. Default settings set.\n";
			ResetDefaultSettings();
			configFile.close();
			return;
		}

		// Reading the file and saving the three strings into the seperate variables
		int i = 0;

		while (configFile >> setting >> evaluation >> value) {
			// Checking to see if all adjustable settings are properly formatted in the file
			// If there is any incorrect syntax, config file will be reset
			if (i >= videoSettingList.size()) {
				std::cout << "Incorrect formatting of the config file was found. Default settings set.\n";
				ResetDefaultSettings();
				configFile.close();
				return;
			} else if (setting != videoSettingList.at(i).name || evaluation != "=" || value < 0 || value > 1) {
				std::cout << "Incorrect formatting of the config file was found. Default settings set.\n";
				ResetDefaultSettings();
				configFile.close();
				return;
			}
			// Applying config files setting changes
			videoSettingList.at(i).state = value;	
			i++;
		}
		configFile.close();
		return;
	}
}

void Settings::CheckFile() {
	std::ifstream configFile; // Variable to store the config file and read from
	std::string setting; // The setting we're evaluating
	std::string evaluation; // The equal sign temporarily stored for ease of use
	int value; // The value of the setting currently being read

	configFile.open("../config.txt");

	// Making sure our file was opened correctly, if not just use default settings
	if (!configFile.is_open()) {
		std::cout << "Unable to read config file. A default file will been created.\n";
		return;
	}
	else {
		// Making sure the file isn't empty, writing default settings if it is
		if (configFile.peek() == std::ifstream::traits_type::eof()) {
			std::cout << "Config file was empty. Default settings set.\n";
			ResetDefaultSettings();
			configFile.close();
			return;
		}

		// Reading the file and saving the three strings into the seperate variables
		int i = 0;

		while (configFile >> setting >> evaluation >> value) {
			// Checking to see if all adjustable settings are properly formatted in the file
			// If there is any incorrect syntax, config file will be reset
			if (i >= videoSettingList.size()) {
				std::cout << "Incorrect formatting of the config file was found. Default settings set.\n";
				ResetDefaultSettings();
				configFile.close();
				return;
			}
			else if (setting != videoSettingList.at(i).name || evaluation != "=" || value < 0 || value > 1 || i >= videoSettingList.size()) {
				std::cout << "Incorrect formatting of the config file was found. Default settings set.\n";
				ResetDefaultSettings();
				configFile.close();
				return;
			}
			i++;
		}
		configFile.close();
		return;
	}
}

void Settings::SaveToFile() {
	std::ofstream configFile; // Variable to store the config file and write to

	configFile.open("../config.txt");
	
	// Clearing the config file and then saving all the current settings
	configFile.clear();
	int i = 0;

	while (i < videoSettingList.size()) {
		configFile << videoSettingList.at(i).name << " = " << videoSettingList.at(i).state << '\n';
		i++;
	}
	return;
}

void Settings::ResetDefaultSettings() {
	std::ofstream configFile;

	configFile.open("../config.txt");
	configFile.clear();

	for (int i = 0; i < videoSettingList.size(); i++) {
		configFile << videoSettingList.at(i).name << " = " << videoSettingList.at(i).state << "\n";
	}

	configFile.close();
}
