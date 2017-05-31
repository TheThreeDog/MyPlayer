#ifndef MYMUSICITEM_H
#define MYMUSICITEM_H

#include <QLabel>
#include <QVector>
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QEvent>
#include <QPixmap>
#include <QPaintEvent>
#include "mybutton.h"
#include "rightkeymenu.h"
#include "video.h"

class MyMusicItem : public QWidget
{
    Q_OBJECT
public:
    explicit MyMusicItem(QWidget *parent = 0);
    void setText(QString str1);
    void setLeftText(QString str);
    QString getLeftText();
    void setPath(QString path);
    void setMusicIndex(int index);
    int getMusicIndex();
    QString getPath();
    RightKeyMenu * getRightKeyMenu();
    void openMV();
    ~MyMusicItem();
signals:
    void doubleClick(int index);
public slots:
protected:
    void mouseDoubleClickEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *e);
    void focusInEvent(QFocusEvent *);
    void focusOutEvent(QFocusEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
private :
    QLabel * left_label;
    QString path;
    int music_index;
    bool is_selected;
    RightKeyMenu * r;
    MyButton *button;
    QString mv_name;
};


#endif // MYMUSICITEM_H
