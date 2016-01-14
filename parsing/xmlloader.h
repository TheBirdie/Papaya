#pragma once

#include <QImage>
#include <QFileInfo>
#include <QList>
#include <QXmlStreamReader>


class Reconstruction{
public:
    // Structure to hold one data entry from XML file
    struct Camera{
            QString imagePath;
            QImage image;
            int width;
            int height;
            double focalLength;
            QString modelType;
            double aspectRatio;
            double skew;
            double principalPoint[2];
            double rotation[3][3];
            double center[3];
    };

    Reconstruction(const QString filename);

    /*
     * Read the XML file and add all the cameras informations in the photos QList
     */
    void read(QList<Camera>& photos);

private:
    void processPhotos(QList<Camera>& photos);
    void processPhoto(Camera& photo);
    void processImagePath(Camera& photo);
    void processImageDimensions(Camera& photo);
    void processFocalLength(Camera& photo);
    void processCameraModelType(Camera& photo);
    void processAspectRatio(Camera& photo);
    void processSkew(Camera& photo);
    void processPrincipalPoint(Camera& photo);
    void processRotation(Camera& photo);
    void processCenter(Camera& photo);
    QString readNextText();
    QString errorString();

    QString _filename;
    QXmlStreamReader xml;
};
