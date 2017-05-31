#include "widget.h"
#include <QPainter>
#include <QBitmap>
#include <QStandardPaths>
#include <QTimer>
#include <QTime>
#include <QFileDialog>
#include <QTextCodec>
#include "qxtglobalshortcut.h"
#include "cube.h"
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    //设置全局热键
    QxtGlobalShortcut* shortcut1 = new QxtGlobalShortcut(QKeySequence("Alt+P"), this);

    connect(shortcut1, SIGNAL(activated()), this, SLOT(playOrPause()));

    QxtGlobalShortcut* shortcut2 = new QxtGlobalShortcut(QKeySequence("Alt+Left"), this);

    connect(shortcut2, SIGNAL(activated()), this, SLOT(playPrevious()));

    QxtGlobalShortcut* shortcut3 = new QxtGlobalShortcut(QKeySequence("Alt+Right"), this);

    connect(shortcut3, SIGNAL(activated()), this, SLOT(playNext()));

    QxtGlobalShortcut* shortcut4 = new QxtGlobalShortcut(QKeySequence("Alt+Up"), this);

    connect(shortcut4, SIGNAL(activated()), this, SLOT(volumeAdd()));

    QxtGlobalShortcut* shortcut5 = new QxtGlobalShortcut(QKeySequence("Alt+Down"), this);

    connect(shortcut5, SIGNAL(activated()), this, SLOT(volumeSub()));

    network_start = false;

    initInterface();

}

void Widget::initInterface()
{
    change_mode = true;
    press = false;
    this->setWindowFlags(Qt::FramelessWindowHint);//设置窗体无边框

    this->setAttribute(Qt::WA_TranslucentBackground);//设置背景透明

    this->setWindowIcon(QIcon(":/img/ico.png"));
    pic.load(":/img/main_interface.png");//加载图像

    this->resize(pic.size());
    this->setMask(QBitmap(pic.mask()));

    //创建系统托盘图标
    this->createStstemTrayIcon();
    //加载播放列表路径
    save_path = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);

    //初始化音量
    volume_ = 80;
    //初始化播放列表
    play_list_ = new QMediaPlaylist;
    //设置播放模式为列表循环
    play_list_->setPlaybackMode(QMediaPlaylist::Loop);

    //初始化播放器
    player_ = new QMediaPlayer;
    player_->setPlaylist(play_list_);//为播放器绑定播放列表
    player_->setVolume(volume_);

    //初始化魔方
    cube = new Cube;
    cube->setWidget(this);
    cube->getButtons().at(0)->setCallback(this,my_selector(playOrPause));
    cube->getButtons().at(1)->setCallback(this,my_selector(playOrPause));
    cube->getButtons().at(2)->setCallback(this,my_selector(playPrevious));
    cube->getButtons().at(3)->setCallback(this,my_selector(playNext));
    cube->getButtons().at(4)->setCallback(this,my_selector(volumeSub));
    cube->getButtons().at(5)->setCallback(this,my_selector(volumeAdd));
    //桌面歌词的开关
    cube->getButtons().at(6)->setCallback(this,my_selector(setLyricShown));

    //初始化lrc歌词类
    lrc = new MyLrc;
    lrc->hide();
    //lrc->show();

    //四个按钮的水平布局

    //音量调节槽
    volume_slider = new VolumeSlider(this);
    volume_slider->setMaximum(100);
    volume_slider->setValue(4);
    volume_slider->move(351,603);
    volume_slider->show();
    //音乐播放进度条
    //自定义进度条
    myslider = new MySlider(this);
    myslider->move(145,640);

    //播放列表的显示区域
    table_widget = new MyTableWidget(this);
    table_widget->move(30,150);
    //在这里读入用户的播放列表文件
    QFile file(save_path+".list");
    if(file.open(QIODevice::ReadOnly|QIODevice::Text)){
        QTextStream in(&file);
        QStringList path_list;
        while(!in.atEnd()){
            QString item ;
            item = in.readLine();
            path_list.append(item);
        }
        if(!path_list.isEmpty()&&path_list.at(0)!="")
            readMusicList(path_list);

    }

    //设置字体颜色
    QPalette palette ;
    palette.setColor(QPalette::WindowText,Qt::lightGray);
    //音乐名显示标签
    music_info = new QLabel(this);
    music_info->setText("MyPlayer");
    music_info->setFixedSize(180,30);
    music_info->move(140,610);
    music_info->setPalette(palette);

    //音乐时间标签
    palette.setColor(QPalette::WindowText,QColor(241,101,40));
    music_time_label = new QLabel(this);
    music_time_label->setText("00:00            00:00");
    music_time_label->setFixedSize(200,30);
    music_time_label->move(175,590);
    music_time_label->setPalette(palette);

    //加载按钮
    loadButtons();
    //设置按钮的回调函数
    add_btn->setCallback(this,my_selector(openFile));
    play_btn->setCallback(this,my_selector(playOrPause));
    pause_btn->setCallback(this,my_selector(playOrPause));
    next_btn->setCallback(this,my_selector(playNext));
    previous_btn->setCallback(this,my_selector(playPrevious));
    mini_btn->setCallback(this,my_selector(minisizeWindow));
    close_btn->setCallback(this,my_selector(close));
    setting_btn->setCallback(this,my_selector(createSetting));
    lyric_open_btn->setCallback(this,my_selector(setLyricShown));
    lyric_close_btn->setCallback(this,my_selector(setLyricShown));
    remote_start_btn->setCallback(this,my_selector(startOrStopNetwork));
    remote_stop_btn->setCallback(this,my_selector(startOrStopNetwork));

    connect(play_mode_btn,SIGNAL(playModeChange(int )),
            this,SLOT(playModeChange(int)));

    //音量槽数值改变
    connect(volume_slider,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));

    //进度条数值改变
    connect(myslider,SIGNAL(valueChanged(int)),this,SLOT(setPosition(int)));
    //定时器
    timer = new QTimer;
    connect(timer,SIGNAL(timeout()),this,SLOT(setMusicSliderPosition()));
    //播放的音乐改变，就出发此槽函数
    connect(player_,SIGNAL(currentMediaChanged(QMediaContent)),this,SLOT(mediaChange(QMediaContent)));
    //托盘被触发事件
    connect(tray_icon_,SIGNAL(activated(QSystemTrayIcon::ActivationReason))
            ,this,SLOT(systrayIconActive(QSystemTrayIcon::ActivationReason)));

}

