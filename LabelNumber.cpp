#include "LabelNumber.h"


LabelNumber::LabelNumber(QPoint startPosition, QWidget *parent) :
    QLabel(parent)
{
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);
    setFocusPolicy(Qt::NoFocus);
    move(startPosition);
    connect(this, &LabelNumber::changeOpacity, this, &LabelNumber::slotSetOpacity);
}

void LabelNumber::startAnimation()
{

    qreal opacity { 0.01 };
    for (int var = 0; var < 100; ++var){
        opacity = opacity + 0.01;
        emit changeOpacity(opacity);
    }
}


void LabelNumber::mousePressEvent(QMouseEvent *pe)
{
    m_ptPosition = pe->pos();
}

void LabelNumber::mouseMoveEvent(QMouseEvent *pe)
{
    move(pe->globalPos() - m_ptPosition);
    emit positionLabelChanged(this->pos());
}

void LabelNumber::slotSetOpacity(qreal opacity)
{
    QTime time;
    time.start();
    for (;;){
        if (time.elapsed() > 40){
                setWindowOpacity(opacity);
                QApplication::processEvents(QEventLoop::DialogExec);
                return;
        }
    }
}

