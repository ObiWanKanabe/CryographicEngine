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

// Name = Value
// If the value is meant to be true or false, 0 is false, and everything else is true
// FLoat values where there is supposed to be an int will be cast as an int
// Ex: FullScreen = 0

// Any new settings added to the enum list and the videoSettingList vector on initialization 
// will automatically be saved to the config file

// These are the types of settings that can be enabled or disabled
// Their position in the list of settings is 0 indexed
// Currently only has true or false values
enum VideoSetting {
	WIDTH, // 0
	HEIGHT, // 1
	FULLSCREEN, // 2
	MSAA, // 3
	MSAA_SAMPLES, //4
	EXPOSURE, // 5
	EXPOSURE_VALUE, // 6
	GAMMA, // 7
	BLOOM, // 8
	BLOOM_PASSES // 9
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

		Video(VideoSetting _setting, std::string _name, float _value) {
			setting = _setting;
			name = _name;
			value = _value;
			
		}

		VideoSetting setting;
		std::string name;
		float value;
	};

	// Private constructor because the settings class is a singleton
	Settings();

	// Initializes the default settings
	void Init();

	// Single instance of the Timer
	static Settings* theInstance;

	std::vector<Video> videoSettingList;
	VideoPreset videoPreset;

public:

	~Settings();

	// Method that gives access to the single instance of the settings class
	static Settings* GetInstance();

	void ToggleVideoSetting(VideoSetting _setting);
	void SetVideoSettingBool(VideoSetting _setting, bool _result);
	void SetVideoSettingInt(VideoSetting _setting, int _result);
	void SetVideoSettingFloat(VideoSetting _settings, float _result);

	const bool GetVideoSettingBool(VideoSetting _setting);
	const int GetVideoSettingInt(VideoSetting _setting);
	const float GetVideoSettingFloat(VideoSetting _setting);

	VideoPreset GetVideoPreset();
	void SetVideoPreset(VideoPreset _preset);

	void ReadFile(); // This will read the file when the program starts and apply settings or fix errors 
	void CheckFile(); // This will read the file and only fix errors 
	void SaveToFile(); // Saves the current settings to the file
	void ResetDefaultSettings(); // Used to restore the file to default settings



};
#endif

