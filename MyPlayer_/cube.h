#ifndef CUBE_H
#define CUBE_H

#include <QWidget>
#include <QPixmap>
#include <QBitmap>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QEvent>
#include <QLabel>
#include <QPaintEvent>
#include <QVector>
#include "mybutton.h"
#include "widget.h"

class Cube : public QWidget
{
    Q_OBJECT
public:
    explicit Cube(QWidget *parent = 0);
    QVector<MyButton*> getButtons();
    void loadButtons();
    void hideButtons();
    void showButtons();
    void setWidget(Widget *w);
    ~Cube();
protected:
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent * );
    void mousePressEvent(QMouseEvent * );
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void paintEvent(QPaintEvent *);
private:
    QVector<MyButton*> cube_buttons;
    QPixmap pic_normal;
    QPixmap pic_hover;
    QLabel *pic_label;
    MyButton * button;
    Widget *widget;
    QPoint old_pos;
    bool ispress;
    //是否更换模式，在魔方模式和普通模式的切换
    bool changeMode;
};
#endif // CUBE_H
