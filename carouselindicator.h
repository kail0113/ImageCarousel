#ifndef CAROUSELINDICATOR_H
#define CAROUSELINDICATOR_H

#include <QWidget>

class CarouselIndicator : public QWidget
{
    Q_OBJECT
public:
    explicit CarouselIndicator(QWidget *parent = nullptr);
    ~CarouselIndicator();

    void select(bool selected);
signals:
    void entered();

protected:
    void paintEvent(QPaintEvent *event);
    void enterEvent(QEvent *event);

private:
    QColor m_selectedColor;
    QColor m_unselectedColor;
    bool m_selected;
};

#endif // CAROUSELINDICATOR_H
