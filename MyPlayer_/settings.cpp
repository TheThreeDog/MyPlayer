#include "settings.h"
#include <QBitmap>
#include <QIcon>
#include <QDebug>
#include <QBitmap>
//级的初始化静态成员
Settings *Settings::s = NULL;
Settings::Settings(QWidget *parent) : QWidget(parent)
{
    this->press = false;
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setAttribute(Qt::WA_TranslucentBackground,true);
    this->setWindowIcon(QIcon(":/img/ico.png"));
    this->setWindowTitle("设置");
    pic.load(":/img/settings_bg.png");
    this->resize(pic.size());
    this->setMask(pic.mask());

    QPixmap p(":/img/set1.png");
    right_label = new QLabel(this);
    right_label->setPixmap(p);
    right_label->resize(p.size());
    right_label->setMask(p.mask());
    right_label->move(184,68);

    normal_setting = new SettingButton(
                ":/img/changgui_normal.png",
                ":/img/changgui_hover.png",
                ":/img/changgui_normal.png",
                this
                );
    normal_setting->move(60,102);
    normal_setting->setFocusPic(":/img/changgui_selected.png");
    connect(normal_setting,SIGNAL(click()),this,SLOT(set1()));
    normal_setting->setCallback(this,my_selector(Settings::set1));
    //normal_setting->setFocus();

    remote_setting = new SettingButton(
                ":/img/yaokong_normal.png",
                ":/img/yaokong_hover.png",
                ":/img/yaokong_normal.png",
                this
                );
    remote_setting->move(60,149);
    remote_setting->setFocusPic(":/img/yaokong_selected.png");
    connect(remote_setting,SIGNAL(click()),this,SLOT(set2()));
    remote_setting->setCallback(this,my_selector(Settings::set2));

    timer_setting = new SettingButton(
                ":/img/dingshi_normal.png",
                ":/img/dingshi_hover.png",
                ":/img/dingshi_normal.png",
                this
                );
    timer_setting->move(60,196);
    timer_setting->setFocusPic(":/img/dingshi_selected.png");
    connect(timer_setting,SIGNAL(click()),this,SLOT(set3()));
    timer_setting->setCallback(this,my_selector(Settings::set3));

    shotkey_setting = new SettingButton(
                ":/img/rejian_normal.png",
                ":/img/rejian_hover.png",
                ":/img/rejian_normal.png",
                this
                );
    shotkey_setting->move(60,243);
    shotkey_setting->setFocusPic(":/img/rejian_selected.png");
    connect(shotkey_setting,SIGNAL(click()),this,SLOT(set4()));
    shotkey_setting->setCallback(this,my_selector(Settings::set4));

    lyric_setting = new SettingButton(
                ":/img/geci_normal.png",
                ":/img/geci_hover.png",
                ":/img/geci_normal.png",
                this
                );
    lyric_setting->move(60,290);
    lyric_setting->setFocusPic(":/img/geci_selected.png");
    connect(lyric_setting,SIGNAL(click()),this,SLOT(set5()));
    lyric_setting->setCallback(this,my_selector(Settings::set5));

}

Settings *Settings::sharedSettingWindow()
{
    if(s == NULL){
        s = new Settings();
        s->activateWindow();
    }else{
        s->activateWindow();
    }
    return s;

}

void Settings::set1()
{
    right_label->setPixmap(QPixmap(":/img/set1.png"));
}
void Settings::set2()
{
    right_label->setPixmap(QPixmap(":/img/set2.png"));
}
void Settings::set3()
{
    right_label->setPixmap(QPixmap(":/img/set3.png"));
}
void Settings::set4()
{
    right_label->setPixmap(QPixmap(":/img/set4.png"));
}
void Settings::set5()
{
    right_label->setPixmap(QPixmap(":/img/set5.png"));
}

Settings::~Settings()
{

}

void Settings::closeEvent(QCloseEvent *e)
{
    this->hide();
    e->ignore();
}

void Settings::paintEvent(QPaintEvent *)
{
    //画笔要记得设置指定this
    QPainter p(this);
    p.drawPixmap(0,0,pic);

}

void Settings::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        old_pos = e->globalPos();
        press = true;
    }
}

void Settings::mouseMoveEvent(QMouseEvent *e)
{
    if(press)
    {
        this->move(this->pos() + e->globalPos() - old_pos);
        old_pos = e->globalPos();
    }

}

void Settings::mouseReleaseEvent(QMouseEvent *)
{
    press = false;
}

SettingButton::SettingButton(QString pic_normal,
                             QString pic_hover,
                             QString pic_press,
                             QWidget *parent):QLabel(parent)
{
    this->pic_normal.load(pic_normal);
    this->pic_hover.load(pic_hover);
    this->pic_press.load(pic_press);
    this->setPixmap(pic_normal);
    this->setFocusPolicy(Qt::ClickFocus);

    m_pListener = NULL;
    m_pfnSelector = NULL;
}

void SettingButton::setCallback(QObject *rec, SEL_MenuHandler selector)
{
    m_pListener = rec;
    m_pfnSelector = selector;
}

SettingButton::~SettingButton()
{

}

void SettingButton::setFocusPic(QString pic)
{
    this->pic_focus.load(pic);
}

void SettingButton::focusInEvent(QFocusEvent *)
{
    this->setPixmap(pic_focus);
    if (m_pListener && m_pfnSelector)
    {
    //    (m_pListener->*m_pfnSelector)(this);    // 直接调用了便是
        emit click();
    }
    isselected = true;
}

void SettingButton::focusOutEvent(QFocusEvent *)
{
    this->setPixmap(pic_normal);
    isselected = false;
}

void SettingButton::mousePressEvent(QMouseEvent *ev)
{
    if(!isselected && ev->button() == Qt::LeftButton)
        this->setPixmap(pic_press);
}

void SettingButton::mouseReleaseEvent(QMouseEvent *)
{
    if(!isselected){
        this->setPixmap(pic_normal);
    }
}

void SettingButton::enterEvent(QEvent *)
{
    if(!isselected)
        this->setPixmap(pic_hover);
}

void SettingButton::leaveEvent(QEvent *)
{
    if(!isselected)
        this->setPixmap(pic_normal);
}
