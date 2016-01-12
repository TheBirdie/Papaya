#include "imageclickable.h"

ImageClickable::ImageClickable(QImage* img, QWidget* parent)
    : QLabel(parent)
{
    image = img;
    miniature = QPixmap::fromImage(img->scaled(150, 100));

    setPixmap(miniature);
    setMargin(5);
}

void ImageClickable::mousePressEvent(QMouseEvent* event)
{
    emit clicked(image);
}
