#include "imagedock.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>

ImageDock::ImageDock(const QString& name, QWidget* parent)
    : QDockWidget(name, parent)
{
    // Dock options
    setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);

    // Scrollbar construction
    QScrollArea* scroll = new QScrollArea(this);
    scroll->setWidgetResizable(true);
    this->setWidget(scroll);

    // Inside of the scrollbar
    container = new QWidget(scroll);
    QVBoxLayout* layoutContainer = new QVBoxLayout(container);
    container->setLayout(layoutContainer);
    scroll->setWidget(container);

}

void ImageDock::addImageList(const QList<QPixmap>& imgList){

    // Add images to dock layout
    for(QList<QPixmap>::const_iterator img = imgList.begin(); img != imgList.end(); img++){
        QLabel* w = new QLabel(this);
        w->setPixmap(*img);
        w->setMargin(5);
        container->layout()->addWidget(w);
    }
}

void ImageDock::deleteImages(){
    qDeleteAll(container->findChildren<QLabel*>());
}
