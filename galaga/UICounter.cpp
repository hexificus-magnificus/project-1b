#include "UICounter.h"

#include <format>
#include <cstdio>

UICounter::UICounter(const char* str, int start, CMPUT350::Point2D pos) : t(str), c(start), topLeft(pos) {}

void UICounter::RenderUI(CMPUT350::GameContext *context) {
    char str[100];
    std::sprintf(str, t, c);
    context->GUIContext->DrawText(str, 10, topLeft, CMPUT350::Colors::white);
}

int UICounter::operator++() {
    return ++c;
}