#ifndef IMAGECAROUSEL_H
#define IMAGECAROUSEL_H

#include <QWidget>
#include <QHBoxLayout>
#include "carouselindicator.h"
#include "imageview.h"

class ImageCarousel;
class ImageCarouselPrivate : public QObject
{
    Q_OBJECT
public:
    ImageCarouselPrivate();
    ~ImageCarouselPrivate();

    void init();
    void addPage(QPixmap pixmap);

    void updateImagePage();
    void updateImagePage(int index);
    void prepareAnimation(int index);


public slots:
    void switchIndicator();
    void switchPage(ImageView::CarouselPageType);

protected:
    void calcIndex(int center, int &left, int &right, int &back);

public:
    ImageCarousel *q;
    QList<QPixmap> m_pixmaps;
    QHBoxLayout *m_indHLayout;
    QList<CarouselIndicator *> m_indicators;

    ImageView *m_view;
    int m_currentIndex;
    ImageView::MovingDirection m_direction;
    int m_step;
};

class ImageCarousel : public QWidget
{
    Q_OBJECT
public:
    explicit ImageCarousel(QWidget *parent = nullptr);
    ~ImageCarousel();

    void addPage(QPixmap pixmap);
    void updateImageView();
signals:

public slots:

private:
    ImageCarouselPrivate *d;
};

#endif // IMAGECAROUSEL_H
