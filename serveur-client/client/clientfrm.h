#ifndef CLIENTFRM_H
#define CLIENTFRM_H

#include "ui_clientfrm.h"
#include <QDialog>
#include <QtNetwork>


class ClientFrm : public QMainWindow, private Ui::ClientFrm
{
    Q_OBJECT

public:
    explicit ClientFrm(QWidget *parent = 0);
private slots:
    void on_connectBtn_clicked();
    void on_sendBtn_clicked();
    void on_msgToSend_returnPressed();
    void connectServer();
    void deconnect();
    void errorSocket(QAbstractSocket::SocketError erreur);
    void getServerData();
    void sendString(QString str);
    void on_quitBtn_clicked();

    void on_outputLbl_moussePressed(const QPoint pt);
    void on_videoBtn_pressed();

    void on_photoBtn_pressed();

private:
    QTcpSocket *socket;
    quint32 tailleMsg;


};

#endif // CLIENTFRM_H
