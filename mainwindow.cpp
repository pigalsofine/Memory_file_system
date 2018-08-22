#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ram.h"
#include <QVector>
#include <QDebug>
#include <QFile>
#include <QStringList>
#include <QPalette>
#define color1 "background-color: rgb(175,238,238)"
#define color2 "background-color: rgb(175,238,0)"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    _f = new QFile("test.txt");
    if(!_f->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug()<<"没有该文件";
    }

    QTextStream txtInput(_f);
    txtInput.setCodec("GBK");
    QString lineStr;

    int k=0;
    while(!txtInput.atEnd())
    {
        lineStr = txtInput.readLine();
        vec_str.push_back(lineStr.split(","));
    }
    for(int i=0;i<vec_str.size();i++){
        for(int j=0;j<vec_str.at(i).size();j++){
            qDebug()<<vec_str.at(i).at(j);
        }
    }
    _f->close();



    _user = new User();
    QFile file[100];
    read(2,_user->ItFCB);
    _button_num = 0;
    _fileflag = 0;
    _cutflag = 0;
    _cutFat = NULL;
    ui->label->hide();
    ui->lineEdit->hide();
    ui->pushButton->hide();
    ui->pushButton1->hide();
    ui->label_5->hide();
    flag=1;
    ui->label_2->setText("路径: root");

    QVector<FCB*> vec_fcb = _user->cd(_user->ItFCB);
    for(int i=0;i<vec_fcb.size();i++){
        _button[vec_fcb.at(i)->block_num] = new  MyButton(this,vec_fcb.at(i));
        _button[vec_fcb.at(i)->block_num]->setText(vec_fcb.at(i)->name);
        _button[vec_fcb.at(i)->block_num]->setGeometry(10,40+_button_num*21,200,20);
        _button_num++;
        _button[vec_fcb.at(i)->block_num]->show();
        if(vec_fcb.at(i)->typefile==1){
           _button[vec_fcb.at(i)->block_num]->setStyleSheet(color1);
            connect(_button[vec_fcb.at(i)->block_num],SIGNAL(clicked(bool)),this,SLOT(cd()));
        }
        if(vec_fcb.at(i)->typefile==2){
            _button[vec_fcb.at(i)->block_num]->setStyleSheet(color2);
            connect(_button[vec_fcb.at(i)->block_num],SIGNAL(clicked(bool)),this,SLOT(filePressSlot()));
        }//如果是文件
    }

//    _user->mkdir(_user->ItFCB,"1");
//    _user->mkdir(_user->ItFCB,"2");
//    QVector<FCB*> vec_fcb=_user->dir(_user->ItFCB);
//    _user->create(vec_fcb.at(0),"4");
//    _user->create(vec_fcb.at(0),"5");
//    FCB* fcb = vec_fcb.at(0);
//    vec_fcb=_user->dir(vec_fcb.at(0));
//  //  user->rename(vec_fcb.at(1),"wanghaidashen");
//    for(int i=0;i<vec_fcb.size();i++){
//        qDebug()<<vec_fcb.at(i)->name;
//    }
//    FCB* cp_fcb = _user->cut(fcb,"4");
//    vec_fcb=_user->dir(fcb);
//    for(int i=0;i<vec_fcb.size();i++){
//        qDebug()<<vec_fcb.at(i)->name;
//    }
//    _user->copy(_user->ItFCB,cp_fcb);
//    vec_fcb=_user->dir(_user->ItFCB);
//    for(int i=0;i<vec_fcb.size();i++){
//        qDebug()<<vec_fcb.at(i)->name;
//    }
    ui->last_block->setText(QString::number(_user->left_Block(),10));
    connect(ui->new_directoiy,SIGNAL(clicked(bool)),this,SLOT(mkdirSlot1()));
    connect(ui->pushButton,SIGNAL(clicked(bool)),this,SLOT(mkdirSlot2()));
    connect(ui->new_file,SIGNAL(clicked(bool)),this,SLOT(createSlot1()));
    connect(ui->pushButton1,SIGNAL(clicked(bool)),this,SLOT(createSlot2()));
    connect(ui->return_last,SIGNAL(clicked(bool)),this,SLOT(returnLastSlot()));
    connect(ui->deleteButton,SIGNAL(clicked(bool)),this,SLOT(deleteSlot()));
    connect(ui->change_name,SIGNAL(clicked(bool)),this,SLOT(changeNameSlot()));
    connect(ui->cut,SIGNAL(clicked(bool)),this,SLOT(cutSlot()));
    connect(ui->copy,SIGNAL(clicked(bool)),this,SLOT(copySlot()));
    connect(ui->exit,SIGNAL(clicked(bool)),this,SLOT(exitSlot()));
}

