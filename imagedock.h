#ifndef IMAGEDOCK_H
#define IMAGEDOCK_H

#include <QWidget>
#include <QDockWidget>

class ImageDock : public QDockWidget
{
public:
    ImageDock(const QString& name, QWidget *parent=0);

    void addImageList(const QList<QPixmap>& imgList);
    void deleteImages();

private:
    QWidget* container;
};

#endif // IMAGEDOCK_H
