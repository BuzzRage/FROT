#include "clientfrm.h"
using namespace std;
#include <QDebug>

ClientFrm::ClientFrm(QWidget *parent) :
    QMainWindow(parent)
{
    setupUi(this);
    //connect(outputLbl,SIGNAL(MoussePressed(QPoint)),this,SLOT(mousePressed(QPoint)));
    socket = new QTcpSocket(this);
    connect(socket,SIGNAL(readyRead()), this, SLOT(getServerData()));
    connect(socket,SIGNAL(connected()), this, SLOT(connectServer()));
    connect(socket,SIGNAL(disconnected()), this, SLOT(deconnect()));
    connect(socket,SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(errorSocket(QAbstractSocket::SocketError)));
    tailleMsg = 0;
}

void ClientFrm::connectServer(){
    msgBox->append(tr("<em>Connexion réussie!</em>"));

    connectBtn->setEnabled(false);
    serverAdress->setEnabled(false);
    serverPort->setEnabled(false);
}

void ClientFrm::deconnect(){
    msgBox->append(tr("<em>Déconnecté du serveur</em>"));
    connectBtn->setEnabled(true);
    serverAdress->setEnabled(true);
    serverPort->setEnabled(true);
}

void ClientFrm::on_connectBtn_clicked()
{
    msgBox->append("Tentaive de connexion");
    connectBtn->setEnabled(false);
    socket->abort();
    socket->connectToHost(serverAdress->text(), serverPort->value());

}

void ClientFrm::on_sendBtn_clicked()
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);
    QString dataToSend = msgToSend->text();
    out<<(qint32) 0;
    out<<dataToSend;
    out.device()->seek(0);
    out<<(qint32)(paquet.size() - sizeof(quint32));
    socket->write(paquet);
    msgToSend->clear();
    msgToSend->focusWidget();
}

void ClientFrm::on_msgToSend_returnPressed()
{
    on_sendBtn_clicked();
}

void ClientFrm::errorSocket(QAbstractSocket::SocketError erreur){
    switch(erreur){
        case QAbstractSocket::HostNotFoundError:
            msgBox->append(tr("<em>Erreur: Le serveur n'a pas pu être trouvé. Mauvaise IP ou mauvais port.</em>"));
            break;
        case QAbstractSocket::ConnectionRefusedError:
            msgBox->append(tr("<em>Erreur: Le serveur a refusé la connexion. Vérifier si le programme serveur a bien été lancé. Vérifier également l'IP et le port.</em>"));
            break;
        case QAbstractSocket::RemoteHostClosedError:
            msgBox->append(tr("<em>Erreur: Le serveur a coupé la connexion.</em>"));
            break;
        default:
            msgBox->append(tr("<em>Erreur: ")+socket->errorString()+tr("</em>"));
    }
    connectBtn->setEnabled(true);
}

void ClientFrm::getServerData()
{
    //msgBox->append("getdata");
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_5_7);
    if(tailleMsg == 0){
        if(socket->bytesAvailable() < (int)sizeof(quint32)){
            return;
        }
        in>>tailleMsg;
    }

    if(socket->bytesAvailable() < tailleMsg){
        return;
    }
    qDebug()<<"Taille reçu :"<<tailleMsg;
    QImage image;
    QByteArray bytes;
    in>>bytes;
    if(!image.loadFromData(bytes)){
        msgBox->append("image loading");
    }

    if(image.isNull()){
         msgBox->append("C'est pas une image !");
    }else{
        outputLbl->setPixmap(QPixmap::fromImage(image));
        outputLbl->resize(outputLbl->pixmap()->size());
    }
    tailleMsg = 0;
}


void ClientFrm::on_quitBtn_clicked()
{
    close();
}

void ClientFrm::on_outputLbl_moussePressed(const QPoint pt)
{
    // Envoyer le point au serveur
    qDebug()<<"Position(x,y): ("<<pt.x()<<","<<pt.y()<<")";
    QString img = QString::number(lblImg->value());

    QString str = QString::number(pt.x())+";"+QString::number(pt.y())+";"+img;
    sendString(str);
}

void ClientFrm::on_videoBtn_pressed()
{
    QString str = "v;";
    sendString(str);
}

void ClientFrm::on_photoBtn_pressed()
{
    QString str = "p;";
    sendString(str);
}

void ClientFrm::sendString(QString str){
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    QString dataToSend = str; // cas1: label;posx;posy; cas2: ;v p; cas3: non traité
    out<<(qint32) 0;
    out<<dataToSend;
    out.device()->seek(0);
    out<<(qint32)(paquet.size() - sizeof(quint32));
    socket->write(paquet);
}
