#pragma once

#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <ostream>
#include <utility>
#include "common.h"

// Color structures
struct ColorYUV {
    double Y;
    double U;
    double V;
    bool is_similar(const ColorYUV&) const;
    bool is_similar_2(const ColorYUV& c, double softness = 20.0) const;
};
struct Color {
    unsigned int R;
    unsigned int G;
    unsigned int B;
    ColorYUV toYUV() const;
    operator ColorYUV() const { return this->toYUV(); };
    bool is_similar(const Color& c) const { return this->toYUV().is_similar((ColorYUV)c); };
    bool is_similar_2(const Color& c) const { return this->toYUV().is_similar_2((ColorYUV)c); };
    bool operator==(const Color& c) const { return this->is_similar(c); }
    bool operator<(const Color& c) const {
        return this->R < c.R || 
            (this->R == c.R && this->G < c.G) ||
            (this->R == c.R && this->G == c.G && this->B < c.B);
        };
};

class Pixel;
// Class Image: For handling Image loading and access to colors
class Image
{
	unsigned int width;
	unsigned int height;
	// Bag of pixels in a linearized matrix form
    std::vector<std::vector<Pixel>> pixels;
	
	public:
        //Parametric constructor, loads file image
        Image(const std::string& file);

        //Accessor to random pixel by index
        Pixel* operator()(unsigned int i, unsigned int j);

        //Accessors
        unsigned int getWidth() const {return this->width;}
        unsigned int getHeight() const {return this->height;}
        Pixel* getAdjacent(unsigned int i, unsigned int j, enum Direction dir);
        Pixel* getAdjacent(const Pixel* const p, enum Direction dir);
};

// Class pixel for metadata of one pixel
class Pixel
{
    //Image reference
    Image* image_ref;
    //Color of the pixel in RGB
    Color m_color;
    //Position of pixel in image
    std::pair<unsigned int, unsigned int> position;

public:
    //Parametric Constructor
    Pixel(Image* const image_ref, unsigned int R, unsigned int G, unsigned int B, int x, int y);
    //Default Constructors
    Pixel() : image_ref(nullptr) {};

    operator IntPoint() const { return IntPoint{ X(), Y() }; }

    Pixel* A(enum Direction dir) {
        if (!image_ref) return nullptr;
        return image_ref->getAdjacent(this, dir);
    }

    Point C() const { return Point{ X() + 0.5f, Y() + 0.5f }; }

    //Accessors
    unsigned int X() const noexcept { return this->position.first; }
    unsigned int Y() const noexcept { return this->position.second; }
    const Color& color() const noexcept { return this->m_color; };

    //for pretty printing pixels
    void print(std::ostream& out);
};

#endif