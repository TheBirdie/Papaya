#ifndef IMAGECLICKABLE_H
#define IMAGECLICKABLE_H

#include <QLabel>
#include <QImage>

class ImageClickable : public QLabel
{
    Q_OBJECT

public:
    ImageClickable(QImage const& img, QWidget* parent = 0);

signals:
    void clicked(QImage const& img);

protected:
    void mousePressEvent(QMouseEvent* event);

private:
    QImage const& image;
    QPixmap miniature;

};

#endif // IMAGECLICKABLE_H
