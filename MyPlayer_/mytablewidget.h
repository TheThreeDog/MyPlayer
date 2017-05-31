#ifndef MYTABLEWIDGET_H
#define MYTABLEWIDGET_H

#include <QLabel>
#include <QVector>
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QEvent>
#include <QPixmap>
#include <QPaintEvent>
#include "mymusicitem.h"

class MyTableWidget :public QWidget
{

public :
    explicit MyTableWidget(QWidget *parent = 0);
    void addItem(MyMusicItem *music_item);
    void removeItem(int index);
    QVector<MyMusicItem*> * getLabels();
    ~MyTableWidget();
protected:
    void wheelEvent(QWheelEvent *e);
private:
    QVector<MyMusicItem*> labels;
    MyMusicItem *label_item;
};


#endif // MYTABLEWIDGET_H
