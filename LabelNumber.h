#ifndef LABELNUMBER_H
#define LABELNUMBER_H

#include <QtWidgets>
#include <QtEvents>


class LabelNumber : public QLabel
{
    Q_OBJECT

    QPoint m_ptPosition;
public:
    explicit LabelNumber(QPoint startPosition, QWidget *parent = nullptr);
    void startAnimation();
protected:
    virtual void mousePressEvent(QMouseEvent* pe);
    virtual void  mouseMoveEvent(QMouseEvent* pe);

signals:
    void positionLabelChanged(QPoint);
    void changeOpacity(qreal);

public slots:
    void slotSetOpacity(qreal opacity);
};

#endif // LABELNUMBER_H
