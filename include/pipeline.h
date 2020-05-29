#ifndef _PIPELINE_H
#define _PIPELINE_H

void initPipeline(int numberImages, int binarizationThreshold, int classificationThreshold, char* maskFilename, int flagShowResults);
Image readImage(int imageNumber);
int readJPG(char* filename, Image* image, struct jpeg_error_mgr* jerr);

#endif