void MainWindow::read(int father_number,FCB* father_fcb)
{
    for(int i=0;i<vec_str.size();i++){
        QStringList tmp_str_list = vec_str.at(i);
        if((tmp_str_list.at(3)).toInt()==father_number){
            FCB* tmp_fcb;
            if((tmp_str_list.at(1)).toInt()==1){
                tmp_fcb = _user->mkdir(father_fcb,tmp_str_list.at(0));
            }else if((tmp_str_list.at(1)).toInt()==2){
                tmp_fcb = _user->create(father_fcb,tmp_str_list.at(0));
            }
            read((tmp_str_list.at(2)).toInt(),tmp_fcb);
        }
    }
}

//新建一个目录
void MainWindow::mkdirSlot1(){
    ui->label->show();
    ui->lineEdit->show();
    ui->pushButton->show();
}

void MainWindow::mkdirSlot2(){
    QString string = "文件夹 "+ui->lineEdit->text();
    qDebug()<<string;
    FCB* fcb = _user->mkdir(_user->current_FCB,string);
    if(fcb==NULL){
        ui->label->hide();
        ui->label_5->show();
        return;
    }
     _button[fcb->block_num] = new  MyButton(this,fcb);
     _button[fcb->block_num]->setText(string);
     _button[fcb->block_num]->setGeometry(10,40+_button_num*21,200,20);
     _button[fcb->block_num]->setStyleSheet(color1);

     connect(_button[fcb->block_num],SIGNAL(clicked(bool)),this,SLOT(cd()));
     _button_num++;
     _button[fcb->block_num]->show();
     ui->last_block->setText(QString::number(_user->left_Block(),10));

     ui->label->hide();
     ui->lineEdit->clear();
     ui->lineEdit->hide();
     ui->pushButton->hide();
     ui->label_5->hide();
}

//新建一个文件
void MainWindow::createSlot1(){
    ui->label->show();
    ui->lineEdit->show();
    ui->pushButton1->show();
}

void MainWindow::createSlot2(){
    QString string = "文件 "+ui->lineEdit->text();
    qDebug()<<string;
    FCB* fcb = _user->create(_user->current_FCB,string);
    if(fcb==NULL){
        ui->label->hide();
        ui->label_5->show();
        return;
    }
     _button[fcb->block_num] = new  MyButton(this,fcb);
     _button[fcb->block_num]->setText(string);
     _button[fcb->block_num]->setGeometry(10,40+_button_num*21,200,20);
     _button[fcb->block_num]->setStyleSheet(color2);
     connect(_button[fcb->block_num],SIGNAL(clicked(bool)),this,SLOT(filePressSlot()));
     _button_num++;
     _button[fcb->block_num]->show();
     ui->last_block->setText(QString::number(_user->left_Block(),10));

     ui->label->hide();
     ui->lineEdit->clear();
     ui->lineEdit->hide();
     ui->pushButton1->hide();
     ui->label_5->hide();
}

void MainWindow::testSlot(){
    MyButton* mybutton = (MyButton*)sender();
    qDebug()<<mybutton->fcb->name;
}

