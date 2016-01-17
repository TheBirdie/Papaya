#ifndef IMAGECLICKABLE_H
#define IMAGECLICKABLE_H

#include <QLabel>
#include <QString>
#include <QImage>

class ImageClickable : public QLabel
{
    Q_OBJECT

public:
    ImageClickable(QImage const& img, QString const& filename, QWidget* parent = 0);

signals:
    void clicked(QImage const& img, QString const& filename);

protected:
    void mousePressEvent(QMouseEvent* event);

private:
    QImage const& image;
    QString const& filename;
    QPixmap miniature;

};

#endif // IMAGECLICKABLE_H
