/*
 * Zaglavlje za ocitavanje slika sa ekstenzijom bmp.
 * */

#ifndef INC_3DMINESWEEPER_BMPLOADER_HPP
#define INC_3DMINESWEEPER_BMPLOADER_HPP

#include <cstdio>
#include <iostream>
#include <cassert>
#include "BMPLoader.hpp"

typedef struct {
    unsigned short type;
    unsigned int size;
    unsigned short reserved1;
    unsigned short reserved2;
    unsigned int offsetbits;
} BITMAPFILEHEADER;

typedef struct {
    unsigned int size;
    unsigned int width;
    unsigned int height;
    unsigned short planes;
    unsigned short bitcount;
    unsigned int compression;
    unsigned int sizeimage;
    int xpelspermeter;
    int ypelspermeter;
    unsigned int colorsused;
    unsigned int colorsimportant;
} BITMAPINFOHEADER;

class BMPLoader {
private:
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
public:
    char *pixels;
    int width, height;

    BMPLoader(int width, int height);

    ~BMPLoader();

    void BMPRead(const char *filename);
};


#endif //INC_3DMINESWEEPER_BMPLOADER_HPP
