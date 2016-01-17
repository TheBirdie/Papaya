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

    void addImage(QImage const& img, const QString& path);
    void deleteImages();

signals:
    void imageClicked(QImage const& img, QString const& filename);

public slots:
    void onLabelClick(const QImage& img, QString const& filename);

private:
    QWidget* m_container;
    FlowLayout* m_layoutContainer;
};

#endif // IMAGEDOCK_H
