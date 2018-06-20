#include "carouselarrow.h"

#include <QPainter>

CarouselArrow::CarouselArrow(ArrowType type, QWidget *parent)
    : QWidget(parent)
{
    m_bMouseHover = false;
    m_bMousePress = false;
    m_type = type;
    setFixedSize(12,20);
}

CarouselArrow::~CarouselArrow()
{

}


void CarouselArrow::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_bMouseHover = true;
    update();
}

void CarouselArrow::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    m_bMouseHover = false;
    update();
}

void CarouselArrow::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bMousePress = true;
        emit clicked();
    }
}

void CarouselArrow::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton){
        m_bMousePress = false;
    }
}

void CarouselArrow::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    QPen pen;
    pen.setWidth(3);
    if(m_bMouseHover){
        pen.setBrush(QColor(255,255,255));
    }else{
        pen.setBrush(QColor(150,150,150));
    }
    painter.setPen(pen);
    int margin = 2;
    switch (m_type) {
    case Right:
    {
        painter.drawLine(QPointF(margin, margin), QPointF(this->width()-margin, this->height()/2));
        painter.drawLine(QPointF(margin, this->height()-margin), QPointF(this->width()-margin, this->height()/2));
    }break;
    case Left:
    {
        painter.drawLine(QPointF(this->width()-margin, margin), QPointF(margin, this->height()/2));
        painter.drawLine(QPointF(this->width()-margin, this->height()-margin), QPointF(margin, this->height()/2));
    }break;
    default:
        break;
    }
    QWidget::paintEvent(event);
}
