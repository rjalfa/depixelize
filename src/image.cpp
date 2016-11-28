#include "image.h"

//File loading 
BMP::BMP(const char* FilePath)
{
    std::fstream hFile(FilePath, std::ios::in | std::ios::binary);
    if (!hFile.is_open()) throw std::invalid_argument("Error: File Not Found.");

    hFile.seekg(0, std::ios::end);
    std::size_t Length = hFile.tellg();
    hFile.seekg(0, std::ios::beg);
    std::vector<std::uint8_t> FileInfo(Length);
    hFile.read(reinterpret_cast<char*>(FileInfo.data()), 54);

    if(FileInfo[0] != 'B' && FileInfo[1] != 'M')
    {
        hFile.close();
        throw std::invalid_argument("Error: Invalid File Format. Bitmap Required.");
    }

    if (FileInfo[28] != 24 && FileInfo[28] != 32)
    {
        hFile.close();
        throw std::invalid_argument("Error: Invalid File Format. 24 or 32 bit Image Required.");
    }

    BitsPerPixel = FileInfo[28];
    width = FileInfo[18] + (FileInfo[19] << 8);
    height = FileInfo[22] + (FileInfo[23] << 8);
    std::uint32_t PixelsOffset = FileInfo[10] + (FileInfo[11] << 8);
    std::uint32_t size = ((width * BitsPerPixel + 31) / 32) * 4 * height;
    Pixels.resize(size);

    hFile.seekg (PixelsOffset, std::ios::beg);
    hFile.read(reinterpret_cast<char*>(Pixels.data()), size);
    hFile.close();
}

_pixel::_pixel(Image* const image_ref, int R, int G, int B, int x, int y)
{
    this->image_ref = image_ref;
    this->colors = make_tuple(R,G,B);
    this->position = make_pair(x,y);
}

//http://www.pcmag.com/encyclopedia/term/55166/yuv-rgb-conversion-formulas
void _pixel::convertYUV(double& y,double& u, double& v)
{
    y = 0.299*get<0>(this->colors) + 0.587*get<1>(this->colors) + 0.114*get<2>(this->colors);
    u = 0.492*(get<2>(this->colors) - y);
    v = 0.877*(get<0>(this->colors) - y);      
}

void _pixel::print(ostream& out)
{
    double y1,u1,v1;
    this->convertYUV(y1,u1,v1);
    out << "<<(" << position.first << "," << position.second << "):{"<<y1<<","<<u1<<","<<v1<<"}>>"; 
}

Image::Image(string file)
{
    auto data = BMP(file.c_str());
    this->width = data.GetWidth();
    this->height = data.GetHeight();
    long off = 0;
    int row = 0, col=0;
    while(off < data.GetPixels().size())
    {
        pixels.push_back(_pixel(this,data.GetPixels()[off+2],data.GetPixels()[off+1],data.GetPixels()[off],row,col));
        off += 3;
        row = (row + 1) % width;
        if(row == 0) {off += 2;col = (col + 1)%height;}
    }
    assert(off== data.GetPixels().size());
}

_pixel* Image::operator()(unsigned int i, unsigned int j)
{
    if(i >= 0 && i < width && j >= 0 && j < height) return &pixels[i+ j*width];
    else return nullptr;
}

bool _pixel::isSimilar(_pixel& a)
{
    double y1,u1,v1;
    this->convertYUV(y1,u1,v1);
    double y2,u2,v2;
    a.convertYUV(y2,u2,v2);
    return (abs(y1-y2) < 48.0 && abs(u1-u2) < 7.0 && abs(v1-v2) < 6.0);
}

//Softer isSimilar
bool _pixel::isSimilar2(_pixel& a)
{
    double y1,u1,v1;
    this->convertYUV(y1,u1,v1);
    double y2,u2,v2;
    a.convertYUV(y2,u2,v2);
    return (abs(y1-y2) < 20+48.0 && abs(u1-u2) < 20+7.0 && abs(v1-v2) < 20+6.0);
}