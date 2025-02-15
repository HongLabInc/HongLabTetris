#ifndef INTERACTIVEWIDGET_H
#define INTERACTIVEWIDGET_H

#include "Widget.h"

class InteractiveWidget: public Widget {
public:
    InteractiveWidget(InputManager* inputManager, ConsoleFrame* frame)
        : Widget(inputManager, frame)
    {}
    virtual ~InteractiveWidget() = default;
};

#endif