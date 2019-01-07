#include "Settings.h"

Settings* Settings::theInstance = nullptr;

Settings::Settings() {
	videoSettingList.push_back(Video(FULLSCREEN, false));
	videoSettingList.push_back(Video(MSAA, true));
	videoSettingList.push_back(Video(EXPOSURE, false));
	videoSettingList.push_back(Video(BLOOM, false));
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
