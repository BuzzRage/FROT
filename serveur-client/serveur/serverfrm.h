#ifndef SERVERFRM_H
#define SERVERFRM_H

#include "ui_serverfrm.h"

#include <QTcpServer>
#include <QTcpSocket>

#include "opencv2/opencv.hpp"
#include "raspicam/raspicam.h"
#include "raspicam/raspicam_cv.h"
#include <QTimer>
#include <QtMath>
#include <ctime>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;
using namespace cv;

class ServerFrm : public QMainWindow, private Ui::ServerFrm
{
    Q_OBJECT

public:
    explicit ServerFrm(QWidget *parent = 0);
    inline QImage cvMatToQImage(const cv::Mat &inMat);
private slots:
    void nouvelleConnection();
    void getClientData();
    void on_quitBtn_clicked();
    void initCamera();
    void getImage();
    void updateLbl();
    void extractImg(int,int,int);
    void train();

private:
    QTcpServer *server;
    QTcpSocket *client;
    quint32 tailleMsg;
    bool hasClient;
    bool isSetted;
    QTimer timer;
    time_t timer_init;
    raspicam::RaspiCam_Cv Camera;
    Mat flippedImg;
    CascadeClassifier faceCascade;
    int currentX;
    int currentY;
    int currentLabel;
    QString mode;
    vector<Mat > trainingImgs;
    vector<int> trainingLabel;
    vector<cv::Rect > faces;
    bool trainingMode;
    Ptr<FaceRecognizer > model;
    bool mode_photo;
    int sujet;

};

#endif // SERVERFRM_H
