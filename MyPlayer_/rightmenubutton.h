#ifndef RIGHTMENUBUTTON_H
#define RIGHTMENUBUTTON_H
#include <QLabel>
#include <QMouseEvent>
#include <QEvent>
typedef void (QObject::*FUNC_PTR) ();

class RightMenuButton : public QLabel
{

public:
    explicit RightMenuButton(QWidget *parent = 0);
    ~RightMenuButton();
    void setCallback(QObject * caller,FUNC_PTR func_ptr);
    void setEnabled(bool );
protected:
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
private :
    FUNC_PTR callback;
    QObject * caller;
    bool enable;

};

#endif // RIGHTMENUBUTTON_H
