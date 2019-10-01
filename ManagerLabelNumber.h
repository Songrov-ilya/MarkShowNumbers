#ifndef MANAGERLABELNUMBER_H
#define MANAGERLABELNUMBER_H

#include "LabelNumber.h"
#include <QtWidgets>
#include <QObject>

class ManagerLabelNumber : public QObject
{
    Q_OBJECT

    QWidget *parent;
    LabelNumber *labelNumbers;
    QStringList listImage;
    QString currentPathImage;
    QSize sizeLabel;
    QPoint currentPosition;
    bool originalSize;

    void refreshLabel();
public:
    explicit ManagerLabelNumber(QWidget *parent);
    virtual ~ManagerLabelNumber();

    void showLabel(bool animation);
    void hideLabel();
    void changeImage();
    void changeSizeImage(QSize sizeImage);
    void changePosLabel(QPoint pos);
    void clearListImages();

    void setListImage(QStringList listImage);
    void setSizeLabel(QSize sizeLabel);
    void setLabelPosition(QPoint startPosition);
    void setCurentPathImage(QString strImage);
    void setOriginalSize(bool original);

    QStringList getListImage();
    QSize getSizeLabel();
    QSize getStartPoint();
    QPoint getLabelPos();
    QPoint getStartPosition();

public slots:
    void slotPositionLabelChanged(QPoint pos);

};

#endif // MANAGERLABELNUMBER_H
