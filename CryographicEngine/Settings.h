#ifndef SETTINGS_H
#define SETTINGS_H

#include <vector>
#include <iostream>
#include <istream>
#include <sstream>
#include <fstream>

// The config file is located in the root folder of the solution and can be edited
// It will also save the last used settings for the next time
// The format for the settings file can be seen below

// Name = 1 or 0 (Enabled or Disabled)
// Ex: FullScreen = 0

// Any new settings added to the enum list and the videoSettingList vector on initialization 
// will automatically be saved to the config file

// These are the types of settings that can be enabled or disabled
// Their position in the list of settings is 0 indexed
// Currently only has true or false values
enum VideoSetting {
	FULLSCREEN, // 0
	MSAA, // 1
	EXPOSURE, // 2
	BLOOM, // 3
};

// Easy presets for the user to chose from
enum VideoPreset {
	LOW,
	MEDIUM,
	HIGH,
	ULTRA
};

class Settings {

private:

	struct Video {

		Video(VideoSetting _setting, std::string _name, bool _state) {
			setting = _setting;
			name = _name;
			state = _state;
			
		}

		VideoSetting setting;
		std::string name;
		bool state;
	};

	// Private constructor because the settings class is a singleton
	Settings();

	// Single instance of the Timer
	static Settings* theInstance;

	std::vector<Video> videoSettingList;
	VideoPreset videoPreset;

public:

	~Settings();

	// Method that gives access to the single instance of the settings class
	static Settings* GetInstance();

	void ToggleVideoSetting(VideoSetting _setting);
	void SetVideoSetting(VideoSetting _setting, bool _result);

	const bool GetVideoSettingState(VideoSetting _setting);

	VideoPreset GetVideoPreset();
	void SetVideoPreset(VideoPreset _preset);

	void ReadFile(); // This will read the file when the program starts and apply settings or fix errors 
	void CheckFile(); // This will read the file and only fix errors 
	void SaveToFile(); // Saves the current settings to the file
	void ResetDefaultSettings(); // Used to restore the file to default settings



};
#endif

