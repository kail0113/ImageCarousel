#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QResizeEvent>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>

#include "imagepage.h"
#include "carouselarrow.h"

class ImageView : public QWidget
{
    Q_OBJECT
public:
    enum CarouselPageType
    {
        LeftPage,
        RightPage
    };
    enum MovingDirection
    {
        MovingLeft,
        MovingRight
    };
    explicit ImageView(QWidget *parent = nullptr);

    void setLeftImagePage(QPixmap pixmap);
    void setRightImagePage(QPixmap pixmap);
    void setCenterImagePage(QPixmap pixmap);
    void setBackImagePage(QPixmap pixmap);
    void setAnimationPixmap(QList<QPixmap>);
    void startAnimation(MovingDirection direction, int step = 1);
signals:
    void pageClicked(ImageView::CarouselPageType);

public slots:
    void movingAnimationEnd();
    void animationShrinkChangeImage(QVariant);
    void pageClicked();
    void arrowClicked();
protected:
    void resizeEvent(QResizeEvent *event);
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void setArrowHidden(bool hidden);

private:
    CarouselArrow *m_leftArrow;
    CarouselArrow *m_rightArrow;
    ImagePage   *m_leftPage;
    ImagePage   *m_centerPage;
    ImagePage   *m_rightPage;
    ImagePage   *m_backPage;

    QRect       m_leftRect;
    QRect       m_centerRect;
    QRect       m_rightRect;
    QRect       m_backRect;

    bool        m_bAnimationPlaying;
    MovingDirection m_direction;

    QList<QPixmap > m_pixmaps;

    QParallelAnimationGroup *m_animationGroupShrink;
    QParallelAnimationGroup *m_animationGroupOpacity;
};

#endif // IMAGEVIEW_H
