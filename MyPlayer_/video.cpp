#include "video.h"
#include <QFileDialog>
#include <QMessageBox>

#include <QtWidgets>
#include <QVideoSurfaceFormat>

Video::Video(QString path,QWidget *parent) : QWidget(parent)
{
    createBtn();
    cs = cm = ch =0;
    s = m = h = -1;
    count = -1;
    mark = -1;
    TimeId = 0;
    mediaPlayer.setVideoOutput(videoWidget);

    connect(&mediaPlayer,SIGNAL(stateChanged(QMediaPlayer::State)),
            this,SLOT(mediaStateChanged(QMediaPlayer::State)));

    connect(&mediaPlayer,SIGNAL(positionChanged(qint64)),
            this,SLOT(positionChanged(qint64)));

    connect(&mediaPlayer, SIGNAL(durationChanged(qint64)),
            this, SLOT(durationChanged(qint64)));
    this->setFont(QFont("../font/msyh.ttf",12,5));
    getOpenFile(path);
}

Video::~Video()
{

}
//组件布局
void Video::createBtn()
{
    videoWidget = new VideoWidget(this);
    videoWidget->setSizePolicy(QSizePolicy::Ignored,QSizePolicy::Ignored);

    timeSlider = new QSlider(Qt::Horizontal);
    timeSlider->setRange(0,0);
    connect(timeSlider,SIGNAL(sliderMoved(int)),this,SLOT(reSetPosition(int )));

    timeLbl = new QLabel(this);

    comBox = new QComboBox(this);
    comBox->addItem("0.5x");
    comBox->addItem("1.0x");
    comBox->addItem("2.0x");
    comBox->setCurrentIndex(1);
    connect(comBox,SIGNAL(currentIndexChanged(int)),this,SLOT(updateRate(int)));

    playBtn = new QPushButton(this);
    playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
    playBtn->setEnabled(false);
    connect(playBtn,SIGNAL(clicked()),this,SLOT(play()));

    volumnBtn = new QPushButton(this);
    volumnBtn->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
    connect(volumnBtn,SIGNAL(clicked()),this,SLOT(volumnClicked()));

    volumnNum = new QLabel(this);

    volumnSlider = new QSlider(Qt::Horizontal);
    volumnSlider->setRange(0,100);
    connect(volumnSlider,SIGNAL(sliderMoved(int)),this,SLOT(reSetVolumn(int )));

    fScreen = new QPushButton(this);
    fScreen->setText("全屏");
    fScreen->setEnabled(false);
    connect(fScreen,SIGNAL(clicked()),this,SLOT(fullScreen()));

    QBoxLayout *layout = new QHBoxLayout;
    layout->addWidget(videoWidget);

    QBoxLayout *layout1 = new QHBoxLayout;
    layout1->addWidget(timeSlider);
    layout1->addWidget(timeLbl);

    QBoxLayout *layout2 = new QHBoxLayout;
    layout2->addStretch(100);
    layout2->addWidget(comBox);
    layout2->addWidget(playBtn);
    layout2->addWidget(volumnBtn);
    layout2->addWidget(volumnSlider);
    layout2->addWidget(volumnNum);
    layout2->addWidget(fScreen);
    layout2->addStretch(100);

    QBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addLayout(layout);
    mainLayout->setStretch(1,100);
    mainLayout->addLayout(layout1);
    mainLayout->addLayout(layout2);
    setLayout(mainLayout);
}
//定时器事件
void Video::timerEvent(QTimerEvent *e)
{
    if(e->timerId() == TimeId){
        qint64 temp;
        temp = mediaPlayer.duration()/1000;
        h = temp /3600;
        temp%=3600;
        m = temp /60;
        temp%= 60;
        s = temp;

        temp = mediaPlayer.position()/1000;
        ch = temp /3600;
        temp%=3600;
        cm = temp /60;
        temp%= 60;
        cs = temp;

    //节目总时间
        QString sstr = QString("%1").arg(s).rightJustified(2,'0');
        QString mstr = QString("%1").arg(m).rightJustified(2,'0');
        QString hstr = QString("%1").arg(h).rightJustified(2,'0');
    //    当前时间
        QString csstr = QString("%1").arg(cs).rightJustified(2,'0');
        QString cmstr = QString("%1").arg(cm).rightJustified(2,'0');
        QString chstr = QString("%1").arg(ch).rightJustified(2,'0');
        QString str = QString("%1:%2:%3/%4:%5:%6").arg(chstr)
                .arg(cmstr).arg(csstr).arg(hstr).arg(mstr).arg(sstr);
        timeLbl->setText(str);
    }
}

void Video::paintEvent(QPaintEvent *)
{
    QImage image(":/images/13.jpg");
    QPainter painter(this);
    QRect target(0,0,this->width(),this->height());
    QRect source(0,0,image.width(),image.height());
    painter.drawImage(target,image,source);
}

