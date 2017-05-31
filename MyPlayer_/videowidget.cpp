#include "videowidget.h"

VideoWidget::VideoWidget(QWidget *parent)
    : QVideoWidget(parent)
{
    this->setMinimumSize(600,500);
    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::black);
    setPalette(p);

    setAttribute(Qt::WA_OpaquePaintEvent);
}

VideoWidget::~VideoWidget()
{

}

void VideoWidget::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Escape && isFullScreen()) {
            setFullScreen(false);
            event->accept();
        } else if (event->key() == Qt::Key_Enter && event->modifiers() & Qt::Key_Alt) {
            setFullScreen(!isFullScreen());
            event->accept();
        } else {
            QVideoWidget::keyPressEvent(event);
        }
}

void VideoWidget::mouseDoubleClickEvent(QMouseEvent *event)
{
    setFullScreen(!isFullScreen());
    event->accept();
}

void VideoWidget::mousePressEvent(QMouseEvent *event)
{
    QVideoWidget::mousePressEvent(event);
}

