#include "imageclickable.h"

ImageClickable::ImageClickable(QPixmap* img, QWidget* parent)
    : QLabel(parent)
{
    image = img;
    setPixmap(*img);
    setMargin(5);
}

void ImageClickable::mousePressEvent(QMouseEvent* event)
{
    emit clicked(image);
}
