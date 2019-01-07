#ifndef SETTINGS_H
#define SETTINGS_H

#include <vector>
#include <iostream>

// These are the types of settings that can be enabled or disabled
// Their position in the list of settings is 0 indexed
enum VideoSetting {
	FULLSCREEN, // 0
	MSAA, // 1
	EXPOSURE, // 2
	BLOOM // 3
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

		Video(VideoSetting _setting, bool _state) {
			setting = _setting;
			state = _state;
		}

		VideoSetting setting;
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




};
#endif

