#include "imagecarousel.h"
#include <QDebug>


ImageCarouselPrivate::ImageCarouselPrivate()
{
    q = 0;
    m_indHLayout = 0;
    m_step = 0;
    m_currentIndex = -1;
    m_indicators.clear();
}

ImageCarouselPrivate::~ImageCarouselPrivate()
{

}

void ImageCarouselPrivate::init()
{
    m_indHLayout = new QHBoxLayout();
    m_indHLayout->setSpacing(5);

    m_view = new ImageView();
    connect(m_view, SIGNAL(pageClicked(ImageView::CarouselPageType)), this, SLOT(switchPage(ImageView::CarouselPageType)));

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->setSpacing(0);
    bottomLayout->setMargin(0);
    bottomLayout->setContentsMargins(0, 0, 0, 0);
    bottomLayout->addStretch();
    bottomLayout->addLayout(m_indHLayout);
    bottomLayout->addStretch();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(m_view);
    layout->addLayout(bottomLayout);
    q->setLayout(layout);
}

void ImageCarouselPrivate::addPage(QPixmap pixmap)
{
    m_pixmaps.append(pixmap);

    CarouselIndicator *ind = new CarouselIndicator();
    ind->setFixedSize(18, 3);
    connect(ind, SIGNAL(entered()), this, SLOT(switchIndicator()));
    m_indicators.append(ind);
    m_indHLayout->addWidget(ind);
}

void ImageCarouselPrivate::updateImagePage()
{
    if(m_currentIndex == -1){
        m_currentIndex = 0;
        m_indicators.at(m_currentIndex)->select(true);
    }
    updateImagePage(m_currentIndex);
}

/**
 * @brief ImageCarouselPrivate::updateImagePage
 *
 * 根据index显示图片
 * @param index  中间的ImagePage的索引值
 */
void ImageCarouselPrivate::updateImagePage(int index)
{
    if(index < 0) return;
    int count = m_indicators.count();
    int leftIndex = 0, rightIndex = 0;
    if(count > 1){
        if(index == 0){
            leftIndex = count - 1;
            rightIndex = index + 1;
        }else if(index == count -1){
            leftIndex = index - 1;
            rightIndex = 0;
        }else{
            leftIndex = index - 1;
            rightIndex = index + 1;
        }
    }
    m_view->setLeftImagePage(m_pixmaps.at(leftIndex));
    m_view->setCenterImagePage(m_pixmaps.at(index));
    m_view->setRightImagePage(m_pixmaps.at(rightIndex));
    m_indicators.at(index)->select(true);
}

/**
 * @brief ImageCarouselPrivate::calcIndex
 *
 * 根据center的值和左右移动的效果，计算左右两侧以及背后的ImagePage
 * 需要显示图片的索引，step为1时需要计算左右以及背后的ImagePage图片的索引
 * step大于1时，只需确定左右ImagePage的图片索引即可
 * @param [in]center
 * @param [out]leftIndex
 * @param [out]rightIndex
 * @param [out]backIndex
 */
void ImageCarouselPrivate::calcIndex(int center, int &leftIndex, int &rightIndex, int &backIndex)
{
    int count = m_indicators.count();
    if(count > 1){
        if(center == 0)
        {
            leftIndex = count - 1;
            rightIndex = center + 1;
        }
        else if(center == count -1)
        {
            leftIndex = center - 1;
            rightIndex = 0;
        }
        else
        {
            leftIndex = center - 1;
            rightIndex = center + 1;
        }
    }

    if(m_direction == ImageView::MovingLeft)
    {
        if(count == 2)
        {
            backIndex = center;
        }
        else if(count == 3)
        {
            backIndex = rightIndex;
        }
        else
        {
            if(rightIndex != count-1)
                backIndex = rightIndex + 1;
            else
                backIndex = 0;
        }
    }
    else if(m_direction == ImageView::MovingRight)
    {
        if(count == 2)
        {
            backIndex = center;
        }
        else if(count == 3)
        {
            backIndex = rightIndex;
        }
        else
        {
            if(leftIndex != 0)
                backIndex = leftIndex - 1;
            else
                backIndex = count - 1;
        }
    }
}

