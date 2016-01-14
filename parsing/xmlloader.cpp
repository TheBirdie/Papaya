#include <fstream>
#include <sstream>
#include <cassert>
#include <iostream>
#include <GL/glew.h>
//#include <GL/GL.h>
#include <QDebug>
#include <QFileInfo>
#include <QDir>
#include <QString>
#include <QImage>
#include <QtCore>
#include <QGLWidget>
#include "xmlloader.h"
#include "camera.h"

using namespace std;

Reconstruction::Reconstruction(const QString filename){
    _filename = filename;
}

void Reconstruction::read(QList<Camera>& photos) {
    QFile xmlFile(_filename);
    xmlFile.open(QIODevice::ReadOnly);
    xml.setDevice(&xmlFile);
    if(xml.readNextStartElement() && xml.name() == "ATExport"){
        processPhotos(photos);

    if(xml.tokenType() == QXmlStreamReader::Invalid)
        xml.readNext();

    if(xml.hasError()){
        xml.raiseError();
        qDebug() << errorString();
    }
    }
}

QString Reconstruction::readNextText(){
    xml.readNext();
    return xml.text().toString();
}

QString Reconstruction::errorString(){
    return QString("XML file has error.");
}

void Reconstruction::processPhotos(QList<Camera>& photos){
    if(!xml.isStartElement() || xml.name() != "ATExport")
        return;
    while(xml.readNextStartElement()){
        if(xml.name() == "Photo"){
            Camera photo;
            processPhoto(photo);
            photos.push_back(photo);
        }
        else
            xml.skipCurrentElement();
    }
}

void Reconstruction::processPhoto(Camera& photo){
    if(!xml.isStartElement() || xml.name() != "Photo")
        return;

    while(xml.readNextStartElement()){
        QStringRef name = xml.name();
        if(name == "ImagePath")
            processImagePath(photo);
        else if(name == "ImageDimensions")
            processImageDimensions(photo);
        else if(name == "FocalLength")
            processFocalLength(photo);
        else if(name == "CameraModelType")
            processCameraModelType(photo);
        else if(name == "AspectRatio")
            processAspectRatio(photo);
        else if(name == "Skew")
            processSkew(photo);
        else if(name == "PrincipalPoint")
            processPrincipalPoint(photo);
        else if(name == "Rotation")
            processRotation(photo);
        else if(name == "Center")
            processCenter(photo);
        else
            xml.skipCurrentElement();
    }
}

void Reconstruction::processImagePath(Camera &photo){
    if(!xml.isStartElement() || xml.name() != "ImagePath")
        return;
    photo.imagePath = readNextText();
    xml.skipCurrentElement();
}

void Reconstruction::processImageDimensions(Camera &photo){
    if(!xml.isStartElement() || xml.name() != "ImageDimensions")
        return;
    while(xml.readNextStartElement()){
        if(xml.name() == "Width")
            photo.width = readNextText().toInt();
        else if(xml.name() == "Height")
            photo.height = readNextText().toInt();

        xml.skipCurrentElement();
    }
}

void Reconstruction::processFocalLength(Camera &photo){
    if(!xml.isStartElement() || xml.name() != "FocalLength")
        return;
    photo.focalLength = readNextText().toDouble();
    xml.skipCurrentElement();
}

void Reconstruction::processCameraModelType(Camera &photo){
    if(!xml.isStartElement() || xml.name() != "CameraModelType")
        return;
    photo.modelType = readNextText();
    xml.skipCurrentElement();
}

void Reconstruction::processAspectRatio(Camera &photo){
    if(!xml.isStartElement() || xml.name() != "AspectRatio")
        return;
    photo.aspectRatio = readNextText().toDouble();
    xml.skipCurrentElement();
}

void Reconstruction::processSkew(Camera &photo){
    if(!xml.isStartElement() || xml.name() != "Skew")
        return;
    photo.skew = readNextText().toDouble();
    xml.skipCurrentElement();
}

void Reconstruction::processPrincipalPoint(Camera &photo){
    if(!xml.isStartElement() || xml.name() != "PrincipalPoint")
        return;
    while(xml.readNextStartElement()){
        if(xml.name() == "x")
            photo.principalPoint[0] = readNextText().toDouble();
        else if(xml.name() == "y")
            photo.principalPoint[1] = readNextText().toDouble();

        xml.skipCurrentElement();
    }
}

void Reconstruction::processRotation(Camera &photo){
    if(!xml.isStartElement() || xml.name() != "Rotation")
        return;
    while(xml.readNextStartElement()){
        if(xml.name() == "M_00")
            photo.rotation[0][0] = readNextText().toDouble();
        else if(xml.name() == "M_01")
            photo.rotation[0][1] = readNextText().toDouble();
        else if(xml.name() == "M_02")
            photo.rotation[0][2] = readNextText().toDouble();
        else if(xml.name() == "M_10")
            photo.rotation[1][0] = readNextText().toDouble();
        else if(xml.name() == "M_11")
            photo.rotation[1][1] = readNextText().toDouble();
        else if(xml.name() == "M_12")
            photo.rotation[1][2] = readNextText().toDouble();
        else if(xml.name() == "M_20")
            photo.rotation[2][0] = readNextText().toDouble();
        else if(xml.name() == "M_21")
            photo.rotation[2][1] = readNextText().toDouble();
        else if(xml.name() == "M_22")
            photo.rotation[2][2] = readNextText().toDouble();

        xml.skipCurrentElement();
    }
}

void Reconstruction::processCenter(Camera &photo){
    if(!xml.isStartElement() || xml.name() != "Center")
        return;
    while(xml.readNextStartElement()){
        if(xml.name() == "x")
            photo.center[0] = readNextText().toDouble();
        else if(xml.name() == "y")
            photo.center[1] = readNextText().toDouble();
        else if(xml.name() == "z")
            photo.center[2] = readNextText().toDouble();

        xml.skipCurrentElement();
    }
}
