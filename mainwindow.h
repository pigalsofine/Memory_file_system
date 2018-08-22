#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "ram.h"
#include <QPushButton>
#include <mybutton.h>
#include <QMouseEvent>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    User* _user;
    MyButton* _button[100];
    FCB* _cutFat;
    QFile* _f;
    int _button_num;
    int flag;
    int _fileflag;//1：删除，2：改名,3:剪切
    int _cutflag;
    QVector<QStringList> vec_str;//用来保存从磁盘读取的内容
    void save(FCB* father_fcb);
    void read(int father_number,FCB* father_fcb);
private:
    Ui::MainWindow *ui;

protected:

public slots:
    void mkdirSlot1();
    void mkdirSlot2();
    void testSlot();
    void returnLastSlot();
    void createSlot1();
    void createSlot2();
    void deleteSlot();
    void filePressSlot();
    void changeNameSlot();//改名字
    void cutSlot();//剪切
    void copySlot(); //复制
    void exitSlot();//保存并退出


    void cd();
};

#endif // MAINWINDOW_H
