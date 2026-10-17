#ifndef SCORE_H
#define SCORE_H

#include <string>

#include "GameObject.h"
#include "GameContext.h"
#include "MathUtil.h"

class UICounter : public CMPUT350::GameObject {
public:
    const char* t;
    int c;
    bool alive = true;
    CMPUT350::Point2D topLeft;

    bool IsAlive() const override;
    void Kill() override;

    UICounter(const char str[], int start, CMPUT350::Point2D pos);

    void RenderUI(CMPUT350::GameContext *context) override;
    
    int operator++();
};

#endif