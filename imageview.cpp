#include "imageview.h"

#include <QDebug>
#include <QTimer>

#define AnimationDuration 300

ImageView::ImageView(QWidget *parent) : QWidget(parent)
{
    m_centerRect = QRect(width() / 6, 0, width() / 3 * 2, height());
    m_leftRect = QRect(0, 12, width() / 3 * 2 - 12, height() - 12);
    m_rightRect = QRect(width() / 3 + 12, 12, width() / 3 * 2 - 12, height() - 12);
    m_backRect = QRect(width() / 6 + 6, 12, width() / 3 * 2 - 12, height() - 12);

    m_leftPage = new ImagePage(this);
    m_centerPage = new ImagePage(this);
    m_rightPage = new ImagePage(this);
    m_backPage = new ImagePage(this);

    m_leftPage->setGeometry(m_leftRect);
    m_centerPage->setGeometry(m_centerRect);
    m_rightPage->setGeometry(m_rightRect);
    m_backPage->setGeometry(m_backRect);

    m_backPage->lower();
    m_centerPage->raise();
    m_centerPage->setActive(true);

    connect(m_leftPage, SIGNAL(clicked()), this, SLOT(pageClicked()));
    connect(m_rightPage, SIGNAL(clicked()), this, SLOT(pageClicked()));
    // 需要连接点击信号，动画切换后，对应的窗口会发生变化
    connect(m_centerPage, SIGNAL(clicked()), this, SLOT(pageClicked()));
    connect(m_backPage, SIGNAL(clicked()), this, SLOT(pageClicked()));

    m_leftArrow = new CarouselArrow(CarouselArrow::Left, this);
    m_rightArrow = new CarouselArrow(CarouselArrow::Right, this);
    setArrowHidden(true);
    connect(m_leftArrow, SIGNAL(clicked()), this, SLOT(arrowClicked()));
    connect(m_rightArrow, SIGNAL(clicked()), this, SLOT(arrowClicked()));

}

void ImageView::setLeftImagePage(QPixmap pixmap)
{
    m_leftPage->setPixmap(pixmap);
}

void ImageView::setRightImagePage(QPixmap pixmap)
{
    m_rightPage->setPixmap(pixmap);
}

void ImageView::setCenterImagePage(QPixmap pixmap)
{
    m_centerPage->setPixmap(pixmap);
}

void ImageView::setBackImagePage(QPixmap pixmap)
{
    m_backPage->setPixmap(pixmap);
}

void ImageView::setAnimationPixmap(QList<QPixmap> pixmaps)
{
    m_pixmaps = pixmaps;
}

/**
 * @brief ImageView::startAnimation
 *
 * 图片轮播切换时的动画效果
 * @param direction 图片切换的方向:左移或是右移
 * @param step      图片切换的步距：目标页面与当前页面索引差值，
 *                  判断是使用移动的动画还是缩放的动画
 */
