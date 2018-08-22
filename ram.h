#ifndef RAM_H
#define RAM_H
#include <QList>
#include <QString>
const int BlockSize = 512;//磁盘块大小
const int BlockCount = 128;//磁盘块个数
const int DiskSize = BlockSize*BlockCount;//磁盘大小

class FCB{
    public:

        FCB* pre;//指向前一个FCB，第一个孩子的pre指向父亲
        FCB* next;//指向后一个FCB，最后一个孩子的next指向父亲
        FCB* deepQList;//指向第一个孩子FCB
        FCB* fa;//指向父亲FCB
        QString name;//文件名称
        int typefile; //文件类型,2为文件，1为目录,0为未使用
        int size; //文件大小
        QString address;//存放路径
        QString detail;//文件内容

        int  block_num;//位于磁盘块所在号


        FCB():name("wag"),typefile(-1),size(0),address(""),
            detail(""),pre(NULL),next(NULL),deepQList(NULL),block_num(-1),fa(NULL){//FCB初始化

        }
        FCB(QString name,int type):name(name),typefile(type),size(0),address(""),
            detail(""),pre(NULL),next(NULL),deepQList(NULL),block_num(-1),fa(NULL){//FCB初始化

        }
        QString getname();

};
const int BlockFcbCount = DiskSize/sizeof(FCB);//目录文件最多FCB数目
//磁盘
class Disk{
    public:
        int FAT1[BlockCount]; //FAT1
        int FAT2[BlockCount]; //FAT2

        Disk(){
            memset(FAT1,0,BlockCount);
            memset(FAT2,0,BlockCount);//初始化两个FAT表
            FAT1[0]=FAT1[1]=FAT1[2]=-2; //0,1,2盘块号依次代表FAT1,FAT2,根目录区
            FAT2[0]=FAT2[1]=FAT2[2]=-2; //FAT2作备份

        }
};

class User{
    public:
        FCB* ItFCB;
        FCB* current_FCB;//当前目录
        Disk* ItDisk;//磁盘
        char* BaseAddr;//虚拟磁盘空间基地址
        int Current_Block;//当前目录的盘快号

        User(){
            this->ItFCB = new FCB("root",1);
            this->ItFCB->block_num = 2;//root所在磁盘号为2号
            this->current_FCB = this->ItFCB;
            this->ItDisk = new Disk();

            BaseAddr = (char*)malloc(DiskSize);//申请虚拟空间
            Current_Block = 2;//当前盘块为根目录所在盘块
        }
        QVector<FCB*> dir(FCB* father_fcb);//显示当前目录下文件
        FCB* mkdir(FCB* father_fcb,QString name);//新建目录，返回创建的目录指针
        FCB* create(FCB* father_fcb,QString name);//新建文件，返回创建的文件指针
        QString del(FCB* father_fcb,QString name); //删除文件
        QVector<FCB*> cd(FCB* father_fcb);//打开目录
        QString rename(FCB* fcb,QString name);//更改名字
        FCB* cut(FCB* father_fcb,QString name);//剪切
        QString copy(FCB *father_fcb,FCB* fcb);//复制
        int left_Block();//计算剩下的磁盘块

};
#endif // RAM_H
