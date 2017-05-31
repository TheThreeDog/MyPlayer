#ifndef VOLUMESLIDER_H
#define VOLUMESLIDER_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>

class VolumeSlider : public QWidget
{
    Q_OBJECT

public:
    explicit VolumeSlider(QWidget *parent = 0);
    void setMaximum(qint64 max);
    void setValue(int value);
    ~VolumeSlider();
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

#endif // VOLUMESLIDER_H
