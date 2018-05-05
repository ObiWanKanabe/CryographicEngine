#ifndef IMAGE_H
#define IMAGE_H

#include <SDL.h>
#include <SDL_image.h>
#include <glew.h>
#include <string>
#include <iostream>

enum MAP_TYPE {
	DEFAULT,
	DIFFUSE,
	SPECULAR,
	NORMAL,
	REFLECTION
};

class Image {
private:
	SDL_Surface* texture;
protected:
	std::string name;
	MAP_TYPE type;
public:
	Image(const char* filePath);
	Image(const char* filePath, MAP_TYPE _type);
	~Image();

	void SetName(std::string &_name);
	void SetMapType(MAP_TYPE _type);

	std::string GetName();
	MAP_TYPE GetMapType();
	int GetWidth();
	int GetHeight();
	Uint8 GetBitsPerPixel();
	GLenum GetFormat();
	GLenum GetPixelStorageType();
	int GetPixelStorageParameter();
	void* GetData();
};

#endif
