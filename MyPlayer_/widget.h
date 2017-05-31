#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>
#include "mybutton.h"
#include <QIcon>
#include <QMediaPlayer>
#include <QMenu>
#include <QMediaPlaylist>
#include <QSystemTrayIcon>
#include <QTimerEvent>
#include <QDir>
#include <QApplication>
#include <QTcpServer>
#include <QTcpSocket>
#include "myslider.h"
#include "mytablewidget.h"
#include "volumeslider.h"
#include "playmodebutton.h"
#include "settings.h"
#include "mylrc.h"
//继承NativeEventFilter响应全局事件（全局热键）
class Cube;
class Widget : public QWidget
{
    Q_OBJECT

public:
    friend class Cube;
    Widget(QWidget *parent = 0);
    void createStstemTrayIcon();
    void initInterface();
    void loadButtons();
    void minisizeWindow();
    void createSetting();
    void readMusicList(QStringList path_list);
    void resolveLrc(const QString &source_file_name);
    void setLyricShown();
    void startOrStopNetwork();
    ~Widget();
public slots:
    void playOrPause();
    void playTo( int i);
    void playNext();
    void playPrevious();
    void volumeAdd();
    void volumeSub();
    void openFile();
    void removeMusic(int index);
    void setPosition(int);
    void setMusicSliderPosition();
    void setMusicTime(qint64 duration,qint64 position);
    void mediaChange(QMediaContent);
    void systrayIconActive(QSystemTrayIcon::ActivationReason reason);
    void valueChanged(int value);
    void playModeChange(int i);
    void playModeChange();
    void showCube();
    void setWindowOnTop();
    void removeAllMusic();
    void newConnect();
    void readMessage();
protected:
    void paintEvent(QPaintEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void closeEvent(QCloseEvent *e);
private:
    QPoint old_pos;
    bool press;
    QPixmap pic;

    int volume_;
    QMediaPlayer *player_;
    QMediaPlaylist *play_list_;
    QSystemTrayIcon *tray_icon_;
    QLabel *music_info;
    QString save_path;
    QTimer *timer;
    QLabel *music_time_label;
    QMenu *systray_menu;
    QAction *open_mainwindow;
    QAction *exit;
    QTcpServer *tcp_server;
    QTcpSocket *tcp_socket;

    MySlider * myslider;
    VolumeSlider * volume_slider;
    MyTableWidget *table_widget;
    Cube *cube;
    MyLrc *lrc;
    QMap<qint64, QString> lrc_map;
    //判断是否能够切换模式
    bool change_mode;
    bool network_start;

    MyButton *play_btn;
    MyButton *next_btn;
    MyButton *previous_btn;
    MyButton *add_btn;
    MyButton *close_btn;
    MyButton *lyric_open_btn;
    MyButton *lyric_close_btn;
    MyButton *mini_btn;
    MyButton *pause_btn;
    MyButton *remote_start_btn;
    MyButton *remote_stop_btn;
    MyButton *serch_btn;
    MyButton *setting_btn;
    PlayModeButton * play_mode_btn;
};

#endif // WIDGET_H
