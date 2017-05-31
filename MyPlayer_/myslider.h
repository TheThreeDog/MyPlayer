#ifndef MYSLIDER_H
#define MYSLIDER_H
#include <QLabel>
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QEvent>
#include <QPixmap>
#include <QPaintEvent>

class MySlider : public QWidget
{
    Q_OBJECT

public :
    explicit MySlider(QWidget *parent = 0);
    void setMaximum(qint64 duration);
    void setSliderPosition(qint64 position);
    qint64 getMaximum();
signals:
    void valueChanged(int);
protected:
    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
private:
    qint64 maximum;
    int current_position;
    bool ispress;

};
#endif // MYSLIDER_H