void Widget::loadButtons()
{
    play_btn = new MyButton(":/img/play_normal.png",
                            ":/img/play_hover.png",
                            ":/img/play_press.png",
                            this);
    play_btn->move(96,596);
    previous_btn = new MyButton(":/img/previous_normal.png",
                                ":/img/previous_hover.png",
                                ":/img/previous_press.png",
                                this);
    previous_btn->move(53,572);

    next_btn = new MyButton(":/img/next_normal.png",
                            ":/img/next_hover.png",
                            ":/img/next_press.png",
                            this);
    next_btn->move(53,620);

    add_btn = new MyButton(":/img/add_normal.png",
                           ":/img/add_hover.png",
                           ":/img/add_press.png",
                           this);
    add_btn->move(137,141);

    close_btn = new MyButton(":/img/close_normal.png",
                             ":/img/close_hover.png",
                             ":/img/close_press.png",
                             this);
    close_btn->move(344,72);

    lyric_open_btn = new MyButton(":/img/lyric_open_normal.png",
                                  ":/img/lyric_open_hover.png",
                                  ":/img/lyric_open_press.png",
                                  this);
    lyric_open_btn->move(301,145);
    lyric_open_btn->hide();

    lyric_close_btn = new MyButton(":/img/lyric_close_normal.png",
                                   ":/img/lyric_close_hover.png",
                                   ":/img/lyric_close_press.png",
                                   this);
    lyric_close_btn->move(301,145);

    mini_btn = new MyButton(":/img/mini_normal.png",
                            ":/img/mini_hover.png",
                            ":/img/mini_press.png",
                            this);
    mini_btn->move(301,90);

    pause_btn = new MyButton(":/img/pause_normal.png",
                             ":/img/pause_hover.png",
                             ":/img/pause_press.png",
                             this);
    pause_btn->move(96,596);

    pause_btn->hide();

    remote_start_btn = new MyButton(":/img/remote_close.png",
                                    ":/img/remote_close.png",
                                    ":/img/remote_close.png",
                                    this);
    remote_start_btn->move(180,117);
    remote_stop_btn = new MyButton(":/img/remote_open.png",
                                   ":/img/remote_open.png",
                                   ":/img/remote_open.png",
                                   this);
    remote_stop_btn->move(180,117);
    remote_stop_btn->hide();

    serch_btn = new MyButton(":/img/serch_normal.png",
                             ":/img/serch_hover.png",
                             ":/img/serch_press.png",
                             this);
    serch_btn->move(217,141);

    setting_btn = new MyButton(":/img/setting_normal.png",
                               ":/img/setting_hover.png",
                               ":/img/setting_press.png",
                               this);
    setting_btn->move(262,72);

    play_mode_btn = new PlayModeButton(this);
    play_mode_btn->move(260,120);
}

