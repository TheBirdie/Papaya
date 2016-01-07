#ifndef IMAGECLICKABLE_H
#define IMAGECLICKABLE_H

#include <QLabel>
#include <QPixmap>

class ImageClickable : public QLabel
{
    Q_OBJECT

public:
    ImageClickable(QPixmap* img, QWidget* parent = 0);

signals:
    void clicked(QPixmap* img);

protected:
    void mousePressEvent(QMouseEvent* event);

private:
    QPixmap* image;

};

#endif // IMAGECLICKABLE_H
