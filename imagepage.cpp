#include "imagepage.h"

#include <QMouseEvent>
#include <QPainter>
#include <QDebug>

ImagePage::ImagePage(QWidget *parent)
    : QWidget(parent)
{
    m_maskAlpha = 0;
}

ImagePage::~ImagePage()
{

}

void ImagePage::setActive(bool active)
{
    if(active)
    {
        m_maskAlpha = 0;
    }
    else
    {
        m_maskAlpha = 100;
    }
    update();
}

void ImagePage::setPixmap(QPixmap pixmap)
{
    m_pixmap = pixmap.scaled(size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    update();
}

int ImagePage::maskAlpha() const
{
    return m_maskAlpha;
}

void ImagePage::setMaskAlpha(const int &alpha)
{
    m_maskAlpha = alpha;
    update();
}

void ImagePage::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
}

void ImagePage::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    if(!m_pixmap.isNull())
        painter.drawPixmap(0, 0, m_pixmap);

    painter.setBrush(QColor(10,10,10, m_maskAlpha));
    painter.drawRect(rect());
}

void ImagePage::resizeEvent(QResizeEvent *event)
{
    if(!m_pixmap.isNull())
        m_pixmap = m_pixmap.scaled(event->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    update();
}

