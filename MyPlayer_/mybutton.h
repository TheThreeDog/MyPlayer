#ifndef MYBUTTON_H
#define MYBUTTON_H
#include <QLabel>
#include <QPixmap>
//回调函数实现
typedef void (QObject::*SEL_MenuHandler)(QObject *);
#define my_selector(_SELECTOR) (SEL_MenuHandler)(&_SELECTOR)

class MyButton : public QLabel
{
    Q_OBJECT
public:
    MyButton();
    MyButton( QString pic1,                 //正常图片
              QString pic2,                 //鼠标经过图片
              QString pic3,                 //鼠标按下图片
              QWidget *parent=0);           //父控件
    void setMoveEnable(bool canmove);
    bool isCanMove();
    void setCallback(QObject *rec, SEL_MenuHandler selector);

    ~MyButton();
signals:
    void clicked(MyButton* clicked);    //信号函数！鼠标离开时发送信号
protected:
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    //回调函数的实现：
    //回调函数的调用者
    QObject* m_pListener;
    //回调函数的函数指针
    SEL_MenuHandler m_pfnSelector;
    //保护类型的三态图片
    QString pic_normal;
    QString pic_hover;
    QString pic_press;
private:
    bool canmove;
    bool ispress;
    QPoint position;
    QPixmap m_pixmap;
};

#endif // MYBUTTON_H
