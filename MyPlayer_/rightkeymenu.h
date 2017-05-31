#ifndef RIGHTKEYMENU_H
#define RIGHTKEYMENU_H
#include <QLabel>
#include <QVector>
#include <QPaintEvent>
#include <QPainter>
#include <QMouseEvent>
#include <QFocusEvent>
#include <QEvent>
#include <QPixmap>
#include <QPaintEvent>
#include "rightmenubutton.h"
#include "settings.h"
class RightKeyMenu : public QWidget
{
    Q_OBJECT

public:
    static RightKeyMenu* shareRightKeyMenu(int music_index,QString path,QWidget *parent);
    static RightKeyMenu *r;
    void setPath(QString path);
    void setMusicIndex(int index);
    int  getMusicIndex();
    void openInExplorer();
    void playBtnClicked();
    void playModeChange();
    void removeMusic();
    void addMusic();
    void cubeMode();
    void setWindowOnTop();
    void clearList();
    void timerSetting();
    void shotKeySetting();
    ~RightKeyMenu();
protected:
    void leaveEvent(QEvent *);
    void enterEvent(QEvent *);
    void timerEvent(QTimerEvent *);
    void paintEvent(QPaintEvent *);
    void focusOutEvent(QFocusEvent *);
signals:
    void addNewMusic();
    void playModeChanged();
    void playBtnClick(int index);
    void removeMusicSignal(int index);
    void changeToCubeMode();
    void setWindowTop();
    void clearListorder();
private:
    //私有化构造函数，采用单例模式
    explicit RightKeyMenu(QWidget *parent = 0);
    RightKeyMenu(int musci_index,QString path, QWidget *parent = 0);

    QPixmap bg_pic;
    QString path;
    int music_index;
    int timer_id;
};
#endif // RIGHTKEYMENU_H
