#ifndef _READ_H
#define _READ_H

Image readImage(int imageNumber);
int readJPG(char* filename, Image* image, struct jpeg_error_mgr* jerr);
void printPixels(Image image);

#endif