Widget::~Widget()
{

}

void Widget::playTo(int i)
{
    if(play_list_->currentIndex() == i){
        if(player_->state()==QMediaPlayer::PausedState
                ||player_->state()==QMediaPlayer::StoppedState){
            QString str = table_widget->getLabels()->at(i)->getPath();
            resolveLrc(str);
            player_->play();
            play_btn->hide();
            pause_btn->show();
            //play_button->setText("暂停");
        }
        else {
            player_->pause();
            play_btn->show();
            pause_btn->hide();
            //play_button->setText("播放");
        }
    }else {
        play_list_->setCurrentIndex(i);
        myslider->setMaximum(player_->duration());
        if(!play_list_->currentMedia().isNull()){
            QString str = table_widget->getLabels()->at(i)->getPath();
            resolveLrc(str);
            player_->play();
            //更换显示歌曲名信息
            music_info->setText(table_widget->getLabels()
                                ->at(play_list_->currentIndex())
                                ->getLeftText());
            music_info->setToolTip(table_widget->getLabels()
                                   ->at(play_list_->currentIndex())
                                   ->getLeftText());
            //更换标题信息
            this->setWindowTitle(table_widget->getLabels()
                                 ->at(play_list_->currentIndex())
                                 ->getLeftText());
            //托盘的显示信息更换
            tray_icon_->setToolTip(table_widget->getLabels()
                                   ->at(play_list_->currentIndex())
                                   ->getLeftText());
            timer->start(100);
            //play_button->setText("暂停");
            play_btn->hide();
            pause_btn->show();
        }
    }

}

void Widget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.drawPixmap(0, 0, pic);//绘制图像
}

void Widget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        old_pos = e->globalPos();
        press = true;
    }
}

void Widget::mouseMoveEvent(QMouseEvent *e)
{
    if(press)
    {
        this->move(this->pos() + e->globalPos() - old_pos);
        old_pos = e->globalPos();
        change_mode = false;
    }

}

void Widget::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->y()>10&&e->y()<130&&e->x()>30&&e->x()<125&&change_mode){
        this->hide();
        cube->show();
    }
    press = false;
    change_mode = true;
}

void Widget::closeEvent(QCloseEvent *e)
{
    if(tray_icon_->isVisible())//如果系统托盘图表存在，则不退出，隐藏窗体！
    {
        this->hide();
        tray_icon_->showMessage("提示","MyPlayer已托管至系统托盘");
        //保存用户播放列表
        QString file_name = save_path + ".list";
        QFile file(file_name); //通过文件路径并打开创建文件
        QTextStream out (&file); //构造文件输出流
        if(file.open(QIODevice::WriteOnly|QFile::Truncate|QIODevice::Text)){
            //qDebug()<<table_widget->getLabels()->count();
            for(int i = 0; i < table_widget->getLabels()->count(); i++ ){
                out<<table_widget->getLabels()->at(i)->getPath()<<endl;
            }
        }
        file.close();
        e->ignore();
    }else{
        e->accept();
    }
}

void Widget::minisizeWindow()
{
    this->showMinimized();
}

