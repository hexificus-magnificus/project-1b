#ifndef SCORE_H
#define SCORE_H

#include <string>

#include "GameObject.h"
#include "GameContext.h"
#include "MathUtil.h"

class UICounter : public CMPUT350::GameObject {
public:
    char* t;
    int c;
    CMPUT350::Point2D topLeft;

    UICounter(char str[], int start, CMPUT350::Point2D pos);

    void RenderUI(CMPUT350::GameContext *context) override;
    
    int operator++();
};

#endif