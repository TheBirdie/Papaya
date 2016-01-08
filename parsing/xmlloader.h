#pragma once
#include <vector>
#include <QImage>
#include <QFileInfo>
#include <QtCore>
#include <camera.h>

/*
 *  Gives all the information about the camera
 */
struct Camera{
        QString imagePath;
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


class XmlLoader{
public:
    XmlLoader(const QString filename);

    /*
     * Read the XML file and add all the cameras informations in the list photos
     */
    void read(std::list<Camera>& photos);

private:
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
