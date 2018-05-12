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
	delete theInstance;
	theInstance = nullptr;
}

ImageManager::HandleType ImageManager::StoreImage(std::string &name, const char* filePath) {
	Image *image = new Image(filePath);

	ImageManager::HandleType output(-1);
	output = images.Get(name);
	if (!output.IsNull()) {
		std::cerr << "ERROR : Image name: " << name << " is already in use." << std::endl;
		return output;
	}

	image->SetName(name);
	return images.Put(name, image);
}

Image* ImageManager::GetImage(ImageManager::HandleType &handle) {
	Image *output = images.Get(handle);
	if (output == nullptr) {
		std::cerr << "ERROR : Image Handle is not in use." << std::endl;
	}
	return output;
}

Image* ImageManager::GetImage(std::string &name) {
	ImageManager::HandleType handle = images.Get(name);
	if (handle.IsNull()) {
		std::cerr << "ERROR : Image name: " << name << " is not in use." << std::endl;
	}
	Image* output = images.Get(handle);
	return output;
}