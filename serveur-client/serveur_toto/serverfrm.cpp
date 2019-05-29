#include "serverfrm.h"

ServerFrm::ServerFrm(QWidget *parent)
:   QDialog(parent), server(0){

//    statusLabel = new QLabel;
//    quitButton = new QPushButton(tr("Quit"));
//     quitButton->setAutoDefault(false);


//        connect(quitButton, SIGNAL(clicked()), this, SLOT(close()));
        connect(server, SIGNAL(newConnection()), this, SLOT(sendFortune()));
        server = new QTcpServer(this);
        if (!server->listen(QHostAddress::Any,2048)) {
            QMessageBox::critical(this, tr("Fortune Server"),
                                  tr("Unable to start the server: %1.")
                                  .arg(server->errorString()));
            close();
            return;
        }
//        QHBoxLayout *buttonLayout = new QHBoxLayout;
//        buttonLayout->addStretch(1);
//        buttonLayout->addWidget(quitButton);
//        buttonLayout->addStretch(1);

//        QVBoxLayout *mainLayout = new QVBoxLayout;
//        mainLayout->addWidget(statusLabel);
//        mainLayout->addLayout(buttonLayout);
//        setLayout(mainLayout);

//        setWindowTitle(tr("Fortune Server"));

}

//void ServerFrm::sendFortune()
//{
//    QByteArray block;
//    QDataStream out(&block, QIODevice::WriteOnly);
//    out.setVersion(QDataStream::Qt_4_0);
//    out << (quint16)0;
//    out << fortunes.at(qrand() % fortunes.size());
//    out.device()->seek(0);
//    out << (quint16)(block.size() - sizeof(quint16));

//    QTcpSocket *clientConnection = tcpServer->nextPendingConnection();
//    connect(clientConnection, SIGNAL(disconnected()),
//            clientConnection, SLOT(deleteLater()));

//    clientConnection->write(block);
//    clientConnection->disconnectFromHost();
//}
