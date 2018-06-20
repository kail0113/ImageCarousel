#include "carouselindicator.h"

#include <QPainter>

CarouselIndicator::CarouselIndicator(QWidget *parent)
    : QWidget(parent)
{
    m_selectedColor = QColor(220, 0, 0);
    m_unselectedColor = QColor(200,200,200);
    m_selected = false;
}

CarouselIndicator::~CarouselIndicator()
{

}

void CarouselIndicator::select(bool selected)
{
    if(m_selected != selected)
    {
        m_selected = selected;
        update();
    }
}

void CarouselIndicator::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    if(m_selected){
        painter.setBrush(QBrush(m_selectedColor));
    }else{
        painter.setBrush(QBrush(m_unselectedColor));
    }

    painter.drawRect(rect());
}

void CarouselIndicator::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    emit entered();
}
