#include "serverfrm.h"
#include <QMessageBox>
#include <QDebug>
#include <QBuffer>

ServerFrm::ServerFrm(QWidget *parent) :
    QMainWindow(parent), server(0),client(0)
{
    setupUi(this);
    isSetted = false;
    //timer.start(200);
    initCamera();
    faceCascade.load("/usr/share/opencv/haarcascades/haarcascade_frontalface_default.xml");
    hasClient = false;
    tailleMsg = 0;
    currentX = -1, currentY = -1;
    trainingMode = false;
    server = new QTcpServer(this);
    connect(server, SIGNAL(newConnection()), this, SLOT(nouvelleConnection()));

    if (!server->listen(QHostAddress::Any,2048)) {
        QMessageBox::critical(this, tr("Fortune Server"),
                              tr("Unable to start the server: %1.")
                              .arg(server->errorString()));
        close();
        return;
    }
    msgBox->append("init");
}

void ServerFrm::nouvelleConnection(){
    //msgBox->append("sendData");
    if(!hasClient){
        hasClient = true;

        client = server->nextPendingConnection();
        connect(client, SIGNAL(disconnected()),
                client, SLOT(deleteLater()));
        connect(client,SIGNAL(readyRead()), this, SLOT(getClientData()));

        connect(&timer, SIGNAL(timeout()), this, SLOT(updateLbl()));
    }
}

void ServerFrm::getClientData()
{
    //qDebug()<<"hi from getClientData";
    QDataStream in(client);
    in.setVersion(QDataStream::Qt_5_7);
    if(tailleMsg == 0){
        if(client->bytesAvailable() < (int)sizeof(quint32)){
            return;
        }
        in>>tailleMsg;
    }

    if(client->bytesAvailable() < tailleMsg){
        return;
    }

    QString msg;
    in>>msg;

    if(msg.contains(";")){
        QStringList list = msg.split(";");
        qDebug() << list;
        bool ok;
        if(list.size() == 3){

            currentX = list.at(0).toInt(&ok, 10);
            currentY = list.at(1).toInt(&ok, 10);
            currentLabel = list.at(2).toInt(&ok, 10);
            qDebug() << "Coordonées reçues: " << " X: " << currentX << " Y: " << currentY << " lbl :" << currentLabel;
            extractImg(currentX, currentY, currentLabel);
        }
        else if(list.size() == 2){
            mode = list.at(0);
            qDebug() << "Mode demandé: " << mode;
            if(mode == "v"){
                timer.start(200);
            }
            else if(mode == "p"){
                timer.stop();
                updateLbl();
            }
        }
    }

    msgBox->append(msg);
    tailleMsg = 0;
}

QImage ServerFrm::cvMatToQImage(const cv::Mat &inMat)
{
    switch (inMat.type())
    {
    case CV_8UC4:
    {
        QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step),QImage::Format_ARGB32);
        return image;
    }
    case CV_8UC3:
    {
        QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step),QImage::Format_RGB888);
        return image.rgbSwapped();
    }
    case CV_8UC1:
    {
    #if QT_VERSION >= QT_VERSION_CHECK(5, 5, 0)
        QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step),QImage::Format_Grayscale8);

    #else

        static QVector<QRgb> sColorTable;
        if (sColorTable.isEmpty())
        {
            sColorTable.resize(256);
            for (int i = 0; i < 256; ++i)
            {
                sColorTable[i] = qRgb(i,i,i);
            }
        }
        QImage image(inMat.data, inMat.cols, inMat.rows, static_cast<int>(inMat.step), QImage::Format_Indexed8);
        image.setColorTable(sColorTable);

    #endif

        return image;
    }
    }
    return QImage();
}

void ServerFrm::initCamera() {
    if(!isSetted){
        Camera.set(CV_CAP_PROP_FORMAT, CV_8UC1 );
        Camera.set(CV_CAP_PROP_FRAME_WIDTH, 640 );
        Camera.set(CV_CAP_PROP_FRAME_HEIGHT, 480 );
        time(&timer_init);
        isSetted = true;
    }
}

void ServerFrm::getImage(){
    Mat image;
    Camera.open();
    Camera.grab();
    Camera.retrieve(image);
    flip(image, flippedImg, -1);


    equalizeHist(flippedImg, flippedImg);

        faceCascade.detectMultiScale(flippedImg,faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30));

        if(faces.size()==0){
            currentX= -1;
            currentY= -1;
        }
        else{
            for(size_t i=0; i<faces.size(); i++){
                Point center(faces[i].x + faces[i].width * 0.5, faces[i].y + faces[i].height * 0.5);
                if(qFabs(faces[i].x - currentX) < faces[i].width && qFabs(faces[i].y - currentY) < faces[i].height){
                    //qDebug()<<"J'suis.";
                    trackedFace = faces[i];
                    //trainingImgs.push_back(new Mat(flippedImg, trackedFace));
                }
                rectangle(flippedImg, faces[i], Scalar(255, 0, 255), 1, 8, 0);
                //qDebug()<<"X : "<<faces[i].x<<" Y : "<<faces[i].y;
            }
        }

}

void ServerFrm::extractImg(int posX, int posY, int label){
    Mat visage;
    for(size_t i=0; i<faces.size(); i++){
        qDebug()<<"Bite";
        if(faces[i].x > posX && posX < (faces[i].x + faces[i].width)){
           if(faces[i].y < posY && posY > (faces[i].y + faces[i].height)){
             visage = flippedImg(faces[i]);
             trainingImgs.push_back(visage);
             trainingLabel.push_back(label);
             qDebug()<< "Img" << trainingImgs.size() << "Lbl" << trainingLabel.size();
           }
        }
    }
}

void ServerFrm::updateLbl(){
    getImage();
    QByteArray trame;
    QImage image = cvMatToQImage(flippedImg);
    //envoyer l'image au client
     QByteArray png;
     QBuffer buffer(&png);
     buffer.open(QIODevice::WriteOnly);
     image.save(&buffer, "PNG");
     buffer.close();
     QDataStream out(&trame, QIODevice::WriteOnly);
     
     out.setVersion(QDataStream::Qt_5_7);
     out << (quint32)0;
     out<<png;
     out.device()->seek(0);
     out << (quint32)(trame.size() - sizeof(quint32));
     qDebug()<<"Taille envoyee:"<< trame.size() - sizeof(quint32);
     client->write(trame);
}


void ServerFrm::on_quitBtn_clicked()
{
    client->disconnectFromHost();
    hasClient = false;
    Camera.release();
    close();
}