void Widget::volumeAdd()
{
    if(player_->volume()<100){
        player_->setVolume(player_->volume()+10);
    }
}

void Widget::volumeSub()
{
    if(player_->volume() > 0)
        player_->setVolume(player_->volume()-10);
}

void Widget::createSetting()
{
    Settings *s = Settings::sharedSettingWindow();
    s->show();
}

void Widget::playOrPause(){
    if(player_->state()==QMediaPlayer::PausedState
            ||player_->state()==QMediaPlayer::StoppedState){
        if(!play_list_->currentMedia().isNull()){
            QString str = table_widget->getLabels()->at(play_list_->currentIndex())->getPath();
            resolveLrc(str);
            player_->play();
            //更换显示歌曲名信息
            music_info->setText(table_widget->getLabels()
                                ->at(play_list_->currentIndex())
                                ->getLeftText());
            music_info->setToolTip(table_widget->getLabels()
                                   ->at(play_list_->currentIndex())
                                   ->getLeftText());
            //更换标题信息
            this->setWindowTitle(table_widget->getLabels()
                                 ->at(play_list_->currentIndex())
                                 ->getLeftText());
            //托盘的显示信息更换
            tray_icon_->setToolTip(table_widget->getLabels()
                                   ->at(play_list_->currentIndex())
                                   ->getLeftText());
            timer->start(100);
            //play_button->setText("暂停");
            play_btn->hide();
            pause_btn->show();
        }
    }
    else{
        player_->pause();
        //play_button->setText("播放");
        play_btn->show();
        pause_btn->hide();
    }

}

void Widget::openFile(){
    QString inital_name = QDir::homePath();
    QStringList path_list = QFileDialog::getOpenFileNames(this,"选择文件",inital_name,"*.mp3");
    if(!path_list.isEmpty()&&path_list.at(0)!="")
        readMusicList(path_list);

}

void Widget::playNext()
{
    int current_index = play_list_->currentIndex();
    if(++current_index == play_list_->mediaCount()) current_index = 0;
    play_list_->setCurrentIndex(current_index);
    myslider->setMaximum(player_->duration());
    if(!play_list_->currentMedia().isNull()){
        QString str = table_widget->getLabels()->at(play_list_->currentIndex())->getPath();
        resolveLrc(str);
        player_->play();
        timer->start(100);
        play_btn->hide();
        pause_btn->show();
        //play_button->setText("暂停");
    }
}

void Widget::playPrevious()
{
    int currentIndex = play_list_->currentIndex();
    if(--currentIndex < 0) currentIndex = play_list_->mediaCount()-1;
    play_list_->setCurrentIndex(currentIndex);
    myslider->setMaximum(player_->duration());
    if(!play_list_->currentMedia().isNull()){
        QString str = table_widget->getLabels()->at(play_list_->currentIndex())->getPath();
        resolveLrc(str);
        player_->play();
        timer->start(100);
        //play_button->setText("暂停");
        play_btn->hide();
        pause_btn->show();
    }
}

