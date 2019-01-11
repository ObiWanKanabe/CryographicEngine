#include "Settings.h"

Settings* Settings::theInstance = nullptr;

Settings::Settings() {
	Init();
}

void Settings::Init() {
	videoSettingList.push_back(Video(WIDTH, "Width", 1280));
	videoSettingList.push_back(Video(HEIGHT, "Height", 720));
	videoSettingList.push_back(Video(FULLSCREEN, "FullScreen", false));
	videoSettingList.push_back(Video(MSAA, "MSAA", true));
	videoSettingList.push_back(Video(MSAA_SAMPLES, "MSAA_Samples", 4));
	videoSettingList.push_back(Video(EXPOSURE, "Exposure", false));
	videoSettingList.push_back(Video(EXPOSURE_VALUE, "Exposure_Value", 1.5f));
	videoSettingList.push_back(Video(GAMMA, "Gamma", 1.1f));
	videoSettingList.push_back(Video(BLOOM, "Bloom", false));
	videoSettingList.push_back(Video(BLOOM_PASSES, "Bloom_Passes", 4));
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
	if (videoSettingList.at(_setting).value) {
		SetVideoSettingBool(_setting, false);
	}
	else if (!videoSettingList.at(_setting).value) {
		SetVideoSettingBool(_setting, true);
	}
}

void Settings::SetVideoSettingBool(VideoSetting _setting, bool _result) {
	videoSettingList.at(_setting).value = static_cast<float>(_result);
}

void Settings::SetVideoSettingInt(VideoSetting _setting, int _result) {
	videoSettingList.at(_setting).value = static_cast<float>(_result);
}

void Settings::SetVideoSettingFloat(VideoSetting _setting, float _result) {
	videoSettingList.at(_setting).value = _result;
}

const bool Settings::GetVideoSettingBool(VideoSetting _setting) {
	if (videoSettingList.at(_setting).value > 0.0f)
		return true;
	else
		return false;
}

const int Settings::GetVideoSettingInt(VideoSetting _setting) {
	return static_cast<int>(videoSettingList.at(_setting).value);
}

const float Settings::GetVideoSettingFloat(VideoSetting _setting) {
	return videoSettingList.at(_setting).value;
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
	float value; // The value of the setting currently being read

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
			} else if (setting != videoSettingList.at(i).name || evaluation != "=" || value < 0.0f || i >= videoSettingList.size()) {
				std::cout << "Incorrect formatting of the config file was found. Default settings set.\n";
				ResetDefaultSettings();
				configFile.close();
				return;
			}
			// Applying config files setting changes
			videoSettingList.at(i).value = value;
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
	float value; // The value of the setting currently being read

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
			else if (setting != videoSettingList.at(i).name || evaluation != "=" || value < 0.0f || i >= videoSettingList.size()) {
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
		configFile << videoSettingList.at(i).name << " = " << videoSettingList.at(i).value << '\n';
		i++;
	}
	return;
}

void Settings::ResetDefaultSettings() {
	std::ofstream configFile;

	configFile.open("../config.txt");
	
	// Clear the file and current settings
	configFile.clear();
	videoSettingList.clear();

	// Apply initial chosen settings
	Init();

	// Write to the file
	for (int i = 0; i < videoSettingList.size(); i++) {
		configFile << videoSettingList.at(i).name << " = " << videoSettingList.at(i).value << "\n";
	}

	configFile.close();
}
