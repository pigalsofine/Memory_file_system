#ifndef MYBUTTON_H
#define MYBUTTON_H
#include <QPushButton>
#include <ram.h>
#include <QMainWindow>
class MyButton : public QPushButton
{
public:
    MyButton(QMainWindow* parent=0,FCB* fcb=0);
    MyButton(QMainWindow* parent=0);
    FCB* fcb;
};

#endif // MYBUTTON_H
