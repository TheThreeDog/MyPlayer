#include "cube.h"
#include <QPainter>
#include <QtMath>
Cube::Cube(QWidget *parent) : QWidget(parent)
{
    this->changeMode = true;
    this->ispress = false;
    this->setWindowFlags(Qt::FramelessWindowHint|Qt::Tool|Qt::WindowStaysOnTopHint);
    this->setAttribute(Qt::WA_TranslucentBackground);
    this->pic_hover.load(":/img/cube_hover.png");
    this->pic_normal.load(":/img/cube_normal.png");
    this->resize(pic_hover.size());
    this->move(100,100);
    this->setMask(pic_hover.mask());
    pic_label = new QLabel(this);
    pic_label->move(0,0);
    pic_label->resize(this->size());
    pic_label->setPixmap(pic_normal);
    loadButtons();
    hideButtons();

}

QVector<MyButton *> Cube::getButtons()
{
    return this->cube_buttons;
}

void Cube::loadButtons()
{
    button = new MyButton(":/img/cube_play_normal.png"
                          ,":/img/cube_play_hover.png"
                          ,":/img/cube_play_press.png"
                          ,this);
    button->move(87,30);
    cube_buttons.append(button);

    button = new MyButton(":/img/cube_pause_normal.png"
                          ,":/img/cube_pause_hover.png"
                          ,":/img/cube_pause_press.png"
                          ,this);
    button->move(85,30);
    cube_buttons.append(button);

    button = new MyButton(":/img/cube_previous_normal.png"
                          ,":/img/cube_previous_hover.png"
                          ,":/img/cube_previous_press.png"
                          ,this);
    button->move(30,58);
    cube_buttons.append(button);

    button = new MyButton(":/img/cube_next_normal.png"
                          ,":/img/cube_next_hover.png"
                          ,":/img/cube_next_press.png"
                          ,this);
    button->move(137,58);
    cube_buttons.append(button);

    button = new MyButton(":/img/cube_volumejia_normal.png"
                          ,":/img/cube_volumejia_hover.png"
                          ,":/img/cube_volumejia_press.png"
                          ,this);
    button->move(30,120);
    cube_buttons.append(button);

    button = new MyButton(":/img/cube_volumeadd_normal.png"
                          ,":/img/cube_volumeadd_hover.png"
                          ,":/img/cube_volumeadd_press.png"
                          ,this);
    button->move(140,120);
    cube_buttons.append(button);

    button = new MyButton(":/img/cube_lyric_normal.png"
                          ,":/img/cube_lyric_hover.png"
                          ,":/img/cube_lyric_press.png"
                          ,this);
    button->move(85,157);
    cube_buttons.append(button);
}

void Cube::hideButtons()
{
    for(int i = 0; i < cube_buttons.count(); i++)
        cube_buttons.at(i)->hide();
}

void Cube::showButtons()
{
    if(widget->player_->state()==QMediaPlayer::PlayingState){
    for(int i = 1; i < cube_buttons.count(); i++)
        cube_buttons.at(i)->show();
    }else {
        cube_buttons.at(0)->show();
        for(int i = 2; i < cube_buttons.count(); i++)
            cube_buttons.at(i)->show();
    }
}

void Cube::setWidget(Widget *w)
{
    this->widget = w;
}

Cube::~Cube()
{

}

void Cube::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->x()>60&&e->x()<177&&e->y()>63&&e->y()<151&&changeMode)
    {
        this->hide();
        widget->show();
        widget->activateWindow();
    }
    ispress = false;
    changeMode = true;
}

void Cube::mouseMoveEvent(QMouseEvent *e)
{
    if(ispress){
        this->move(this->pos()+(e->pos()-old_pos));
        changeMode = false;
    }

}

void Cube::mousePressEvent(QMouseEvent *e)
{
    if(e->button() ==  Qt::LeftButton){
        //this->setPixmap(QPixmap(pic_press));
            old_pos = e->pos();
            ispress = true;
    }
}

void Cube::enterEvent(QEvent *)
{
    pic_label->setPixmap(pic_hover);
    showButtons();
}

void Cube::leaveEvent(QEvent *)
{
    pic_label->setPixmap(pic_normal);
    hideButtons();
}

void Cube::paintEvent(QPaintEvent *)
{
}
