#pragma once

#ifndef IMAGE_H
#define IMAGE_H

#include "common.h"

class Image;

//Class for loading BMP images
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


//Class pixel for meta of One pixel
class _pixel
{
    //Image reference
	Image* image_ref;

    //Color of the pixel in RGB888
	Color colors;

    //Position of pixel in image
	pair<unsigned int,unsigned int> position;
	
    public:
        //Parametric Constructor
        _pixel(Image* const image_ref, int R, int G, int B, int x, int y);
        //Default Constructor
        _pixel() : image_ref(nullptr) {};

        //Accessors
        unsigned int getX() {return this->position.first;}
        unsigned int getY() {return this->position.second;}
        Color getColor() {return this->colors;};    
        
        //isSimilar for pixels, compares colors as in common.cpp
        bool isSimilar(_pixel& a);
        bool isSimilar2(_pixel& a);

        //for pretty printing pixels
        void print(ostream& out);

        //convertYUV for pixel
        void convertYUV(double& y,double& u, double& v);
};	

//Class Image: For handling Image loading and access to colors

class Image
{
	unsigned int width;
	unsigned int height;
	//Soup of pixels
    vector<_pixel> pixels;
	
	public:
        //Parametric constructor, loads file image
		Image(string file);

        //Accessor to random pixel by index
        _pixel* operator()(unsigned int i, unsigned int j);
        //Accessors
        unsigned int getWidth() const {return this->width;}
        unsigned int getHeight() const {return this->height;}
        vector<_pixel> getPixels() {return this->pixels;}
};

#endif