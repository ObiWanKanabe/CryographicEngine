#include "ImageManager.h"

ImageManager* ImageManager::theInstance = nullptr;

ImageManager* ImageManager::GetInstance() {
	if (theInstance == nullptr) {
		theInstance = new ImageManager();
	}
	return theInstance;
}

ImageManager::~ImageManager() {
	images.clearKeysAndValues();
}

ImageManager::HandleType ImageManager::StoreImage(std::string &name, const char* filePath) {
	Image *image = new Image(filePath);
	image->SetName(name);
	return images.Put(name, image);
}

Image* ImageManager::GetImage(ImageManager::HandleType &handle) {
	Image *output = images.Get(handle);
	return output;
}

Image* ImageManager::GetImage(std::string &name) {
	Image *output = nullptr;
	ImageManager::HandleType handle = images.Get(name);
	output = images.Get(handle);
	return output;
}