#include "mybutton.h"
#include "mybutton.h"
#include <QPixmap>
#include <QMouseEvent>
#include <QDebug>
#include <QBitmap>

MyButton::MyButton()
{
    new (this)MyButton("","","",0);
}
//参数canmove决定能否移动，是true的时候才执行对应代码
//变量ispress记录是否按下，如果没按下就不进行移动。
MyButton::MyButton(
        QString pic1,
        QString pic2,
        QString pic3,
        QWidget *parent):pic_normal(pic1),pic_hover(pic2),pic_press(pic3),QLabel(parent)
{
    //设置图像中透明部分为穿透，则选区为自定义图片
    m_pixmap.load(pic_normal);
    this->setPixmap(m_pixmap);
    //setMask( m_pixmap.mask() );

    //设置本控件的鼠标追踪
    this->setMouseTracking(true);
    ispress = false;
    position = QPoint(0,0);
    //设置图片随label自动拉伸
    //this->setScaledContents(true);
    this->m_pListener = NULL;
    this->m_pfnSelector = NULL;
    this->canmove = false;

}

void MyButton::setMoveEnable(bool canmove)
{
    this->canmove = canmove;
}

bool MyButton::isCanMove()
{
    return this->canmove;
}



MyButton::~MyButton()
{

}


void MyButton::enterEvent(QEvent *)
{
    this->setPixmap(QPixmap(pic_hover));
}

void MyButton::leaveEvent(QEvent *)
{
    this->setPixmap(QPixmap(pic_normal));
}
//鼠标按下时的操作
void MyButton::mousePressEvent(QMouseEvent *e)
{
    if(e->button() ==  Qt::LeftButton){
        this->setPixmap(QPixmap(pic_press));
        if (canmove){
            position = e->pos();
            ispress = true;
        }
    }
}
//鼠标抬起时的操作
void MyButton::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button() ==  Qt::LeftButton){
        this->setPixmap(QPixmap(pic_hover));
        if (canmove)
            ispress = false;
        emit clicked(this);             //发送clicked信号
        if (m_pListener && m_pfnSelector)
        {
            (m_pListener->*m_pfnSelector)(this);    // 直接调用了便是
        }
    }
}
void MyButton::setCallback(QObject *rec, SEL_MenuHandler selector)
{
    m_pListener = rec;
    m_pfnSelector = selector;
}

void MyButton::mouseMoveEvent(QMouseEvent *e)
{
    if (canmove){
        if(ispress){
            this->move(this->pos()+(e->pos()-position));
        }
    }

}
