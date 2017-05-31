#include "mytablewidget.h"
#include <QPalette>
#include <QPixmap>
#include <QBitmap>
//歌曲列表部分定义

MyTableWidget::MyTableWidget(QWidget *parent):QWidget(parent)
{
    this->setFixedSize(360,470);
    //设置条目部分的遮罩！
    QPixmap pic;
    pic.load(":/img/music_item_mask.png");
    this->setMask(pic.mask());
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    QPalette t;
    t.setColor(QPalette::WindowText,Qt::lightGray);
    this->setPalette(t);
}

void MyTableWidget::addItem(MyMusicItem *music_item)
{
    //妈的就一个show纠结了两天！！！！！！！
    music_item->show();
    //这里不应该直接移动固定距离，而是移动到上一个条目的末尾
    //music_item->move(QPoint(music_item->x(),music_item->getMusicIndex()*music_item->height()));
    if(music_item->getMusicIndex() != 0){
        int i = music_item->getMusicIndex();
        music_item->move(QPoint(music_item->x(),
                         this->getLabels()->at(i-1)->y()+ music_item->height()));
    }
    labels.append(music_item);
}

void MyTableWidget::removeItem(int index)
{
    this->labels.at(index)->close();
}

QVector<MyMusicItem *> * MyTableWidget::getLabels()
{
    return &this->labels;
}

MyTableWidget::~MyTableWidget()
{

}

void MyTableWidget::wheelEvent(QWheelEvent *e)
{
    if(e->orientation() == Qt::Vertical){
        if(e->delta() > 0){//滚动角度大于0
            //首先labels不能为空
            //如果第一个标签的坐标的y小于0，则说明往下移动的过多，则不能移动
            if(labels.count()!=0&&labels.at(0)->pos().y() < 32)//-32一个标签的长度
                for(int i = 0; i < this->labels.count();i++)
                    labels.at(i)->move(labels.at(i)->x(),labels.at(i)->y()+20);
        }
        else{
            //同理 如果最后一个标签的坐标的y+32（底角坐标）大于height，则说明往上移动的过多，则不能移动
            if(labels.count()!=0&&labels.at(labels.count()-1)->pos().y()+32 > this->height()-50)
                for(int i = 0; i < this->labels.count();i++)
                    labels.at(i)->move(labels.at(i)->x(),labels.at(i)->y()-20);
        }
    }
    e->accept();
}

