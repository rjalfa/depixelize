#include "image.h"



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

void _pixel::print(ostream& out)
{
    out << "<<(" << position.first << "," << position.second << "):{"<<get<0>(colors)<<","<<get<1>(colors)<<","<<get<2>(colors)<<"}>>"; 
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
    return (abs((unsigned int)(get<0>(this->colors)-get<0>(a.colors))==0))&&(abs((unsigned int)(get<1>(this->colors)-get<1>(a.colors))==0))&&(abs((unsigned int)(get<2>(this->colors)-get<2>(a.colors))==0));
}