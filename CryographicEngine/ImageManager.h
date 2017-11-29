#ifndef IMAGEMANAGER_H
#define IMAGEMANAGER_H

#include "ResourceManager.h"
#include "Image.h"

class ImageManager {
public:
	typedef ResourceHandle<Image> HandleType;
private:

	ResourceManager<Image> images;
	static ImageManager* theInstance;

	Image Manager() {};
public:

	static ImageManager* GetInstance();

	~ImageManager();

	ImageManager::HandleType StoreImage(std::string &name, const char* filePath);

	Image* GetImage(ImageManager::HandleType &handle);

	Image* GetImage(std::string &name);
};
#endif


