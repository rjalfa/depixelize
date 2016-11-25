<div align="center">
  <img src="http://i64.tinypic.com/f22beq.jpg"><br><br>
</div>

# Depixelize PixelArt
Depixelize lets you to create beautiful looking anti aliased images using the 1990s pixel art. 
An implementation of the algorithm stated in '[Depixeizing Pixel Art](http://johanneskopf.de/publications/pixelart/)' by Johannes Kopf and Dani Lischinski published in [SIGGRAPH 2011](http://www.siggraph.org/s2011/).

## Getting Started
The algorithm is divided into following parts - 
* Input the image
* Initialize the similarity graph on the basis of pixel colors
* Resolve crossing edges using different heuristics
* Create voronoi regions using the similarity graph
* Extract B-spline curves and optimize

### Prerequisites  
* g++ 
* [OpenGL](https://www.opengl.org/)
* [GLEW: The OpenGL Extension Wrangler Library](glew.sourceforge.net/)
* [GLUT - The OpenGL Utility Toolkit](https://www.opengl.org/resources/libraries/glut/)
* make

### Try (it's fun) 
```shell
$ sudo make
$ ./build/depixelize ./test/dolphin.bmp
```
### Acknowledgments
* [Depixelizing Pixel Art](http://johanneskopf.de/publications/pixelart/) by Johannes Kopf and Dani Lischinski]
* [YUV/RGB Conversion formulas](http://www.pcmag.com/encyclopedia/term/55166/yuv-rgb-conversion-formulas)
* [Loading bmp file in C++/Opengl](http://stackoverflow.com/questions/20595340/loading-a-tga-bmp-file-in-c-opengl)
