#pragma once

#ifndef IMAGE_H
#define IMAGE_H

#include "common.h"

class Image;

// Source: http://stackoverflow.com/questions/20595340/loading-a-tga-bmp-file-in-c-opengl
class BMP
{
private:
    std::uint32_t width, height;
    std::uint16_t BitsPerPixel;
    std::vector<std::uint8_t> Pixels;

public:
    BMP(const char* FilePath);
    std::vector<std::uint8_t> GetPixels() const {return this->Pixels;}
    std::uint32_t GetWidth() const {return this->width;}
    std::uint32_t GetHeight() const {return this->height;}
    bool HasAlphaChannel() {return BitsPerPixel == 32;}
};

class _pixel
{
	Image* image_ref;
	Color colors;
	pair<unsigned int,unsigned int> position;
	public:
        _pixel(Image* const image_ref, int R, int G, int B, int x, int y);
        _pixel() : image_ref(nullptr) {};
        unsigned int getX() {return this->position.first;}
        unsigned int getY() {return this->position.second;}
        Color getColor() {return this->colors;};    
        bool isSimilar(_pixel& a);
        void print(ostream& out);
        void convertYUV(double& y,double& u, double& v);
};	

class Image
{
	unsigned int width;
	unsigned int height;
	vector<_pixel> pixels;
	
	public:
		Image(string file);
        _pixel* operator()(unsigned int i, unsigned int j);
        unsigned int getWidth() const {return this->width;}
        unsigned int getHeight() const {return this->height;}
        vector<_pixel> getPixels() {return this->pixels;}
};

#endif