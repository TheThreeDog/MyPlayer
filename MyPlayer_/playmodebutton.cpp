#include "playmodebutton.h"
#include "playmodebutton.h"

PlayModeButton::PlayModeButton(QWidget *parent):QLabel(parent)
{
    type = 1;
    pic_go.load(":/img/mode_go.png");
    pic_loop.load(":/img/mode_loop.png");
    pic_oneloop.load(":/img/mode_oneloop.png");
    pic_random.load(":/img/mode_random.png");
    this->setPixmap(pic_loop);
}


PlayModeButton::~PlayModeButton()
{

}

void PlayModeButton::mousePressEvent(QMouseEvent *)
{

}

void PlayModeButton::mouseReleaseEvent(QMouseEvent *)
{
    switch(type){
    case 1:
        this->setPixmap(pic_oneloop);
        this->type = 2;
        this->setToolTip("单曲循环");
        emit playModeChange(2);
        break;
    case 2:
        this->setPixmap(pic_random);
        this->type = 3;
        this->setToolTip("随机播放");
        emit playModeChange(3);
        break;
    case 3:
        this->setPixmap(pic_go);
        this->type = 4;
        this->setToolTip("顺序播放");
        emit playModeChange(4);
        break;
    case 4:
        this->setPixmap(pic_loop);
        this->type = 1;
        this->setToolTip("列表循环");
        emit playModeChange(1);
        break;
    }

}

void PlayModeButton::mouseMoveEvent(QMouseEvent *)
{

}

