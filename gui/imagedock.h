#ifndef IMAGEDOCK_H
#define IMAGEDOCK_H

#include <QWidget>
#include <QDockWidget>
#include <QImage>

class QGridLayout;
class FlowLayout;

class ImageDock : public QDockWidget
{
    Q_OBJECT
public:
    ImageDock(const QString& name, QWidget *parent=0);

    void addImageList(QList<QImage>& imgList);
    void addImage(QImage const& img);
    void deleteImages();

signals:
    void imageClicked(QImage const& img);

public slots:
    void onLabelClick(const QImage& img);

private:
    QWidget* m_container;
    FlowLayout* m_layoutContainer;
};

#endif // IMAGEDOCK_H
