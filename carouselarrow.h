#ifndef CAROUSELARROW_H
#define CAROUSELARROW_H

#include <QWidget>
#include <QMouseEvent>

class CarouselArrow : public QWidget
{
    Q_OBJECT
public:
    enum ArrowType{
        Left,
        Right
    };
    explicit CarouselArrow(ArrowType type, QWidget *parent = nullptr);
    ~CarouselArrow();
signals:
    void clicked();

public slots:

protected:
    void enterEvent(QEvent *event);
    void leaveEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    bool m_bMouseHover;
    bool m_bMousePress;
    ArrowType m_type;
};

#endif // CAROUSELARROW_H
