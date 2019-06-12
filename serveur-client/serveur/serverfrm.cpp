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
    cpt=0;
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
                mode_photo = false;
                sujet = list.at(1).toInt(&ok, 10);
                train();
                timer.start(200);
            }
            else if(mode == "p"){
                mode_photo = true;
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
        Camera.open();
        time(&timer_init);
        isSetted = true;
    }
}

void ServerFrm::getImage(){
    Mat image;
    Mat imgBuffer;
    Camera.grab();
    Camera.retrieve(image);
    flip(image, flippedImg, -1);


    equalizeHist(flippedImg, flippedImg);
    recoVisage = flippedImg.clone();

    faceCascade.detectMultiScale(flippedImg,faces, 1.1, 2, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30));

    if(faces.size()==0){
        currentX= -1;
        currentY= -1;
    }
    else{
        for(size_t i=0; i<faces.size(); i++){
            //qDebug()<<"J'suis.";
            if(!mode_photo){
                model->set("threshold",threshold);
                int x = (faces[i].x-(faces[i].width/20)>0) ? faces[i].x-(faces[i].width/20) : faces[i].x;
                int y = (faces[i].y-(faces[i].height/10)>0) ? faces[i].y-(faces[i].height/10) : faces[i].y;
                int width = (faces[i].width+(faces[i].width/10)+x < 640) ? faces[i].width+(faces[i].width/10) : faces[i].width;
                int height = (faces[i].height+(faces[i].height/5)+y < 480 ) ? faces[i].height+(faces[i].height/5) : faces[i].height;

                cv::Rect roi(x,y,width,height);
                imgBuffer = flippedImg(roi);
                int pred = model->predict(imgBuffer);
                string model_info = format("\tLBPH(radius=%i, neighbors=%i, grid_x=%i, grid_y=%i, threshold=%.2f)",
                                           model->getInt("radius"),
                                           model->getInt("neighbors"),
                                           model->getInt("grid_x"),
                                           model->getInt("grid_y"),
                                           model->getDouble("threshold"));
                qDebug()<<QString::fromStdString(model_info);
                string str = format("Label : %i",pred);
                qDebug()<<pred<<" sjt: "<<sujet;
                if(pred == sujet){
                    rectangle(flippedImg, faces[i], Scalar(255, 0, 255), 1, 8, 0);
                    putText(flippedImg, str, cvPoint(faces[i].x, (faces[i].y-5)), FONT_HERSHEY_SIMPLEX, 0.8, Scalar(255,255,255));
                }
            }else{
                rectangle(flippedImg, faces[i], Scalar(255, 0, 255), 1, 8, 0);
            }
            //qDebug()<<"X : "<<faces[i].x<<" Y : "<<faces[i].y;
        }
    }

}

void ServerFrm::extractImg(int posX, int posY, int label){
    Mat visage;
    for(size_t i=0; i<faces.size(); i++){
        //qDebug()<<faces[i].x<<faces[i].x + faces[i].width<<faces[i].y<<faces[i].y + faces[i].height;
        if(faces[i].x < posX && posX < (faces[i].x + faces[i].width)){
            if(faces[i].y < posY && posY < (faces[i].y + faces[i].height)){
                cv::Rect roi(faces[i].x-(faces[i].width/20),faces[i].y-(faces[i].height/10),faces[i].width+(faces[i].width/10),faces[i].height+(faces[i].height/5));
                visage = recoVisage(roi);
                char buff[80];
                sprintf(buff,"CG%03d.png",cpt++);

                imwrite(buff,visage);
                trainingImgs.push_back(visage);
                trainingLabel.push_back(label);
                qDebug()<< "Img" << trainingImgs.size() << "Lbl" << trainingLabel.size();
            }
        }
    }
}

void ServerFrm::train()
{
    // The following lines simply get the last images from
    // your dataset and remove it from the vector. This is
    // done, so that the training data (which we learn the
    // cv::LBPHFaceRecognizer on) and the test data we test
    // the model with, do not overlap.
    Mat testSample = trainingImgs[trainingImgs.size() - 1];
    int testLabel = trainingLabel[trainingLabel.size() - 1];
    trainingImgs.pop_back();
    trainingLabel.pop_back();
    model = createLBPHFaceRecognizer();
    model->set("threshold",threshold);
    model->train(trainingImgs, trainingLabel);
    // The following line predicts the label of a given
    // test image:
    int predictedLabel = model->predict(testSample);
    //
    // To get the confidence of a prediction call the model with:
    //
    //      int predictedLabel = -1;
    //      double confidence = 0.0;
    //      model->predict(testSample, predictedLabel, confidence);
    //
    string result_message = format("Predicted class = %d / Actual class = %d.", predictedLabel, testLabel);
    msgBox->append(result_message.c_str());

    // First we'll use it to set the threshold of the LBPHFaceRecognizer
    // to 0.0 without retraining the model. This can be useful if
    // you are evaluating the model:
    //

    // Now the threshold of this model is set to 0.0. A prediction
    // now returns -1, as it's impossible to have a distance below
    // it
    predictedLabel = model->predict(testSample);
    msgBox->append("Predicted class = ");
    msgBox->append(tr("pred = %1").arg(predictedLabel));

    // We could get the histograms for example:
    vector<Mat> histograms = model->getMatVector("histograms");
    // But should I really visualize it? Probably the length is interesting:
    cout << "Size of the histograms: " << histograms[0].total() << endl;
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

void ServerFrm::on_thresholdSpn_editingFinished()
{
    threshold = thresholdSpn->value();
}
