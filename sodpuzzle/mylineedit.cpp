#include "mylineedit.h"


MyLineEdit::MyLineEdit(int x, int y, QWidget *parent) :QLineEdit(parent)
{
    this->x= x;
    this->y= y;
}