void Video::closeEvent(QCloseEvent *e)
{
    this->hide();
    mediaPlayer.stop();
    e->ignore();
}
//打开文件
void Video::getOpenFile(QString path)
{
    //QString path = QFileDialog::getOpenFileName(this, tr("Open Movie"));
    QFileInfo info = QFileInfo(path);
    QString fileName = info.baseName();
    fScreen->setEnabled(true);

    if(!fileName.isEmpty()){
        count = myList.size();
        myList.insert(path,count);
        int i;
        for(i = 0;i< myList.size();i++){
            if(myList.size() == (count+1)){
                //itemModel->setItem(count,0,new QStandardItem(fileName));
                mark = count;
            }
        }
        QString video = myList.key(count);
        mediaPlayer.setMedia(QUrl::fromLocalFile(video));
        playBtn->setEnabled(true);
        mediaPlayer.play();
        if(count >= 1){
        }
    }
    TimeId = this->startTimer(1000);
}

void Video::aboutUs()
{
    QMessageBox::information(this,"关于","播放器!",QMessageBox::Ok);
}
//播放
void Video::play()
{
    TimeId = this->startTimer(1000);

    //volumnSlider->setValue(30);
    //mediaPlayer.setVolume(30);
    //volumnNum->setText(QString("%1").arg(30));
    switch(mediaPlayer.state()){
    case QMediaPlayer::PlayingState:
        mediaPlayer.pause();
        break;
    default:
        mediaPlayer.play();
        break;
    }
}
//播放与暂停图标设置
void Video::mediaStateChanged(QMediaPlayer::State state)
{
    switch (state){
    case QMediaPlayer::PlayingState:
        playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
        break;
    default:
        playBtn->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
        break;
    }
}

void Video::positionChanged(qint64 position)
{
    if(mediaPlayer.duration() == mediaPlayer.position()){
        this->killTimer(TimeId);
        timeLbl->setText("00:00:00");
        timeSlider->setValue(0);
    }
    timeSlider->setValue(position);
}

void Video::durationChanged(qint64 duration)
{
    timeSlider->setRange(0,duration);
}
//滚动进度条
void Video::reSetPosition(int posi)
{
    mediaPlayer.setPosition(posi);
}
//声音滚动条
void Video::reSetVolumn(int volumn)
{
    mediaPlayer.setVolume(volumn);
    volumnSlider->setValue(volumn);
    volumnNum->setText(QString("%1").arg(volumn));
}
//声音按钮点击槽函数
void Video::volumnClicked()
{
    if(mediaPlayer.volume()){
        volumnBtn->setIcon(style()->standardIcon(QStyle::SP_MediaVolumeMuted));
        old_volume = mediaPlayer.volume();
        mediaPlayer.setVolume(0);
        volumnSlider->setValue(0);
        volumnNum->setText(QString("%1").arg(0));
    }else{
        volumnBtn->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));
        mediaPlayer.setVolume(old_volume);
        volumnSlider->setValue(old_volume);
        volumnNum->setText(QString("%1").arg(old_volume));
    }
}
//播放倍率的设置
void Video::updateRate(int rate)
{
    if(rate == 0){
        mediaPlayer.setPlaybackRate(0.5);
    }else if(rate == 1){
        mediaPlayer.setPlaybackRate(1.0);
    }else{
        mediaPlayer.setPlaybackRate(2.0);
    }
}

void Video::videoPause()
{
    mediaPlayer.pause();
}

void Video::changeMovie(QModelIndex index)
{
    int i = index.row();
    mark = i;
    QString video = myList.key(i);
    mediaPlayer.setMedia(QUrl::fromLocalFile(video));
    mediaPlayer.play();
}

void Video::preVideo()
{
    if(mark == 0){
        QString video = myList.key(myList.size()-1);
        mediaPlayer.setMedia(QUrl::fromLocalFile(video));
        mark = myList.size() - 1;
        mediaPlayer.play();
    }else{
        QString video = myList.key(mark-1);
        mediaPlayer.setMedia(QUrl::fromLocalFile(video));
        mark--;
        mediaPlayer.play();
    }
    TimeId = this->startTimer(TimeId);
}

void Video::nextVideo()
{
    if(mark == (myList.size()-1)){
        QString video = myList.key(0);
        mark = 0;
        mediaPlayer.setMedia(QUrl::fromLocalFile(video));
        mediaPlayer.play();
    }else{
        QString video = myList.key(mark+1);
        mediaPlayer.setMedia(QUrl::fromLocalFile(video));
        mark++;
        mediaPlayer.play();
    }
    TimeId = this->startTimer(TimeId);
}

void Video::fullScreen()
{
    this->videoWidget->setFullScreen(true);
}
