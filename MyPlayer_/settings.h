#ifndef SETTINGS_H
#define SETTINGS_H

#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QCloseEvent>
#include <QLabel>
#include <QEvent>
typedef void (QObject::*SEL_MenuHandler)(QObject *);
#define my_selector(_SELECTOR) (SEL_MenuHandler)(&_SELECTOR)

class SettingButton;
class Settings : public QWidget
{
    Q_OBJECT
public:
    static Settings* sharedSettingWindow();
    static Settings * s;
    ~Settings();

    SettingButton *normal_setting;
    SettingButton *remote_setting;
    SettingButton *timer_setting;
    SettingButton *shotkey_setting;
    SettingButton *lyric_setting;
public slots:
    void set1();
    void set2();
    void set3();
    void set4();
    void set5();
protected:
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent * );
    void mousePressEvent(QMouseEvent * );
    void closeEvent(QCloseEvent *);
    void paintEvent(QPaintEvent *);
private:
    explicit Settings(QWidget *parent = 0);
    QPoint old_pos;
    QPixmap pic;
    bool press;
    QLabel *right_label;
};

class SettingButton : public QLabel
{
    Q_OBJECT

public:
    explicit SettingButton(QString pic_normal,
                           QString pic_hover,
                           QString pic_press,
                           QWidget *parent = 0);
    void setCallback(QObject *rec, SEL_MenuHandler selector);
    ~SettingButton();
    void setFocusPic(QString pic);
signals:
    void click();
protected:
    void focusInEvent(QFocusEvent *ev);
    void focusOutEvent(QFocusEvent *ev);
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void enterEvent(QEvent *);
    void leaveEvent(QEvent *);
private:
    //回调函数的实现：
    //回调函数的调用者
    QObject* m_pListener;
    //回调函数的函数指针
    SEL_MenuHandler m_pfnSelector;
    bool isselected;
    QPixmap pic_normal;
    QPixmap pic_hover;
    QPixmap pic_press;
    QPixmap pic_focus;
};

#endif // SETTINGS_H
