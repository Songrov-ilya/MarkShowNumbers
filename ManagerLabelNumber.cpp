#include "ManagerLabelNumber.h"

void ManagerLabelNumber::refreshLabel()
{
    if(labelNumbers){
        currentPosition = labelNumbers->pos();
        labelNumbers->disconnect(labelNumbers, &LabelNumber::changeOpacity,
                                 labelNumbers, &LabelNumber::slotSetOpacity);
        labelNumbers->hide();
        labelNumbers->deleteLater();
    }

    labelNumbers = new LabelNumber(currentPosition);
    connect(labelNumbers, &LabelNumber::positionLabelChanged, this, &ManagerLabelNumber::slotPositionLabelChanged);
    QPixmap pix {QPixmap(currentPathImage).scaled(sizeLabel)};
    labelNumbers->setPixmap(pix);
    labelNumbers->resize(pix.size());
    labelNumbers->setMask(pix.mask());
}


ManagerLabelNumber::ManagerLabelNumber(QWidget *parent) :
    parent(parent),
    labelNumbers(nullptr)
{
    currentPosition = getStartPosition();
    sizeLabel = QSize(234, 234);
    originalSize = true;
}

ManagerLabelNumber::~ManagerLabelNumber()
{
    delete labelNumbers;
}

void ManagerLabelNumber::showLabel(bool animation)
{
    changeImage();
    labelNumbers->show();
    if(animation){
        labelNumbers->startAnimation();
    }
}

void ManagerLabelNumber::hideLabel()
{
    if(labelNumbers){
        labelNumbers->hide();
    }
}

void ManagerLabelNumber::changeImage()
{
    if(originalSize){
        this->sizeLabel = QPixmap(currentPathImage).size();
    }
    refreshLabel();
}

void ManagerLabelNumber::changeSizeImage(QSize sizeImage)
{
    this->sizeLabel = sizeImage;
    refreshLabel();
}

void ManagerLabelNumber::changePosLabel(QPoint pos)
{
    currentPosition = pos;
    if(labelNumbers){
        labelNumbers->move(pos);
    }
}

void ManagerLabelNumber::clearListImages()
{
    listImage.clear();
}

void ManagerLabelNumber::setListImage(QStringList listImage)
{
    this->listImage.append(listImage);
}

void ManagerLabelNumber::setSizeLabel(QSize sizeImage)
{
    this->sizeLabel = sizeImage;
}

void ManagerLabelNumber::setLabelPosition(QPoint startPosition)
{
    currentPosition = startPosition;
}

void ManagerLabelNumber::setCurentPathImage(QString strImage)
{
    this->currentPathImage = strImage;
}

void ManagerLabelNumber::setOriginalSize(bool original)
{
    this->originalSize = original;
}

QStringList ManagerLabelNumber::getListImage()
{
    return listImage;
}

QSize ManagerLabelNumber::getSizeLabel()
{
    return sizeLabel;
}

QPoint ManagerLabelNumber::getLabelPos()
{
    return currentPosition;
}

QPoint ManagerLabelNumber::getStartPosition()
{
    QPoint point;
    for (QScreen *screen : QGuiApplication::screens()){
        point.setX(screen->size().width() - 250);
        point.setY(screen->size().height() - 300 );
    }
    return point;
}

void ManagerLabelNumber::slotPositionLabelChanged(QPoint pos)
{
    currentPosition = pos;
}