void ImageView::startAnimation(MovingDirection direction, int step)
{
    m_bAnimationPlaying = true;
    m_direction = direction;
    //左右移动切换， 设置相关动画效果
    if(step == 1)
    {
        QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup(this);
        connect(animationGroup, SIGNAL(finished()), this, SLOT(movingAnimationEnd()));
        QPropertyAnimation *leftPageAnimation = new QPropertyAnimation(m_leftPage, "geometry", animationGroup);
        QPropertyAnimation *rightPageAnimation = new QPropertyAnimation(m_rightPage, "geometry", animationGroup);
        QPropertyAnimation *centerPageAnimation = new QPropertyAnimation(m_centerPage, "geometry", animationGroup);
        QPropertyAnimation *tempPageAnimation = new QPropertyAnimation(m_backPage, "geometry", animationGroup);
        leftPageAnimation->setDuration(AnimationDuration);
        leftPageAnimation->setEasingCurve(QEasingCurve::Linear);
        rightPageAnimation->setDuration(AnimationDuration);
        rightPageAnimation->setEasingCurve(QEasingCurve::Linear);
        centerPageAnimation->setDuration(AnimationDuration);
        centerPageAnimation->setEasingCurve(QEasingCurve::Linear);
        tempPageAnimation->setDuration(AnimationDuration);
        tempPageAnimation->setEasingCurve(QEasingCurve::Linear);

        QPropertyAnimation *AlphaLightAnimation = new QPropertyAnimation(animationGroup);
        AlphaLightAnimation->setPropertyName("maskAlpha");
        AlphaLightAnimation->setDuration(AnimationDuration);
        AlphaLightAnimation->setEasingCurve(QEasingCurve::Linear);
        AlphaLightAnimation->setEndValue(0);

        QPropertyAnimation *AlphaDarkAnimation = new QPropertyAnimation(m_centerPage, "maskAlpha", animationGroup);
        AlphaDarkAnimation->setDuration(AnimationDuration);
        AlphaDarkAnimation->setEasingCurve(QEasingCurve::Linear);
        AlphaDarkAnimation->setEndValue(100);

        if(direction == MovingLeft)
        {
            leftPageAnimation->setEndValue(m_backRect);
            rightPageAnimation->setEndValue(m_centerRect);
            centerPageAnimation->setEndValue(m_leftRect);
            tempPageAnimation->setEndValue(m_rightRect);
            AlphaLightAnimation->setTargetObject(m_rightPage);
            m_rightPage->raise();
            m_leftPage->lower();
        }
        else if(direction == MovingRight)
        {
            leftPageAnimation->setEndValue(m_centerRect);
            rightPageAnimation->setEndValue(m_backRect);
            centerPageAnimation->setEndValue(m_rightRect);
            tempPageAnimation->setEndValue(m_leftRect);
            AlphaLightAnimation->setTargetObject(m_leftPage);
            m_leftPage->raise();
            m_rightPage->lower();
        }
        animationGroup->start(QAbstractAnimation::DeleteWhenStopped);
    }
    else    /**< 设置缩放动画效果 */
    {
        m_animationGroupShrink = new QParallelAnimationGroup(this);
        //缩放并中心移动动画
        QPropertyAnimation *leftPageAnimation = new QPropertyAnimation(m_leftPage, "geometry", m_animationGroupShrink);
        leftPageAnimation->setProperty("ImageChanged", QVariant(false));
        connect(leftPageAnimation, SIGNAL(valueChanged(QVariant)), this, SLOT(animationShrinkChangeImage(QVariant)));

        QPropertyAnimation *rightPageAnimation = new QPropertyAnimation(m_rightPage, "geometry", m_animationGroupShrink);
        QPropertyAnimation *centerPageAnimation = new QPropertyAnimation(m_centerPage, "geometry", m_animationGroupShrink);
        QPropertyAnimation *backPageAnimation = new QPropertyAnimation(m_backPage, "geometry", m_animationGroupShrink);

        leftPageAnimation->setDuration(AnimationDuration);
        leftPageAnimation->setEasingCurve(QEasingCurve::Linear);
        rightPageAnimation->setDuration(AnimationDuration);
        rightPageAnimation->setEasingCurve(QEasingCurve::Linear);
        centerPageAnimation->setDuration(AnimationDuration);
        centerPageAnimation->setEasingCurve(QEasingCurve::Linear);
        backPageAnimation->setDuration(AnimationDuration);
        backPageAnimation->setEasingCurve(QEasingCurve::Linear);

        int distance = 10;
        QRect rect = m_centerRect.adjusted(distance, distance, -distance, -distance / 3);
        leftPageAnimation->setKeyValueAt(0, m_leftRect);
        leftPageAnimation->setKeyValueAt(0.5, rect);
        leftPageAnimation->setKeyValueAt(1, m_leftRect);
        rightPageAnimation->setKeyValueAt(0, m_rightRect);
        rightPageAnimation->setKeyValueAt(0.5, rect);
        rightPageAnimation->setKeyValueAt(1, m_rightRect);
        centerPageAnimation->setKeyValueAt(0, m_centerRect);
        centerPageAnimation->setKeyValueAt(0.5, rect);
        centerPageAnimation->setKeyValueAt(1, m_centerRect);
        backPageAnimation->setKeyValueAt(0, m_backRect);
        backPageAnimation->setKeyValueAt(0.5, rect);
        backPageAnimation->setKeyValueAt(1, m_backRect);

        //明暗变化动画
        QPropertyAnimation *leftAlphaAnimation = new QPropertyAnimation(m_leftPage, "maskAlpha", m_animationGroupShrink);
        QPropertyAnimation *rightAlphaAnimation = new QPropertyAnimation(m_rightPage, "maskAlpha", m_animationGroupShrink);
        leftAlphaAnimation->setDuration(AnimationDuration);
        leftAlphaAnimation->setEasingCurve(QEasingCurve::Linear);
        rightAlphaAnimation->setDuration(AnimationDuration);
        rightAlphaAnimation->setEasingCurve(QEasingCurve::Linear);
        leftAlphaAnimation->setKeyValueAt(0, 100);
        leftAlphaAnimation->setKeyValueAt(0.5, 0);
        leftAlphaAnimation->setKeyValueAt(1, 100);
        rightAlphaAnimation->setKeyValueAt(0, 100);
        rightAlphaAnimation->setKeyValueAt(0.5, 0);
        rightAlphaAnimation->setKeyValueAt(1, 100);

        m_animationGroupShrink->start();
    }
}