void Widget::readMusicList(QStringList path_list)
{
    int i = this->table_widget->getLabels()->count();
    //i获取当前列表中条目的数量，从列表处开始添加。
    //用music_num遍历获取到的列表曲目连续添加
    int music_num = 0;
    for(; music_num < path_list.size(); music_num++){
        QString path = QDir::toNativeSeparators(path_list.at(music_num));
        if(!path.isEmpty()){
            //将新歌曲添加到播放列表中，两者通过相同的index联系！
            play_list_->addMedia(QUrl::fromLocalFile(path));
            if(player_->state() == QMediaPlayer::StoppedState)
                play_list_->setCurrentIndex(i);
            //将新歌曲添加到自定义的显示列表中
            QString file_name = path.split("\\").last();
            MyMusicItem* new_music = new MyMusicItem(table_widget);
            new_music->setPath(path);
            new_music->setLeftText(file_name.split(".").front());
            new_music->setMusicIndex(i);

            table_widget->addItem(new_music);//添加至播放列表
            //将条目的双击信号与槽函数关联
            connect(new_music,SIGNAL(doubleClick(int)),this,SLOT(playTo(int)));
            //列表右键菜单触发事件
            //记得每次也要将i++，i还作为歌曲index传入！
            i++;
        }
    }
    //  右键菜单中的槽函数们
    if(music_num == 0) return ; //如果没有打开任何文件，就不连接这些槽函数
    connect(this->table_widget->getLabels()->at(0)->getRightKeyMenu(),
            SIGNAL(playBtnClick(int)),
            this,
            SLOT(playTo(int)));
    connect(this->table_widget->getLabels()->at(0)->getRightKeyMenu(),
            SIGNAL(removeMusicSignal(int)),
            this,
            SLOT(removeMusic(int )));
    connect(this->table_widget->getLabels()->at(0)->getRightKeyMenu(),
            SIGNAL(addNewMusic()),
            this,
            SLOT(openFile()));
    connect(this->table_widget->getLabels()->at(0)->getRightKeyMenu(),
            SIGNAL(playModeChanged()),
            this,
            SLOT(playModeChange()));
    connect(this->table_widget->getLabels()->at(0)->getRightKeyMenu(),
            SIGNAL(changeToCubeMode()),
            this,
            SLOT(showCube()));
    connect(this->table_widget->getLabels()->at(0)->getRightKeyMenu(),
            SIGNAL(setWindowTop()),
            this,
            SLOT(setWindowOnTop()));
    connect(this->table_widget->getLabels()->at(0)->getRightKeyMenu(),
            SIGNAL(clearListorder()),
            this,
            SLOT(removeAllMusic()));
}

void Widget::createStstemTrayIcon()
{
    tray_icon_ = new QSystemTrayIcon(this);
    tray_icon_->setIcon(QIcon(":/img/ico.png"));
    tray_icon_->setToolTip("MyPlayer-音乐播放器");
    tray_icon_->show();

    open_mainwindow = new QAction("打开主面板",this);
    connect(open_mainwindow,SIGNAL(triggered()),this,SLOT(show()));

    exit = new QAction("退出",this);
    connect(exit,SIGNAL(triggered()),qApp,SLOT(quit()));

    systray_menu = new QMenu(this);
    systray_menu->addAction(open_mainwindow);
    systray_menu->addSeparator();
    systray_menu->addAction(exit);
    //添加右键菜单
    tray_icon_->setContextMenu(systray_menu);
}

void Widget::removeMusic(int index)
{
    //qDebug()<<play_list_->mediaCount();
    //if(play_list_->removeMedia(index)){
    table_widget->getLabels()->at(index)->close();
    table_widget->getLabels()->remove(index);
    for(int i = index; i < table_widget->getLabels()->count();i++){
        MyMusicItem * m = table_widget->getLabels()->at(i);
        m->move(m->x(),m->y()-m->height());
        m->setMusicIndex(m->getMusicIndex()-1);
    }
    //}
}

void Widget::setPosition(int value)
{
    qint64 currentPosition=player_->position();
    if(qAbs(currentPosition-value)>99)
    {
        player_->stop();
        timer->stop();
        player_->setPosition(value);
        player_->play();
        timer->start();
        play_btn->hide();
        pause_btn->show();
        //play_button->setText("暂停");
    }

}

void Widget::setMusicSliderPosition()
{
    qint64 position=player_->position();
    myslider->setMaximum(player_->duration());
    myslider->setSliderPosition(position);
    //myslider->update();
    setMusicTime(player_->duration(),player_->position());

    qint64 time = player_->position();
    // 获取当期时间对应的歌词
    if(!lrc_map.isEmpty()) {
        // 获取当前时间在歌词中的前后两个时间点
        qint64 previous = 0;
        qint64 later = 0;
        //keys()方法返回lrc_map列表
        foreach (qint64 value, lrc_map.keys()) {
            if (time >= value){
                previous = value;
            } else {
                later = value;
                break;
            }
        }

        // 达到最后一行,将later设置为歌曲总时间的值
        if (later == 0)
            later = player_->duration();

        // 获取当前时间所对应的歌词内容
        QString current_lrc = lrc_map.value(previous);

        //        // 没有内容时
        //        if(current_lrc.length() < 2)
        //            current_lrc = tr("简易音乐播放器");

        // 如果是新的一行歌词，那么重新开始显示歌词遮罩
        if(current_lrc != lrc->text()) {
            lrc->setText(current_lrc);
            qint64 interval_time = later - previous;
            lrc->start_lrc_mask(interval_time);
        }
    }
}

