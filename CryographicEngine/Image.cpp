#include "Image.h"

Image::Image(const char* fileName) {
	texture = new SDL_Surface();
	texture = IMG_Load(fileName);
	if (texture == NULL) {
		std::cerr << "Unable to load: " << fileName << "\n";
	}
}

Image::~Image() {
	delete texture;
	texture = nullptr;
}

void Image::SetName(std::string &_name) {
	name = _name;
}

std::string Image::GetName() {
	return name;
}

int Image::GetWidth() {
	return texture->w;
}

int Image::GetHeight() {
	return texture->h;
}

Uint8 Image::GetBitsPerPixel() {
	return texture->format->BitsPerPixel;
}

GLenum Image::GetFormat() {

	if (texture->format->BytesPerPixel == 1) {
		return GL_RED;
	} 
	else if (texture->format->BytesPerPixel == 4 || texture->format->BytesPerPixel == 8) {
		return GL_RGBA;
	}
	else {
		return GL_RGB;
	}
}

GLenum Image::GetPixelStorageType() {
	return GL_PACK_ALIGNMENT;
};

int Image::GetPixelStorageParameter() {
	return 0;
};

void* Image::GetData() {
	return texture->pixels;
}