/**
 * @brief ImageCarouselPrivate::prepareAnimation
 *
 * step==1时   移动切换 根据当前显示图片信息计算backImage需显示的图片索引值
 * step>=2时   缩放切换 计算出index为中心图像时，左右两侧图片索引，取得图像传入ImageView
 * @param index 切换后显示中间的图片索引
 */
void ImageCarouselPrivate::prepareAnimation(int index)
{
    if(m_step == 1)
    {
        int leftIndex = 0, rightIndex = 0, backIndex = 0;
        calcIndex(m_currentIndex, leftIndex, rightIndex, backIndex);
        m_view->setLeftImagePage(m_pixmaps.at(leftIndex));
        m_view->setCenterImagePage(m_pixmaps.at(m_currentIndex));
        m_view->setRightImagePage(m_pixmaps.at(rightIndex));
        m_view->setBackImagePage(m_pixmaps.at(backIndex));
    }
    else
    {
        int leftIndex = 0, rightIndex = 0, backIndex = 0;
        calcIndex(index, leftIndex, rightIndex, backIndex);
        QList<QPixmap> list;
        list<<m_pixmaps.at(leftIndex)
            <<m_pixmaps.at(index)
            <<m_pixmaps.at(rightIndex);
        m_view->setAnimationPixmap(list);
    }
}

void ImageCarouselPrivate::switchIndicator()
{
    CarouselIndicator *ind = qobject_cast<CarouselIndicator*>(sender());
    if(ind == 0) return;
    int index = m_indicators.indexOf(ind);
    if(index == m_currentIndex) return;

    if(m_currentIndex != -1){
        m_indicators.at(m_currentIndex)->select(false);
    }

    if(index > m_currentIndex)
    {
        m_direction = ImageView::MovingLeft;
    }
    else if(index < m_currentIndex)
    {
        m_direction = ImageView::MovingRight;
    }

    m_step = qAbs(index - m_currentIndex);

    prepareAnimation(index);
    m_currentIndex = index;

    m_indicators.at(index)->select(true);
    m_view->startAnimation(m_direction, m_step);
}

/**
 * @brief ImageCarouselPrivate::switchPage
 * @param [in]pageType   表示点击的是那一侧的ImagePage  LeftPage--左侧  RightPage--右侧
 */
void ImageCarouselPrivate::switchPage(ImageView::CarouselPageType pageType)
{
    if(m_currentIndex != -1){
        m_indicators.at(m_currentIndex)->select(false);
    }
    m_step = 1;
    int tarIndex;
    if(pageType == ImageView::LeftPage)
    {
        //点击左侧窗口， 动画效果右移
        m_direction = ImageView::MovingRight;
        tarIndex = m_currentIndex - 1;
        tarIndex = tarIndex < 0 ? m_pixmaps.count() - 1 : tarIndex;

    }
    else if(pageType == ImageView::RightPage)
    {
        m_direction = ImageView::MovingLeft;
        tarIndex = m_currentIndex + 1;
        tarIndex = tarIndex > m_pixmaps.count() - 1 ? 0 : tarIndex;
    }
    prepareAnimation(tarIndex);
    m_currentIndex = tarIndex;
    m_indicators.at(m_currentIndex)->select(true);
    m_view->startAnimation(m_direction, m_step);
}


ImageCarousel::ImageCarousel(QWidget *parent)
    : QWidget(parent), d(new ImageCarouselPrivate())
{
    d->q = this;
    d->init();
}

ImageCarousel::~ImageCarousel()
{

}

void ImageCarousel::addPage(QPixmap pixmap)
{
    d->addPage(pixmap);
}

void ImageCarousel::updateImageView()
{
    d->updateImagePage();
}
