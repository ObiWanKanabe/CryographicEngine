#ifndef IMAGE_H
#define IMAGE_H

#include <SDL.h>
#include <SDL_image.h>
#include <glew.h>
#include <string>
#include <iostream>

class Image {
private:
	SDL_Surface* texture;
protected:
	std::string name;
public:
	Image(const char* filePath);
	~Image();

	void SetName(std::string &_name);

	std::string GetName();
	int GetWidth();
	int GetHeight();
	Uint8 GetBitsPerPixel();
	GLenum GetFormat();
	GLenum GetPixelStorageType();
	int GetPixelStorageParameter();
	void* GetData();
};

#endif