void Widget::setMusicTime(qint64 duration, qint64 position)
{
    QTime time1(0,position/60000,position/1000%60);
    QTime time2(0,duration/60000,duration/1000%60);
    music_time_label->setText(QString("%1            %2")
                              .arg(time1.toString("mm:ss"))
                              .arg(time2.toString("mm:ss")));
}

void Widget::mediaChange(QMediaContent)
{
    if(!player_->state()==QMediaPlayer::StoppedState)
    {
        //重新加载歌词文件
        QString str = table_widget->getLabels()->at(play_list_->currentIndex())->getPath();
        resolveLrc(str);
        //把进度条归零
        myslider->setSliderPosition(0);
        //更换显示歌曲名信息
        music_info->setText(table_widget->getLabels()
                            ->at(play_list_->currentIndex())
                            ->getLeftText());
        music_info->setToolTip(table_widget->getLabels()
                               ->at(play_list_->currentIndex())
                               ->getLeftText());
        //更换标题信息
        this->setWindowTitle(table_widget->getLabels()
                             ->at(play_list_->currentIndex())
                             ->getLeftText());
        //托盘的显示信息更换
        tray_icon_->setToolTip(table_widget->getLabels()
                               ->at(play_list_->currentIndex())
                               ->getLeftText());
    }
}

void Widget::systrayIconActive(QSystemTrayIcon::ActivationReason reason)
{
    switch(reason){
    case QSystemTrayIcon::DoubleClick:
    case QSystemTrayIcon::Trigger:
        this->showNormal();
        //this->show();
        //设为活动窗口，即把窗口放到顶层
        this->activateWindow();
        break;
    default:
        break;
    }
}

void Widget::valueChanged(int value)
{
    player_->setVolume(100-value);
}

void Widget::playModeChange(int i)
{
    switch(i){
    case 1:
        play_list_->setPlaybackMode(QMediaPlaylist::Loop);
        break;
    case 2:
        play_list_->setPlaybackMode(QMediaPlaylist::CurrentItemInLoop);
        break;
    case 3:
        play_list_->setPlaybackMode(QMediaPlaylist::Random);
        break;
    case 4:
        play_list_->setPlaybackMode(QMediaPlaylist::Loop);
        break;
    }
}

void Widget::playModeChange()
{
    int i = rand()%3+1;
    playModeChange(i);
}

void Widget::showCube()
{
    this->hide();
    cube->show();
}

void Widget::setWindowOnTop()
{
    static bool on_top = false;
    on_top = !on_top;
    if(on_top)
        this->setWindowFlags(this->windowFlags()|Qt::WindowStaysOnTopHint);
    else
        this->setWindowFlags(this->windowFlags()&~Qt::WindowStaysOnTopHint);
    this->show();
}

