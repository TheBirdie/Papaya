#include "imagedock.h"

#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

#include "flowlayout.h"
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
    m_container = new QWidget(scroll);
    m_layoutContainer = new FlowLayout(m_container);
    m_container->setLayout(m_layoutContainer);
    scroll->setWidget(m_container);

}

void ImageDock::addImage(QImage const& img, QString const& filename)
{
    ImageClickable* w = new ImageClickable(img, filename, this);
    m_layoutContainer->addWidget(w);

    // Connect the label to a click event
    QObject::connect(w, SIGNAL(clicked(QImage const&, QString const&)), this, SLOT(onLabelClick(QImage const&, QString const&)));
}

// Empty the dock of previously loaded images
void ImageDock::deleteImages(){
    qDeleteAll(m_container->findChildren<ImageClickable*>());
}

// Redirect signal with additional parameter image and filename
void ImageDock::onLabelClick(QImage const& img, QString const& filename){
    emit imageClicked(img, filename);
}
