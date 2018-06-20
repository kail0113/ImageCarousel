#ifndef IMAGEPAGE_H
#define IMAGEPAGE_H

#include <QWidget>

class ImagePage : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(int maskAlpha READ maskAlpha WRITE setMaskAlpha)
public:
    explicit ImagePage(QWidget *parent = nullptr);
    ~ImagePage();
    void setActive(bool active);
    void setPixmap(QPixmap pixmap);

    int maskAlpha() const;
    void setMaskAlpha(const int &);
signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);
    void resizeEvent(QResizeEvent *event);

private:
    QPixmap m_pixmap;
    int m_maskAlpha;
};

#endif // IMAGEPAGE_H
