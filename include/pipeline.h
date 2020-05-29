#pragma once
#include <structs.h>

Image readImage(int imageNumber);
int readJPG(char* filename, Image* image, struct jpeg_error_mgr* jerr);
void initPipeline(int numberImages, int binarizationThreshold, int classificationThreshold, char* maskFilename, int flagShowResults);