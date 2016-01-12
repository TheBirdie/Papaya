#ifndef IMAGEDOCK_H
#define IMAGEDOCK_H

#include <QWidget>
#include <QDockWidget>

class ImageDock : public QDockWidget
{
    Q_OBJECT
public:
    ImageDock(const QString& name, QWidget *parent=0);

    void addImageList(QList<QPixmap>& imgList);
    void deleteImages();

signals:
    void imageClicked(QPixmap* img);

public slots:
    void onLabelClick(QPixmap* img);

private:
    QWidget* container;
};

#endif // IMAGEDOCK_H
