#include "BMPLoader.hpp"

BMPLoader::~BMPLoader() {
    delete [] pixels;
}

void BMPLoader::BMPRead(const char *filename) {
    FILE *file;
    BITMAPFILEHEADER bfh;
    BITMAPINFOHEADER bih;
    unsigned int i;
    unsigned char r, g, b, a;

    free(this->pixels);
    this->pixels = nullptr;

    /* Otvaranje fajla koji sadrzi sliku u binarnom rezimu. */
    if((file = fopen(filename, "rb")) == nullptr) {
        fprintf(stderr, "Nepostojeci fajl\n");
        exit(1);
    }

    /* Ocitavanje podataka prvog zaglavlja. */
    fread(&bfh.type, 2, 1, file);
    fread(&bfh.size, 4, 1, file);
    fread(&bfh.reserved1, 2, 1, file);
    fread(&bfh.reserved2, 2, 1, file);
    fread(&bfh.offsetbits, 4, 1, file);

    /* Ocitavanje podataka drugog zaglavlja. */
    fread(&bih.size, 4, 1, file);
    fread(&bih.width, 4, 1, file);
    fread(&bih.height, 4, 1, file);
    fread(&bih.planes, 2, 1, file);
    fread(&bih.bitcount, 2, 1, file);
    fread(&bih.compression, 4, 1, file);
    fread(&bih.sizeimage, 4, 1, file);
    fread(&bih.xpelspermeter, 4, 1, file);
    fread(&bih.ypelspermeter, 4, 1, file);
    fread(&bih.colorsused, 4, 1, file);
    fread(&bih.colorsimportant, 4, 1, file);

    /*
     * Iz drugog zaglavlja koriste se samo informacije o dimenzijama slike.
     */
    this->width = bih.width;
    this->height = bih.height;

    /*
     * Alociranje niza odgovarajuce duzinu u zavisnosti od toga da li se
     * koristi RGB ili RGBA.
     */
    if (bih.bitcount == 24)
        this->pixels = new char[3 * bih.width * bih.height * sizeof(char)];
    else if (bih.bitcount == 32)
        this->pixels = new char[4 * bih.width * bih.height * sizeof(char)];
    else {
        fprintf(stderr, "image_read(): Podrzane su samo slike koje po pikselu cuvaju 24 ili 32 bita podataka.\n");
        exit(1);
    }
    assert(this->pixels != nullptr);

    /* Ucitavaju se podaci o pikselima i smestaju u alocirani niz. */
    if (bih.bitcount == 24)
        /*
         * U slucaju kada je RGB, 24 bita = 3 bajta, 1 po komponenti
         */
        for (i = 0; i < bih.width * bih.height; i++) {
            fread(&b, sizeof(char), 1, file);
            fread(&g, sizeof(char), 1, file);
            fread(&r, sizeof(char), 1, file);

            this->pixels[3 * i] = r;
            this->pixels[3 * i + 1] = g;
            this->pixels[3 * i + 2] = b;
        }
    else if (bih.bitcount == 32)
        /*
         * RGBA, tj. 32 bita = 4 bajta, 1 po komponenti
         */
        for (i = 0; i < bih.width * bih.height; i++) {
            fread(&b, sizeof(char), 1, file);
            fread(&g, sizeof(char), 1, file);
            fread(&r, sizeof(char), 1, file);
            fread(&a, sizeof(char), 1, file);

            this->pixels[4 * i] = r;
            this->pixels[4 * i + 1] = g;
            this->pixels[4 * i + 2] = b;
            this->pixels[4 * i + 3] = a;
        }

    fclose(file);
}

BMPLoader::BMPLoader(int width, int height) {
    /* U slucaju greske prekida se program. */
    assert(width >= 0 && height >= 0);

    this->width = width;
    this->height = height;

    if(width == 0 || height == 0)
        this->pixels = nullptr;
    else {
        this->pixels = new char[3 * width * height * sizeof(char)];
    }
}
