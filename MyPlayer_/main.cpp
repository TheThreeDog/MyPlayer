#include "widget.h"
#include <QApplication>
#include <QFontDatabase>//设置全局字体头文件
#include <QTextCodec>

int main(int argc, char *argv[])
{
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf8"));
    QApplication a(argc, argv);
    QFont font;
    font.setFamily("微软雅黑");
    font.setPointSize(10);
    a.setFont(font);
    Widget w;
    w.show();

    return a.exec();
}
