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

void ImageDock::addImage(QImage const& img)
{
    ImageClickable* w = new ImageClickable(img, this);
    container->layout()->addWidget(w);

    // Connect the label to a click event
    QObject::connect(w, SIGNAL(clicked(QImage const&)), this, SLOT(onLabelClick(QImage const&)));
}

void ImageDock::addImageList(QList<QImage>& imgList){

    // Add images to dock layout
    for (QList<QImage>::iterator img = imgList.begin(); img != imgList.end(); img++)
        addImage(*img);
}

void ImageDock::deleteImages(){
    qDeleteAll(container->findChildren<ImageClickable*>());
}

void ImageDock::onLabelClick(QImage const& img){
    emit imageClicked(img);
}
