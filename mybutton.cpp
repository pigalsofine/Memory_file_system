#include "mybutton.h"

MyButton::MyButton(QMainWindow* parent,FCB* fcb):QPushButton(parent)
{
    this->fcb = fcb;
}

MyButton::MyButton(QMainWindow* parent):QPushButton(parent)
{

}

