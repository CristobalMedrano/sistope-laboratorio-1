#ifndef _PIPELINE_H
#define _PIPELINE_H

void initPipeline(int numberImages, int binarizationThreshold, int classificationThreshold, char* maskFilename, int flagShowResults);
Image readImage(int imageNumber);
int readJPG(char* filename, Image* image, struct jpeg_error_mgr* jerr);
void writeImage(Image image, int imageNumber);
int writeJPG(Image* image, int imageNumber, char* filename, struct jpeg_error_mgr* jerr);
Image convertGrayScale(Image image);
Image binarizeImage(Image image, int binarizationThreshold);

#endif