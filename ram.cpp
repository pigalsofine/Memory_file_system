#include "ram.h"
#include <QDebug>
#include <QVector>
QVector<FCB*> User::dir(FCB* father_fcb){
    QVector<FCB*> vec_fcb;//存放当前目录下的所有目录
    for(FCB* tmp = father_fcb->deepQList;tmp!=father_fcb;tmp=tmp->next){
        vec_fcb.push_back(tmp);
    }
    return vec_fcb;
}

FCB* User::mkdir(FCB* father_fcb,QString name){
    if(father_fcb->typefile==0)
        return NULL;
    int tmp_i=0;
    if(father_fcb->deepQList){
        for(FCB* tmp = father_fcb->deepQList;tmp!=father_fcb;tmp=tmp->next){
            if(name==tmp->name)
                return NULL;
        }
    }
    for(int i=3;i<BlockCount;i++){//查找FAT表中第一个为空的编号，把他设置为表示文件
        if(this->ItDisk->FAT1[i]==0){
            this->ItDisk->FAT1[i] = 1;
            tmp_i = i;
            break;
        }
    }
    if(father_fcb->deepQList==NULL){
        father_fcb->deepQList = new FCB(name,1);
        father_fcb->deepQList->next = father_fcb;
        father_fcb->deepQList->pre = father_fcb;

    }else{
        FCB* tmp_fcb = new FCB(name,1);
        tmp_fcb->next = father_fcb->deepQList;
        tmp_fcb->pre = father_fcb->deepQList;
        father_fcb->deepQList->pre = tmp_fcb;
        father_fcb->deepQList = tmp_fcb;
    }
    father_fcb->deepQList->block_num = tmp_i;//新建的fcb的磁盘块号为i
    father_fcb->deepQList->fa = father_fcb;
    father_fcb->typefile==1;
    return father_fcb->deepQList;
}

FCB* User::create(FCB* father_fcb,QString name){
    if(father_fcb->typefile==0)
        return NULL;
    int tmp_i=0;
    if(father_fcb->deepQList){
        for(FCB* tmp = father_fcb->deepQList;tmp!=father_fcb;tmp=tmp->next){
            if(name==tmp->name)
                return NULL;
        }
    }

    for(int i=3;i<BlockCount;i++){//查找FAT表中第一个为空的编号，把他设置为表示文件
        if(this->ItDisk->FAT1[i]==0){
            this->ItDisk->FAT1[i] = 1;
            tmp_i = i;
            break;
        }
    }
    if(father_fcb->deepQList==NULL){
        father_fcb->deepQList = new FCB(name,2);
        father_fcb->deepQList->next = father_fcb;
        father_fcb->deepQList->pre = father_fcb;
    }else{
        FCB* tmp_fcb = new FCB(name,2);
        tmp_fcb->next = father_fcb->deepQList;
        tmp_fcb->pre = father_fcb;
        father_fcb->deepQList->pre = tmp_fcb;
        father_fcb->deepQList = tmp_fcb;

    }
    father_fcb->deepQList->block_num = tmp_i;
    father_fcb->deepQList->fa = father_fcb;
    return father_fcb->deepQList;
}

QString User::del(FCB *father_fcb, QString name){
    for(FCB* tmp = father_fcb->deepQList;tmp!=father_fcb;tmp=tmp->next){
        qDebug()<<"del in";
        if(tmp->name == name){
            if(tmp==father_fcb->deepQList){
                father_fcb->deepQList = tmp->next;
                tmp->next->pre = father_fcb;
            }else if(tmp->next==father_fcb){
                tmp->pre->next = father_fcb;

            }else{
                tmp->pre->next = tmp->next;
                tmp->next->pre = tmp->pre;
                qDebug()<<"del";
            }
            delete tmp;
            return "";
        }
    }return "删除失败";
}

QString User::rename(FCB *fcb,QString name){
    fcb->name = name;
    return "";
}

FCB* User::cut(FCB *father_fcb, QString name){
    for(FCB* tmp = father_fcb->deepQList;tmp!=father_fcb;tmp=tmp->next){
        if(tmp->name == name){
            if(tmp==father_fcb->deepQList){
                father_fcb->deepQList = tmp->next;
                tmp->next->pre = father_fcb;
            }else if(tmp->next==father_fcb){
                tmp->pre->next = father_fcb;

            }else{
                tmp->pre->next = tmp->next;
                tmp->next->pre = tmp->pre;
            }
            return tmp;
        }
    }
    return NULL;
}

QString User::copy(FCB *father_fcb,FCB *fcb){

    if(father_fcb->deepQList!=NULL){
        for(FCB* tmp = father_fcb->deepQList;tmp!=father_fcb;tmp=tmp->next){
            if(fcb->name==tmp->name)
                return "粘贴失败,有相同名字文件";
        }
        fcb->next = father_fcb->deepQList;
        fcb->pre = father_fcb;
        father_fcb->deepQList->pre = fcb;
        father_fcb->deepQList = fcb;
    }else{
        father_fcb->deepQList = fcb;
        fcb->pre = father_fcb;
        fcb->next = father_fcb;
    }
    return "";
}

QVector<FCB*> User::cd(FCB* father_fcb)
{
    this->current_FCB = father_fcb;//更改当前目录
    QVector<FCB*> temp;
    if(father_fcb->deepQList==NULL)
        return temp;
    return dir(father_fcb);
}

int User::left_Block(){
    int counter=0;
    for(int i=0;i<BlockCount;i++){
        if(this->ItDisk->FAT1[i]==0){
            counter++;
        }
    }
    return counter;
}

QString FCB::getname(){
    return name;
}
