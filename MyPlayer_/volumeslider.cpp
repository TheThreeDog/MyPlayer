#include "volumeslider.h"

VolumeSlider::VolumeSlider(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(30,80);
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    maximum = 0;
    current_position = 0;
    ispress = false;
}

void VolumeSlider::setMaximum(qint64 max)
{
    this->maximum = max;
}

void VolumeSlider::setValue(int value)
{

    //this->slider_position = position;
    //将坐标按比例转化为滑条按钮的横坐标
    double p = value;
    double m = maximum;
    //必须要用浮点数，不然出来结果是0
    double x =(p/m)*this->height();
    if(!this->ispress){
        current_position = x;
    }
    update();
}

VolumeSlider::~VolumeSlider()
{

}
void VolumeSlider::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    //left top width height!!!!
    QRect rect(5,0,3,this->height());
    p.fillRect(rect,QColor(241,101,40));
    QRect rect2(5,0,3,current_position);
    p.fillRect(rect2,Qt::gray);
    QPoint center(6,current_position);
    p.setBrush(Qt::white);
    p.setPen(Qt::white);
    p.drawEllipse(center,4,4);
}

void VolumeSlider::mouseReleaseEvent(QMouseEvent *e)
{
    current_position = e->pos().y();
    double x = e->pos().y();
    double w = this->height();
    qint64 music_pos = x/w*maximum;
    emit valueChanged(music_pos);
    update();
    ispress = false;
}

void VolumeSlider::mouseMoveEvent(QMouseEvent *e)
{
    if(e->y()>3&&e->y()<this->height()-3){
        this->current_position = e->pos().y();
        update();
    }
}

void VolumeSlider::mousePressEvent(QMouseEvent *e)
{
    current_position = e->pos().y();
    update();
    ispress = true;
}

