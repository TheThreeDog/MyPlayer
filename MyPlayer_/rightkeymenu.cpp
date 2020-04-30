#include "rightkeymenu.h"
#include <QBitmap>
#include <QFileInfo>
#include <QDesktopServices>
//自定义右键菜单部分定义
//初始化单例模式对象
RightKeyMenu *RightKeyMenu::r = NULL;
RightKeyMenu::RightKeyMenu(int music_index,QString path,QWidget *parent):QWidget(parent)
{
    //使用焦点监听事件//通过鼠标获取焦点
    this->setFocusPolicy(Qt::ClickFocus);
    this->path = path;
    this->music_index = music_index;
    this->timer_id  = 0;
    //利用Tool属性没有任务栏图标
    this->setWindowFlags(Qt::FramelessWindowHint
                         |Qt::Tool);
    this->setAttribute(Qt::WA_TranslucentBackground);
    bg_pic.load(":/img/right.png");
    this->resize(bg_pic.size());
    this->setMask(QBitmap(bg_pic.mask()));

    //右键菜单中的按钮
    RightMenuButton * play_btn ;
    RightMenuButton * open_in_explorer_btn;
    RightMenuButton * remove_btn;
    RightMenuButton * delete_btn;

    play_btn = new RightMenuButton(this);
    play_btn->move(33,2);
    play_btn->setText("播放/暂停");
    play_btn->setCallback(this,my_selector(RightKeyMenu::playBtnClicked));

    open_in_explorer_btn = new RightMenuButton(this);
    open_in_explorer_btn->move(33,30);
    open_in_explorer_btn->setText("打开文件所在位置");
    open_in_explorer_btn->setCallback(this,my_selector(RightKeyMenu::openInExplorer));

    remove_btn = new RightMenuButton(this);
    remove_btn->move(33,60);
    remove_btn->setText("移除歌曲");
    remove_btn->setCallback(this,my_selector(RightKeyMenu::removeMusic));

    delete_btn = new RightMenuButton(this);
    delete_btn->move(33,90);
    delete_btn->setText("清空列表");
    delete_btn->setCallback(this,my_selector(RightKeyMenu::clearList));

    RightMenuButton * add_btn = new RightMenuButton(this);
    add_btn->move(33,130);
    add_btn->setText("添加歌曲");
    add_btn->setCallback(this,(FUNC_PTR)addMusic);

    RightMenuButton * play_model_btn = new RightMenuButton(this);
    play_model_btn->move(33,160);
    play_model_btn->setText("播放模式");
    play_model_btn->setCallback(this,my_selector(RightKeyMenu::playModeChange));

    RightMenuButton * remote_controll_btn = new RightMenuButton(this);
    remote_controll_btn->move(33,200);
    remote_controll_btn->setText("开启遥控器");
    remote_controll_btn->setEnabled(false);

    RightMenuButton * cube_mode_btn = new RightMenuButton(this);
    cube_mode_btn->move(33,230);
    cube_mode_btn->setText("魔方模式");
    cube_mode_btn->setCallback(this,my_selector(RightKeyMenu::cubeMode));

    RightMenuButton * on_time_btn = new RightMenuButton(this);
    on_time_btn->move(33,260);
    on_time_btn->setText("定时设置");
    on_time_btn->setCallback(this,my_selector(RightKeyMenu::timerSetting));

    RightMenuButton * window_on_top_btn = new RightMenuButton(this);
    window_on_top_btn->move(33,300);
    window_on_top_btn->setText("窗口总在最前");
    window_on_top_btn->setCallback(this,my_selector(RightKeyMenu::setWindowOnTop));

    RightMenuButton * shot_key_btn = new RightMenuButton(this);
    shot_key_btn->move(33,330);
    shot_key_btn->setText("热键设置");
    shot_key_btn->setCallback(this,my_selector(RightKeyMenu::shotKeySetting));

    RightMenuButton * help_btn = new RightMenuButton(this);
    help_btn->move(33,370);
    help_btn->setText("使用帮助");
    help_btn->setEnabled(false);
    RightMenuButton * advice_btn = new RightMenuButton(this);
    advice_btn->move(33,400);
    advice_btn->setText("意见反馈");
    advice_btn->setEnabled(false);
    RightMenuButton * about_btn = new RightMenuButton(this);
    about_btn->move(33,430);
    about_btn->setText("关于MyPlayer");
    about_btn->setEnabled(false);
}
//获取单例模式对象
RightKeyMenu*  RightKeyMenu::shareRightKeyMenu(int music_index,QString path,QWidget *parent)
{
    if(r == NULL)
        r = new RightKeyMenu(music_index,path,parent);
    else {
        r->setPath(path);
        r->setMusicIndex(music_index);
    }
    return r;
}

void RightKeyMenu::setPath(QString path)
{
    this->path = path;
}

void RightKeyMenu::setMusicIndex(int index)
{
    this->music_index = index;
}

int RightKeyMenu::getMusicIndex()
{
    return this->music_index;
}

RightKeyMenu::~RightKeyMenu()
{

}

void RightKeyMenu::leaveEvent(QEvent *)
{
    this->timer_id = this->startTimer(500);
}

void RightKeyMenu::enterEvent(QEvent *)
{
    if(this->timer_id != 0){
        this->killTimer(timer_id);
    }
}

void RightKeyMenu::timerEvent(QTimerEvent *)
{
    this->hide();
}

void RightKeyMenu::paintEvent(QPaintEvent *)
{
    QPainter painter(this);

    painter.drawPixmap(0, 0, bg_pic);//绘制图像
}

void RightKeyMenu::focusOutEvent(QFocusEvent *)
{
    this->hide();
}

void RightKeyMenu::openInExplorer()
{
    this->hide();
    QFileInfo fi = QFileInfo(path);
    QString file_path = fi.absolutePath();
    //格式必须确定，前面需要加上file：///
    QString path = QString("file:///").append(file_path);
    QDesktopServices::openUrl(QUrl(path, QUrl::TolerantMode));
}

void RightKeyMenu::playBtnClicked()
{
    this->hide();
    emit playBtnClick(this->music_index);
}

void RightKeyMenu::playModeChange()
{
    this->hide();
    emit playModeChanged();
}

void RightKeyMenu::removeMusic()
{
    this->hide();
    emit removeMusicSignal(this->music_index);
}

void RightKeyMenu::addMusic()
{
    this->hide();
    emit addNewMusic();
}

void RightKeyMenu::cubeMode()
{
    this->hide();
    emit changeToCubeMode();
}

void RightKeyMenu::setWindowOnTop()
{
    this->hide();
    emit setWindowTop();
}

void RightKeyMenu::clearList()
{
    this->hide();
    emit clearListorder();
}

void RightKeyMenu::timerSetting()
{
    this->hide();
    Settings *s = Settings::sharedSettingWindow();
    s->timer_setting->setFocus();
    s->show();
}

void RightKeyMenu::shotKeySetting()
{
    this->hide();
    Settings *s = Settings::sharedSettingWindow();
    s->shotkey_setting->setFocus();
    s->show();
}
