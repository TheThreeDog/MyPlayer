#include "myslider.h"

//自定义进度条部分定义；

MySlider::MySlider(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(180,20);
    //this->setAttribute(Qt::WA_TranslucentBackground,true);
    //this->setWindowFlags(Qt::FramelessWindowHint);
    maximum = 0;
    current_position = 0;
    ispress = false;

}
void MySlider::setMaximum(qint64 duration)
{
    this->maximum = duration;
}

void MySlider::setSliderPosition(qint64 position)
{
    //this->slider_position = position;
    //将坐标按比例转化为滑条按钮的横坐标
    double p = position;
    double m = maximum;
    //必须要用浮点数，不然出来结果是0
    double x =(p/m)*this->width();
    if(!this->ispress){
        current_position = x;
    }
    update();
}

qint64 MySlider::getMaximum()
{
    return this->maximum;
}
void MySlider::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    //left top width height!!!!
    QRect rect(0,5,this->width(),3);
    p.fillRect(rect,Qt::gray);
    QRect rect2(0,5,current_position,3);
    p.fillRect(rect2,QColor(241,101,40));
    QPoint center(current_position,6);
    p.setBrush(Qt::white);
    p.setPen(Qt::white);
    p.drawEllipse(center,4,4);
}

void MySlider::mouseReleaseEvent(QMouseEvent *e)
{
    current_position = e->pos().x();
    double x = e->pos().x();
    double w = this->width();
    qint64 music_pos = x/w*maximum;
    emit valueChanged(music_pos);
    update();
    ispress = false;
}

void MySlider::mouseMoveEvent(QMouseEvent *e)
{
    if(e->x()>3&&e->x()<this->width()-3){
        this->current_position = e->pos().x();
        update();
    }
}

void MySlider::mousePressEvent(QMouseEvent *e)
{
    current_position = e->pos().x();
    update();
    ispress = true;
}
