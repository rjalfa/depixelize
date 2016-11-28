#include "common.h"

void convertYUV(const Color& colors,double& y,double& u, double& v)
{
    y = 0.299*get<0>(colors) + 0.587*get<1>(colors) + 0.114*get<2>(colors);
    u = 0.492*(get<2>(colors) - y);
    v = 0.877*(get<0>(colors) - y);      
}


bool isSimilar(const Color& a,const Color& b)
{
    double y1,u1,v1;
    convertYUV(a,y1,u1,v1);
    double y2,u2,v2;
    convertYUV(b,y2,u2,v2);
    return (abs(y1-y2) < 48.0 && abs(u1-u2) < 7.0 && abs(v1-v2) < 6.0);
}