void ImageView::movingAnimationEnd()
{
    ImagePage *tmpPage = 0;
    tmpPage = m_backPage;
    if(m_direction == MovingLeft)
    {
        m_backPage = m_leftPage;
        m_leftPage = m_centerPage;
        m_centerPage = m_rightPage;
        m_rightPage = tmpPage;
    }
    else if(m_direction == MovingRight)
    {
        m_backPage = m_rightPage;
        m_rightPage = m_centerPage;
        m_centerPage = m_leftPage;
        m_leftPage = tmpPage;
    }
}

void ImageView::animationShrinkChangeImage(QVariant var)
{
    Q_UNUSED(var)
    QPropertyAnimation *animation = qobject_cast<QPropertyAnimation *>(sender());
    if(animation->currentTime() > AnimationDuration / 2 && !animation->property("ImageChanged").toBool())
    {
        m_leftPage->setPixmap(m_pixmaps.at(0));
        m_centerPage->setPixmap(m_pixmaps.at(1));
        m_rightPage->setPixmap(m_pixmaps.at(2));
        animation->setProperty("ImageChanged", QVariant(true));
    }
}

void ImageView::pageClicked()
{
    ImagePage *page = qobject_cast<ImagePage*>(sender());

    // 此处direction表示点击的是左侧页面还是右侧页面
    CarouselPageType pageType;
    if(page == m_leftPage)
    {
        pageType = LeftPage;
    }
    else if(page == m_rightPage)
    {
        pageType = RightPage;
    }
    else
    {
        return;
    }
    emit pageClicked(pageType);
}

/**
 * @brief ImageView::arrowClicked
 *
 * 效果等同与左右侧ImagePage点击效果
 * 点击左侧箭头，则动画效果为向右移动(参考网易云)
 * 点击右侧教头，则动画效果为向左移动(参考网易云)
 */
void ImageView::arrowClicked()
{
    CarouselArrow *arrow = qobject_cast<CarouselArrow *>(sender());
    CarouselPageType pageType;
    if(arrow == m_leftArrow)
    {
        pageType = LeftPage;
    }
    else if(arrow = m_rightArrow)
    {
        pageType = RightPage;
    }
    emit pageClicked(pageType);
}

void ImageView::setArrowHidden(bool hidden)
{
    //TODO
    m_leftArrow->setHidden(hidden);
    m_rightArrow->setHidden(hidden);
    if(!hidden){
        m_leftArrow->move(m_leftPage->x()+8, m_leftPage->y()+(m_leftPage->height()-m_leftArrow->height())/2);
        m_rightArrow->move(m_rightPage->x()+m_rightPage->width()-m_rightArrow->width()-8,
                           m_rightPage->y()+(m_rightPage->height()-m_rightArrow->height())/2);
        m_leftArrow->raise();
        m_rightArrow->raise();
    }
}

void ImageView::resizeEvent(QResizeEvent *event)
{
    m_centerRect = QRect(width() / 6, 0, width() / 3 * 2, height());
    m_leftRect = QRect(0, 12, width() / 3 * 2 - 12, height() - 12);
    m_rightRect = QRect(width() / 3 + 12, 12, width() / 3 * 2 - 12, height() - 12);
    m_backRect = QRect(width() / 6 + 6, 12, width() / 3 * 2 - 12, height() - 12);

    m_leftPage->setGeometry(m_leftRect);
    m_centerPage->setGeometry(m_centerRect);
    m_rightPage->setGeometry(m_rightRect);
    m_backPage->setGeometry(m_backRect);
}

void ImageView::enterEvent(QEvent *event)
{
    Q_UNUSED(event);
    setArrowHidden(false);
}

void ImageView::leaveEvent(QEvent *event)
{
    Q_UNUSED(event);
    setArrowHidden(true);
}
