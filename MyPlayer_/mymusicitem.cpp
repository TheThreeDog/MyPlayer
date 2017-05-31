#include "mymusicitem.h"
#include <QHBoxLayout>
#include <QDesktopWidget>
#include <QApplication>
#include <QFileInfo>
//歌曲信息条目类部分定义
MyMusicItem::MyMusicItem(QWidget *parent) : QWidget(parent)
{
    //通过鼠标单击获取焦点
    this->setFocusPolicy(Qt::ClickFocus);
    this->is_selected = false;
    this->setAutoFillBackground(true);
    this->setMouseTracking(true);
    //设置背景透明
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/img/trans.png")));
    this->setPalette(palette);

    this->setFixedSize(360,34);

    r = RightKeyMenu::shareRightKeyMenu(0,QString(""),this);

    QHBoxLayout * h = new QHBoxLayout;
    left_label = new QLabel(this);
    left_label->setFixedWidth(260);
    h->addStretch(1);
    h->addWidget(left_label,6,Qt::AlignLeft);
    h->addStretch(3);
    button = NULL;
    this->setLayout(h);
}

void MyMusicItem::setText(QString str1)
{
    this->left_label->setText(str1);
}

void MyMusicItem::setLeftText(QString str)
{
    this->left_label->setText(str);
}

QString MyMusicItem::getLeftText()
{
    return this->left_label->text();
}

void MyMusicItem::setPath(QString path)
{
    this->path = path;

    //把音频文件的后缀改成wmv后缀
    mv_name = path.remove(path.right(3)) + "wmv";
}

void MyMusicItem::setMusicIndex(int index)
{
    this->music_index = index;
}

int MyMusicItem::getMusicIndex()
{
    return this->music_index;
}

QString MyMusicItem::getPath()
{
    return this->path;
}

RightKeyMenu *MyMusicItem::getRightKeyMenu()
{
    return this->r;
}

void MyMusicItem::openMV()
{
    Video *v = new Video(mv_name);
    v->show();
}

MyMusicItem::~MyMusicItem()
{

}

void MyMusicItem::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
        emit doubleClick(this->music_index);
}

void MyMusicItem::mousePressEvent(QMouseEvent *)
{
}

void MyMusicItem::mouseReleaseEvent(QMouseEvent *e)
{
    if(e->button()==Qt::RightButton){
        r = RightKeyMenu::shareRightKeyMenu(this->music_index,this->path,this);

        QDesktopWidget *desktop_widget =
                QApplication::desktop();
        //得到客户区矩形
        QRect client_rect = desktop_widget->availableGeometry();
        //得到应用程序矩形
        //QRect application_rect = desktop_widget->screenGeometry();
        int win_width = client_rect.width();
        int win_height = client_rect.height();
        if((e->globalY()+r->height()>win_height)&&(e->globalX()+r->width()>win_width)){
            r->move(win_width-r->width(),win_height-r->height());
            r->show();
            r->setFocus();
            return ;
        }
        if(e->globalY()+r->height()>win_height){
            r->move(e->globalX(),win_height-r->height());
            r->show();
            r->setFocus();
            return ;
        }
        if(e->globalX()+r->width()>win_width){
            r->move(win_width-r->width(),e->globalY());
            r->show();
            r->setFocus();
            return ;
        }
        r->move(e->globalPos());
        r->show();
        r->setFocus();
        return ;
    }
}

void MyMusicItem::focusInEvent(QFocusEvent *)
{
    is_selected = true;
    QPalette palette;
    palette.setColor(QPalette::Background, QColor(88,88,88));
    this->setPalette(palette);
}

void MyMusicItem::focusOutEvent(QFocusEvent *)
{
    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QPixmap(":/img/trans.png")));
    this->setPalette(palette);
    button->hide();
    is_selected = false;
}

void MyMusicItem::enterEvent(QEvent *)
{
    if(!is_selected){
        QPalette palette;
        palette.setColor(QPalette::Background, QColor(42,42,42));
        this->setPalette(palette);

        QFileInfo fi(mv_name);
        if(fi.exists()){
            if(button == NULL){
                button = new MyButton(":/img/mv_normal.png",
                                      ":/img/mv_hover.png",
                                      ":/img/mv_press.png",
                                      this);
                button->move(300,2);
                button->setCallback(this,my_selector(openMV));
            }
        }else {
            if(NULL == button){
                button = new MyButton(":/img/mvdisable.png",
                                      ":/img/mvdisable.png",
                                      ":/img/mvdisable.png",
                                      this);
                button->move(300,2);
            }
        }
        button->show();
    }
}

void MyMusicItem::leaveEvent(QEvent *)
{
    if(!is_selected){
        QPalette palette;
        palette.setBrush(QPalette::Background, QBrush(QPixmap(":/img/trans.png")));
        this->setPalette(palette);
        button->hide();
    }
}


