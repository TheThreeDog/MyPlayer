#include "rightmenubutton.h"
#include <QPalette>

RightMenuButton::RightMenuButton(QWidget *parent):QLabel(parent)
{
    enable = true;
    QPalette palette;
    palette.setColor(QPalette::WindowText,Qt::lightGray);
    palette.setColor(QPalette::Background,Qt::transparent);
    this->setPalette(palette);
    QFont font("../myplayer1/msyh.ttf",11);
    this->setFont(font);
    this->setAutoFillBackground(true);
    this->setFixedSize(195,28);
    caller = NULL;
    callback = NULL;

}

RightMenuButton::~RightMenuButton()
{

}

void RightMenuButton::setCallback(QObject *caller, FUNC_PTR func_ptr)
{
    this->caller = caller;
    this->callback = func_ptr;

}

void RightMenuButton::setEnabled(bool enable)
{
    this->enable = enable;
    QPalette palette;
    palette.setColor(QPalette::WindowText,Qt::darkGray);
    palette.setColor(QPalette::Background,Qt::transparent);
    this->setPalette(palette);
    QFont font("../myplayer1/msyh.ttf",11);
    this->setFont(font);
    this->setAutoFillBackground(true);
    this->setFixedSize(195,28);

}

void RightMenuButton::mousePressEvent(QMouseEvent *)
{

}

void RightMenuButton::mouseReleaseEvent(QMouseEvent *)
{
    if(enable){
        if(caller&&callback)
            (caller->*callback)();
    }
}
void RightMenuButton::enterEvent(QEvent *)
{
    if(enable){
        QPalette palette;
        palette.setColor(QPalette::Background,Qt::darkGray);
        palette.setColor(QPalette::WindowText,Qt::lightGray);
        this->setPalette(palette);
    }else {}
}

void RightMenuButton::leaveEvent(QEvent *)
{
    if(enable){
        QPalette palette;
        palette.setColor(QPalette::Background,Qt::transparent);
        palette.setColor(QPalette::WindowText,Qt::lightGray);
        this->setPalette(palette);
    }else{}
}

