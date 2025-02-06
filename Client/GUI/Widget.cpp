#include "Widget.h"

Widget::Widget(){};
Widget::Widget(InputManager* im, ConsoleFrame* frame): width(0), height(0), posX(0), posY(0), mInputManager(im), mFrame(frame) {}

// 소멸자 구현
Widget::~Widget() {}

// resize 함수 구현
void Widget::resize(int width, int height) {
    this->width = width;
    this->height = height;
}

void Widget::SetPosition(int x, int y)
{
    posX = x;
    posY = y;
}
