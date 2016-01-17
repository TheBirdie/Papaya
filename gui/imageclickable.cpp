#include "imageclickable.h"

ImageClickable::ImageClickable(QImage const& img, QString const& filename, QWidget* parent)
    : QLabel(parent), image(img), filename(filename)
{
    miniature = QPixmap::fromImage(img);

    setPixmap(miniature);
    setMargin(5);
}

// Emit signal on click
void ImageClickable::mousePressEvent(QMouseEvent* event)
{
    emit clicked(image, filename);
}
