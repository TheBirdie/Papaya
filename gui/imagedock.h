#ifndef IMAGEDOCK_H
#define IMAGEDOCK_H

#include <QWidget>
#include <QDockWidget>
#include <QImage>

class ImageDock : public QDockWidget
{
    Q_OBJECT
public:
    ImageDock(const QString& name, QWidget *parent=0);

    void addImageList(QList<QImage>& imgList);
    void deleteImages();

signals:
    void imageClicked(QImage* img);

public slots:
    void onLabelClick(QImage* img);

private:
    QWidget* container;
};

#endif // IMAGEDOCK_H
