#include "imagedock.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

#include "imageclickable.h"

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

void ImageDock::addImageList(QList<QPixmap>& imgList){

    // Add images to dock layout
    for(QList<QPixmap>::iterator img = imgList.begin(); img != imgList.end(); img++){
        ImageClickable* w = new ImageClickable(&(*img), this);
        container->layout()->addWidget(w);

        // Connect the label to a click event
        QObject::connect(w, SIGNAL(clicked(QPixmap*)), this, SLOT(onLabelClick(QPixmap*)));
    }
}

void ImageDock::deleteImages(){
    qDeleteAll(container->findChildren<ImageClickable*>());
}

void ImageDock::onLabelClick(QPixmap *img){
    emit imageClicked(img);
}