void MainWindow::cd(){


    if(_fileflag==1){
        MyButton* mybutton = (MyButton*)sender();
        FCB* tmp_fcb = mybutton->fcb->fa;
        qDebug()<<tmp_fcb->name;

        //先清空
        QVector<FCB*> vec_fcb = _user->cd(tmp_fcb);
        for(int i=0;i<vec_fcb.size();i++){
             _button[vec_fcb.at(i)->block_num]->deleteLater();
        }

            _user->del(_user->current_FCB,mybutton->fcb->name);
            _fileflag = 0;


       // 再绘制
        _button_num = 0;
        vec_fcb = _user->cd(tmp_fcb);
        for(int i=0;i<vec_fcb.size();i++){
            _button[vec_fcb.at(i)->block_num] = new  MyButton(this,vec_fcb.at(i));
            _button[vec_fcb.at(i)->block_num]->setText(vec_fcb.at(i)->name);
            _button[vec_fcb.at(i)->block_num]->setGeometry(10,40+_button_num*21,200,20);
            _button_num++;
            _button[vec_fcb.at(i)->block_num]->show();
            if(vec_fcb.at(i)->typefile==1){
               _button[vec_fcb.at(i)->block_num]->setStyleSheet(color1);
                connect(_button[vec_fcb.at(i)->block_num],SIGNAL(clicked(bool)),this,SLOT(cd()));
            }
            if(vec_fcb.at(i)->typefile==2){
                _button[vec_fcb.at(i)->block_num]->setStyleSheet(color2);
                connect(_button[vec_fcb.at(i)->block_num],SIGNAL(clicked(bool)),this,SLOT(filePressSlot()));
            }//如果是文件
            ui->last_block->setText(QString::number(_user->left_Block(),10));
        }
        qDebug()<<"in";
    }else if(_fileflag==2){
        MyButton* mybutton = (MyButton*)sender();
        FCB* tmp_fcb = mybutton->fcb->fa;
        //先清空
        QVector<FCB*> vec_fcb = _user->cd(tmp_fcb);
        for(int i=0;i<vec_fcb.size();i++){
             _button[vec_fcb.at(i)->block_num]->deleteLater();
        }
        QString tmp_str = "文件 "+ui->lineEdit->text();
        _user->rename(mybutton->fcb, tmp_str);
        _fileflag = 0;


       // 再绘制
        _button_num = 0;
        vec_fcb = _user->cd(tmp_fcb);
        for(int i=0;i<vec_fcb.size();i++){
            _button[vec_fcb.at(i)->block_num] = new  MyButton(this,vec_fcb.at(i));
            _button[vec_fcb.at(i)->block_num]->setText(vec_fcb.at(i)->name);
            _button[vec_fcb.at(i)->block_num]->setGeometry(10,40+_button_num*21,200,20);
            _button_num++;
            _button[vec_fcb.at(i)->block_num]->show();
            if(vec_fcb.at(i)->typefile==1){
               _button[vec_fcb.at(i)->block_num]->setStyleSheet(color1);
                connect(_button[vec_fcb.at(i)->block_num],SIGNAL(clicked(bool)),this,SLOT(cd()));
            }
            if(vec_fcb.at(i)->typefile==2){
                _button[vec_fcb.at(i)->block_num]->setStyleSheet(color2);
                connect(_button[vec_fcb.at(i)->block_num],SIGNAL(clicked(bool)),this,SLOT(filePressSlot()));
            }//如果是文件
        }
    }else{
        _button_num = 0;
        MyButton* mybutton = (MyButton*)sender();
        qDebug()<<mybutton->fcb->name<<" "<<mybutton->fcb->block_num;

        QVector<FCB*> vec_fcb = _user->cd(mybutton->fcb->fa);
        for(int i=0;i<vec_fcb.size();i++){
             _button[vec_fcb.at(i)->block_num]->deleteLater();
        }

        vec_fcb = _user->cd(mybutton->fcb);
        for(int i=0;i<vec_fcb.size();i++){
            _button[vec_fcb.at(i)->block_num] = new  MyButton(this,vec_fcb.at(i));
            _button[vec_fcb.at(i)->block_num]->setText(vec_fcb.at(i)->name);
            _button[vec_fcb.at(i)->block_num]->setGeometry(10,40+_button_num*21,200,20);
            _button_num++;
            _button[vec_fcb.at(i)->block_num]->show();
            if(vec_fcb.at(i)->typefile==1){
               _button[vec_fcb.at(i)->block_num]->setStyleSheet(color1);
                connect(_button[vec_fcb.at(i)->block_num],SIGNAL(clicked(bool)),this,SLOT(cd()));
            }
            if(vec_fcb.at(i)->typefile==2){
                _button[vec_fcb.at(i)->block_num]->setStyleSheet(color2);
                connect(_button[vec_fcb.at(i)->block_num],SIGNAL(clicked(bool)),this,SLOT(filePressSlot()));
            }//如果是文件
        }
        _user->current_FCB = mybutton->fcb;
        FCB* tmp_fcb = _user->current_FCB;
        QString str = "路径: ";
        QVector<QString> vec_str;
        while(tmp_fcb!=NULL){
            qDebug()<<tmp_fcb->name;
            vec_str.push_back(tmp_fcb->name);
            tmp_fcb = tmp_fcb->fa;
        }
        for(int i=vec_str.size()-1;i>=0;i--){
            str = str+vec_str[i]+"/";
        }
        ui->label_2->setText(str);
    }


}

