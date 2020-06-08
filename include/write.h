#ifndef _WRITE_H
#define _WRITE_H

void writeImage(Image image, int imageNumber);
int writeJPG(Image* image, int imageNumber, char* filename, struct jpeg_error_mgr* jerr);

#endif