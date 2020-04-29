#include "image.h"
#include "BMP.h"
#include "common.h"

#include <map>

bool ColorYUV::is_similar(const ColorYUV& c) const
{
    return is_similar_2(c, 0.0);
}
bool ColorYUV::is_similar_2(const ColorYUV& c, double softness) const
{
    return abs(Y - c.Y) < softness + 48.0 \
        && abs(U - c.U) < softness + 7.0 \
        && abs(V - c.V) < softness + 6.0;
}

// Converting to YUV as per Wikipedia definitions
ColorYUV Color::toYUV() const {
    double y = 0.299 * R + 0.587 * G + 0.114 * B;
    double u = 0.492 * (B - y);
    double v = 0.877 * (R - y);
    return ColorYUV{ y, u, v };
}

Pixel::Pixel(Image* const image_ref, unsigned int R, unsigned int G, unsigned int B, int x, int y)
{
    this->image_ref = image_ref;
    this->m_color = Color{ R, G, B };
    this->position = std::make_pair(x,y);
}

void Pixel::print(std::ostream& out)
{
    auto yuv = ColorYUV(m_color);
    out << "<<(" << position.first << "," << position.second << "):{"<< yuv.Y <<","<< yuv.U <<","<< yuv.V <<"}>>";
}

Image::Image(const std::string& file)
{
    auto raw_data = BMP(file.c_str());
    this->width = raw_data.bmp_info_header.width;
    this->height = raw_data.bmp_info_header.height;
    this->pixels.resize(this->width);
    int row = 0, col = 0;
    for (int row = 0; row < this->width; row++) {
        pixels[row].resize(this->height);
        for (int col = 0; col < this->height; col++)
        {
            uint8_t R, G, B, A;
            raw_data.get_pixel(row, this->height - 1 - col, B, G, R, A);
            pixels[row][col] = Pixel(this, R, G, B, row, col);
        }
    }
}

std::map<Direction, std::pair<int, int>> direction_deltas = {
    {TOP_LEFT, std::make_pair(-1, -1)},
    {TOP, std::make_pair(0, -1)},
    {TOP_RIGHT, std::make_pair(1, -1)},
    {BOTTOM_LEFT, std::make_pair(-1, 1)},
    {BOTTOM, std::make_pair(0, 1)},
    {BOTTOM_RIGHT, std::make_pair(1, 1)},
    {LEFT, std::make_pair(-1, 0)},
    {RIGHT, std::make_pair(1, 0)}
};

Pixel* Image::operator()(unsigned int i, unsigned int j)
{
    if(i >= 0 && i < this->width
        && j >= 0 && j < this->height) return &pixels[i][j];
    else return nullptr;
}

Pixel* Image::getAdjacent(unsigned int i, unsigned int j, enum Direction dir) {
    const std::pair<int, int>& deltas = direction_deltas[dir];
    unsigned int adjX = i + X(deltas);
    unsigned int adjY = j + Y(deltas);
    auto ret = this->operator()(adjX, adjY);
    return ret;
}

Pixel* Image::getAdjacent(const Pixel* const p, enum Direction dir) {
    if (p) return getAdjacent(p->X(), p->Y(), dir);
    return nullptr;
}