void MainWindow::returnLastSlot()
{
    _button_num = 0;
    FCB* tmp_fcb = _user->current_FCB;


    if(tmp_fcb->fa){
        QVector<FCB*> vec_fcb = _user->cd(tmp_fcb);
        for(int i=0;i<vec_fcb.size();i++){
             _button[vec_fcb.at(i)->block_num]->deleteLater();
        }
        vec_fcb = _user->cd(tmp_fcb->fa);
        for(int i=0;i<vec_fcb.size();i++){
            _button[vec_fcb.at(i)->block_num] = new  MyButton(this,vec_fcb.at(i));
            _button[vec_fcb.at(i)->block_num]->setText(vec_fcb.at(i)->name);
            _button[vec_fcb.at(i)->block_num]->setGeometry(10,40+_button_num*21,200,20);
            _button_num++;

            _button[vec_fcb.at(i)->block_num]->show();
            if(vec_fcb.at(i)->typefile==1){
               _button[vec_fcb.at(i)->block_num]->setStyleSheet(color1);
                connect(_button[vec_fcb.at(i)->block_num],SIGNAL(clicked(bool)),this,SLOT(cd()));
            }
            if(vec_fcb.at(i)->typefile==2){
                _button[vec_fcb.at(i)->block_num]->setStyleSheet(color2);
                connect(_button[vec_fcb.at(i)->block_num],SIGNAL(clicked(bool)),this,SLOT(filePressSlot()));
            }//如果是文件
        }
        QString str = "路径: root/";
        QVector<QString> vec_str;
        tmp_fcb = tmp_fcb->fa;
        while(tmp_fcb!=NULL){
            qDebug()<<tmp_fcb->name;
            vec_str.push_back(tmp_fcb->name);
            tmp_fcb = tmp_fcb->fa;
        }
        for(int i=vec_str.size()-2;i>=0;i--){
            str = str+vec_str[i]+"/";
        }
        ui->label_2->setText(str);
   }

}

void MainWindow::deleteSlot()
{
    _fileflag = 1;
}

void MainWindow::filePressSlot()
{
    MyButton* mybutton = (MyButton*)sender();
    FCB* tmp_fcb = mybutton->fcb->fa;
    qDebug()<<tmp_fcb->name;
    if(_fileflag==1){

        //先清空
        QVector<FCB*> vec_fcb = _user->cd(tmp_fcb);
        for(int i=0;i<vec_fcb.size();i++){
             _button[vec_fcb.at(i)->block_num]->deleteLater();
        }

            _user->del(_user->current_FCB,mybutton->fcb->name);
            _fileflag = 0;


       // 再绘制
        _button_num = 0;
        vec_fcb = _user->cd(tmp_fcb);
        for(int i=0;i<vec_fcb.size();i++){
            _button[vec_fcb.at(i)->block_num] = new  MyButton(this,vec_fcb.at(i));
            _button[vec_fcb.at(i)->block_num]->setText(vec_fcb.at(i)->name);
            _button[vec_fcb.at(i)->block_num]->setGeometry(10,40+_button_num*21,200,20);
            _button_num++;
            _button[vec_fcb.at(i)->block_num]->show();
            if(vec_fcb.at(i)->typefile==1){
               _button[vec_fcb.at(i)->block_num]->setStyleSheet(color1);
                connect(_button[vec_fcb.at(i)->block_num],SIGNAL(clicked(bool)),this,SLOT(cd()));
            }
            if(vec_fcb.at(i)->typefile==2){
                _button[vec_fcb.at(i)->block_num]->setStyleSheet(color2);
                connect(_button[vec_fcb.at(i)->block_num],SIGNAL(clicked(bool)),this,SLOT(filePressSlot()));
            }//如果是文件
            ui->last_block->setText(QString::number(_user->left_Block(),10));
        }
        qDebug()<<"in";
    }else if(_fileflag==2){
        //先清空
        QVector<FCB*> vec_fcb = _user->cd(tmp_fcb);
        for(int i=0;i<vec_fcb.size();i++){
             _button[vec_fcb.at(i)->block_num]->deleteLater();
        }
        QString tmp_str = "文件 "+ui->lineEdit->text();
        _user->rename(mybutton->fcb, tmp_str);
        _fileflag = 0;


       // 再绘制
        _button_num = 0;
        vec_fcb = _user->cd(tmp_fcb);
        for(int i=0;i<vec_fcb.size();i++){
            _button[vec_fcb.at(i)->block_num] = new  MyButton(this,vec_fcb.at(i));
            _button[vec_fcb.at(i)->block_num]->setText(vec_fcb.at(i)->name);
            _button[vec_fcb.at(i)->block_num]->setGeometry(10,40+_button_num*21,200,20);
            _button_num++;
            _button[vec_fcb.at(i)->block_num]->show();
            if(vec_fcb.at(i)->typefile==1){
               _button[vec_fcb.at(i)->block_num]->setStyleSheet(color1);
                connect(_button[vec_fcb.at(i)->block_num],SIGNAL(clicked(bool)),this,SLOT(cd()));
            }
            if(vec_fcb.at(i)->typefile==2){
                _button[vec_fcb.at(i)->block_num]->setStyleSheet(color2);
                connect(_button[vec_fcb.at(i)->block_num],SIGNAL(clicked(bool)),this,SLOT(filePressSlot()));
            }//如果是文件
        }
    }else if(_fileflag==3){
        //先清空
        QVector<FCB*> vec_fcb = _user->cd(tmp_fcb);
        for(int i=0;i<vec_fcb.size();i++){
             _button[vec_fcb.at(i)->block_num]->deleteLater();
        }
        _cutFat = _user->cut(mybutton->fcb->fa,mybutton->fcb->name);
        _fileflag = 0;


       // 再绘制
        _button_num = 0;
        vec_fcb = _user->cd(tmp_fcb);
        for(int i=0;i<vec_fcb.size();i++){
            _button[vec_fcb.at(i)->block_num] = new  MyButton(this,vec_fcb.at(i));
            _button[vec_fcb.at(i)->block_num]->setText(vec_fcb.at(i)->name);
            _button[vec_fcb.at(i)->block_num]->setGeometry(10,40+_button_num*21,200,20);
            _button_num++;
            _button[vec_fcb.at(i)->block_num]->show();
            if(vec_fcb.at(i)->typefile==1){
               _button[vec_fcb.at(i)->block_num]->setStyleSheet(color1);
                connect(_button[vec_fcb.at(i)->block_num],SIGNAL(clicked(bool)),this,SLOT(cd()));
            }
            if(vec_fcb.at(i)->typefile==2){
                _button[vec_fcb.at(i)->block_num]->setStyleSheet(color2);
                connect(_button[vec_fcb.at(i)->block_num],SIGNAL(clicked(bool)),this,SLOT(filePressSlot()));
            }//如果是文件
        }
    }
    ui->label->hide();
    ui->lineEdit->hide();
    ui->lineEdit->hide();
}

