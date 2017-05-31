#ifndef VIDEO_H
#define VIDEO_H

#include <QMovie>
#include <QWidget>
#include <QMenuBar>
#include <QMenu>
#include <QAction>

#include <QPushButton>
#include <QComboBox>
#include <QSlider>
#include <QLabel>

#include <QMediaPlayer>
#include"videowidget.h"
#include <QTimer>
#include <QTimerEvent>

#include <QBoxLayout>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPainter>

#include <QTableView>
#include <QMediaPlaylist>
#include <QStandardItemModel>

#include <QMap>

class Video : public QWidget
{
    Q_OBJECT
public:
    explicit Video(QString path,QWidget *parent = 0);
    ~Video();
    void createBtn();

protected:
    void timerEvent(QTimerEvent *event);
    void paintEvent(QPaintEvent *);
    void closeEvent(QCloseEvent *e);
signals:

public slots:
    void getOpenFile(QString path);
    void aboutUs();
    void play();
private slots:
    void mediaStateChanged(QMediaPlayer::State state);
    void positionChanged(qint64 position);
    void durationChanged(qint64 duration);
    void reSetPosition(int );
    void reSetVolumn(int );
    void volumnClicked();
    void updateRate(int);
    void videoPause();
    void changeMovie(QModelIndex);
    void preVideo();
    void nextVideo();
    void fullScreen();

private:
    //按钮等
    QComboBox *comBox;
    QPushButton *playBtn;
    QSlider *timeSlider;
    QSlider *volumnSlider;
    QLabel *timeLbl;
    QPushButton *volumnBtn;
    QLabel *volumnNum;
    QPushButton *fScreen;

    QMediaPlayer::State playerState;
    QMediaPlayer mediaPlayer;
    VideoWidget *videoWidget;

    QMap<QString,int> myList;
    int count;
    int mark;

    qint64 ch,cm,cs;
    qint64 h,m,s;
    int TimeId;
    int old_volume;
};

#endif // VIDEO_H
