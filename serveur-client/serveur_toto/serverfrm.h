#ifndef SERVERFRM_H
#define SERVERFRM_H

#include <QtWidgets>
#include <QtNetwork>
#include <stdlib.h>
#include <QtCore>
#include "ui_serverfrm.h"

class ServerFrm : public QMainWindow, private Ui::ServerFrm
{
    Q_OBJECT

    public:
        explicit ServerFrm(QWidget *parent = 0);
        void sendtoclient(const QString &message);

    private slots:  //les slots de gestion des évènements
       void connexion();
       void getdata();
       void deconnexion();
       QString getIPaddress();

    private:
       QLabel *serverstatus;//affiche l'état du serveur
       QLabel *messagelbl; //affiche le message du client
       QPushButton *quitBtn;
       Qserver *server;
       QList<QTcpSocket *> client;
       quint16 tailleMessage;
};

#endif // SERVERFRM_H
