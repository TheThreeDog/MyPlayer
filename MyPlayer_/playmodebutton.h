#ifndef PLAYMODEBUTTON_H
#define PLAYMODEBUTTON_H
#include <QLabel>
#include <QPixmap>
#include <QMouseEvent>
//enum TYPE{LOOP,GO,ONELOOP,RANDOM};
class PlayModeButton : public QLabel
{
    Q_OBJECT

public:
    explicit PlayModeButton(QWidget * parent);
    ~PlayModeButton();
signals:
    void playModeChange(int);
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
private:
    QPixmap pic_loop;
    QPixmap pic_go;
    QPixmap pic_oneloop;
    QPixmap pic_random;
    int type;
};

#endif // PLAYMODEBUTTON_H