void MainWindow::changeNameSlot()
{
    _fileflag = 2;
    ui->label->show();
    ui->lineEdit->clear();
    ui->lineEdit->show();
}

void MainWindow::cutSlot()
{
    _fileflag = 3;
}

void MainWindow::copySlot()
{
    if(_cutFat!=NULL){

        FCB* tmp_fcb = _user->current_FCB;
        //先清空
        QVector<FCB*> vec_fcb = _user->cd(tmp_fcb);
        for(int i=0;i<vec_fcb.size();i++){
             _button[vec_fcb.at(i)->block_num]->deleteLater();
        }

        _user->copy(_user->current_FCB,_cutFat);
       // 再绘制
        _button_num = 0;
        vec_fcb = _user->cd(tmp_fcb);
        for(int i=0;i<vec_fcb.size();i++){
            _button[vec_fcb.at(i)->block_num] = new  MyButton(this,vec_fcb.at(i));
            _button[vec_fcb.at(i)->block_num]->setText(vec_fcb.at(i)->name);
            _button[vec_fcb.at(i)->block_num]->setGeometry(10,40+_button_num*21,200,20);
            _button_num++;
            _button[vec_fcb.at(i)->block_num]->show();
            if(vec_fcb.at(i)->typefile==1){
               _button[vec_fcb.at(i)->block_num]->setStyleSheet(color1);
                connect(_button[vec_fcb.at(i)->block_num],SIGNAL(clicked(bool)),this,SLOT(cd()));
            }
            if(vec_fcb.at(i)->typefile==2){
                _button[vec_fcb.at(i)->block_num]->setStyleSheet(color2);
                connect(_button[vec_fcb.at(i)->block_num],SIGNAL(clicked(bool)),this,SLOT(filePressSlot()));
            }//如果是文件
        }
    }
}

void MainWindow::exitSlot()
{
    _f = new QFile("test.txt");
    if(!_f->open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return ;
    }
    this->save(_user->ItFCB);
    _f->close();

}

void MainWindow::save(FCB* father_fcb)
{
    if(father_fcb==NULL||father_fcb->deepQList==NULL)
        return;
    for(FCB* tmp = father_fcb->deepQList;tmp!=father_fcb;tmp=tmp->next){
        qDebug()<<tmp->name<<" "<<tmp->typefile;
        QTextStream txtOutput(_f);
        QString str = tmp->name+","+QString::number(tmp->typefile,10)+","+ QString::number(tmp->block_num, 10)+","+
                 QString::number(tmp->fa->block_num, 10);

        txtOutput << str << endl;
        save(tmp);

    }


}

MainWindow::~MainWindow()
{
    delete ui;
}
