#ifndef IMAGECLICKABLE_H
#define IMAGECLICKABLE_H

#include <QLabel>
#include <QImage>

class ImageClickable : public QLabel
{
    Q_OBJECT

public:
    ImageClickable(QImage* img, QWidget* parent = 0);

signals:
    void clicked(QImage* img);

protected:
    void mousePressEvent(QMouseEvent* event);

private:
    QImage* image;
    QPixmap miniature;

};

#endif // IMAGECLICKABLE_H
