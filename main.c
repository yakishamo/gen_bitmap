#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define BI_RGB       0
#define BI_RLE8      1
#define BI_RLE4      2
#define BI_BITFIELDS 3
#define BI_JPEG      4
#define BI_PNG       5

struct BITMAPFILEHEADER {
    char bfType[2];
    uint32_t bfSize;
    uint16_t bfReserved1;
    uint16_t bfReserved2;
    uint32_t bfOffBits;
 } __attribute__((packed));

struct BITMAPINFOHEADER {
    uint32_t bcSize;
    int32_t bcWidth;
    int32_t bcHeight;
    uint16_t bcPlanes;
    uint16_t bcBitCount;
    uint32_t biCompression;
    uint32_t biSizeImage;
    uint32_t biXPixPerMeter;
    uint32_t biYPixPerMeter;
    uint32_t biClrUsed;
    uint32_t biCirImportant;
}__attribute__((packed));

void initFileHeader(struct BITMAPFILEHEADER *fh) {
    fh->bfType[0] = 'B';
    fh->bfType[1] = 'M';
    fh->bfReserved1 = 0;
    fh->bfReserved2 = 0;
    fh->bfOffBits = sizeof(struct BITMAPFILEHEADER) + sizeof(struct BITMAPINFOHEADER);
    return;
}

int main() {
    FILE *fp;
    struct BITMAPFILEHEADER fh;
    struct BITMAPINFOHEADER ih;
    uint32_t color = 0x000000ff;
    uint32_t bitmap[0x100][0x100];
    srand(time(NULL));

    for(int i = 0; i < 0x100; i++) {
        for(int j = 0; j < 0x100; j++)
        bitmap[i][j] = (rand()%0x100) + (rand()%0x100)*0x100 + (rand()%0x100)*0x10000 + (rand()%0x100)*0x1000000;
    }
    /*
    for(int i = 1; i < 0x100; i++) {
        memcpy(bitmap[i], bitmap[0], 0x100*sizeof(uint32_t));
    }
    */

    initFileHeader(&fh);
    fh.bfSize = fh.bfOffBits + sizeof(bitmap);

    ih.bcSize         = 40;
    ih.bcWidth        = 0x100;
    ih.bcHeight       = 0x100;
    ih.bcPlanes       = 1;
    ih.bcBitCount     = 32;
    ih.biCompression  = BI_RGB;
    ih.biSizeImage    = 0x100 * 0x100 * 4;
    ih.biXPixPerMeter = 0;
    ih.biYPixPerMeter = 0;
    ih.biClrUsed      = 0;
    ih.biCirImportant = 0;

    fp = fopen("bitmap.bmp", "wb+");
    if(fp == NULL) {
        fprintf(stderr, "fopen failed.\n");
        return -1;
    } else {
        fprintf(stderr, "opened bitmap.bmp\n");
    }
    fseek(fp, 0, SEEK_SET);
    fwrite(&fh, sizeof(fh), 1, fp);
    fwrite(&ih, sizeof(ih), 1, fp);
    fwrite(bitmap, sizeof(uint32_t), 0x100*0x100, fp);

    fclose(fp);

    return 0;
    
}