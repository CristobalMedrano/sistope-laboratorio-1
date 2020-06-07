#ifndef _CLASSIFY_H
#define _CLASSIFY_H

int classifyImage(Image image, int classificationThreshold);
void showImageResultTitle(int numberImages, int flagShowResults);
void showImageResultBody(int numberImages, int imageNumber, int flagShowResults, int isNearlyBlack);

#endif