//解析歌词文件
void Widget::resolveLrc(const QString &source_file_name)
{
    lrc_map.clear();
    if(source_file_name.isEmpty())
        return;
    QString file_name = source_file_name;
    QString lrc_file_name = file_name.remove(file_name.right(3)) + "lrc";//把音频文件的后缀改成lrc后缀

    // 打开歌词文件
    QFile file(lrc_file_name);
    if (!file.open(QIODevice::ReadOnly)) {
        lrc->setText(QString(table_widget->getLabels()->at(play_list_->currentIndex())->getLeftText())
                     + tr(" --- 未找到歌词文件！"));
        return ;
    }
    // 设置字符串编码
    //QTextCodec::setCodecForLocale(QTextCodec::codecForLocale());
    QString all_text = QString(file.readAll());
    file.close();
    // 将歌词按行分解为歌词列表
    QStringList lines = all_text.split("\n");

    //这个是时间标签的格式[00:05.54]
    //正则表达式d{2}表示匹配2个数字
    QRegExp rx("\\[\\d{2}:\\d{2}\\.\\d{2}\\]");
    foreach(QString oneline, lines) {
        QString temp = oneline;
        temp.replace(rx, "");//用空字符串替换正则表达式中所匹配的地方,这样就获得了歌词文本
        // 然后依次获取当前行中的所有时间标签，并分别与歌词文本存入QMap中
        //indexIn()为返回第一个匹配的位置，如果返回为-1，则表示没有匹配成功
        //正常情况下pos后面应该对应的是歌词文件
        int pos = rx.indexIn(oneline, 0);
        while (pos != -1) { //表示匹配成功
            QString cap = rx.cap(0);//返回第0个表达式匹配的内容
            // 将时间标签转换为时间数值，以毫秒为单位
            QRegExp regexp;
            regexp.setPattern("\\d{2}(?=:)");
            regexp.indexIn(cap);
            int minute = regexp.cap(0).toInt();
            regexp.setPattern("\\d{2}(?=\\.)");
            regexp.indexIn(cap);
            int second = regexp.cap(0).toInt();
            regexp.setPattern("\\d{2}(?=\\])");
            regexp.indexIn(cap);
            int millisecond = regexp.cap(0).toInt();
            qint64 totalTime = minute * 60000 + second * 1000 + millisecond * 10;
            // 插入到lrc_map中
            lrc_map.insert(totalTime, temp);
            pos += rx.matchedLength();
            pos = rx.indexIn(oneline, pos);//匹配全部
        }
    }
    // 如果lrc_map为空
    if (lrc_map.isEmpty()) {
        lrc->setText(QString(table_widget->getLabels()->at(play_list_->currentIndex())->getLeftText())
                     + tr(" --- 歌词文件内容错误！"));
        return;
    }
}

void Widget::setLyricShown()
{
    if(lrc->isHidden()){
        lrc->show();
        lyric_open_btn->show();
        lyric_close_btn->hide();
    }
    else{
        lrc->hide();
        lyric_open_btn->hide();
        lyric_close_btn->show();
    }
}

void Widget::startOrStopNetwork()
{

    if(remote_stop_btn->isHidden()){
        tcp_server = new QTcpServer(this);
        //监听任何脸上的60000端口的ip
        //qDebug()<<"listen start";
        tcp_server->listen(QHostAddress::Any,60000);
        connect(tcp_server,SIGNAL(newConnection()),this,SLOT(newConnect()));
        remote_stop_btn->show();
        remote_start_btn->hide();
    }else {
        tcp_server->close();
        tcp_server = NULL;
        remote_stop_btn->hide();
        remote_start_btn->show();
    }

}

void Widget::newConnect()
{
    //qDebug()<<"getConnection";
    //得到每个连接进来的socket
    tcp_socket = tcp_server->nextPendingConnection();
    //有可读的信息，触发读函数槽
    connect(tcp_socket,SIGNAL(readyRead()),this,SLOT(readMessage()));
}

void Widget::readMessage()
{
    qint64 len = tcp_socket->bytesAvailable();
    QByteArray alldata = tcp_socket->read(len);
    //开始转码
    QTextCodec *utf8code = QTextCodec::codecForName("UTF-8");
    QString utf8str = utf8code->toUnicode(alldata.mid(2));
    //qDebug()<<"utf-8["<<utf8str<<"]";
    if(utf8str == "play_previous")
        this->playNext();
    if(utf8str == "play_next")
        this->playPrevious();
    if(utf8str == "volume_up")
        this->volumeAdd();
    if(utf8str == "volume_down")
        this->volumeSub();
    if(utf8str == "play_or_pause")
        this->playOrPause();
    if(utf8str == "shutdown_delay")
        system("shutdown -s -t 3600");
    if(utf8str == "shutdown")
        system("shutdown -s ");
    if(utf8str == "cancle")
        system("shutdown -a");


}

void Widget::removeAllMusic()
{
    for(int i = table_widget->getLabels()->count()-1;i>=0;i--){
        player_->stop();
        play_list_->removeMedia(i);
        MyMusicItem *item = table_widget->getLabels()->at(i);
        table_widget->getLabels()->remove(i);
        item->close();
    }
}


