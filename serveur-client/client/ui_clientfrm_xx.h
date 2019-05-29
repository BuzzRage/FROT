/********************************************************************************
** Form generated from reading UI file 'clientfrm.ui'
**
** Created by: Qt User Interface Compiler version 5.7.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CLIENTFRM_H
#define UI_CLIENTFRM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextBrowser>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

#include <QMouseEvent>
#include <QPoint>

QT_BEGIN_NAMESPACE

class TotoLabel : public QLabel
{
    Q_OBJECT

public:
    TotoLabel( QWidget* parent = 0, Qt::WindowFlags f = 0 ) : QLabel( parent, f ) {};
    TotoLabel( const QString& text, QWidget* parent = 0, Qt::WindowFlags f = 0 ): QLabel( text, parent, f ) {};
signals:
    void MoussePressed(const QPoint );

private:
    void mousePressEvent( QMouseEvent* ev )
        {
            const QPoint p = ev->pos();
            emit MoussePressed( p );
        }
};

class Ui_ClientFrm
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_5;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_3;
    QHBoxLayout *horizontalLayout;
    QLabel *label;
    QSpinBox *serverPort;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    QLineEdit *serverAdress;
    QPushButton *connectBtn;
    QVBoxLayout *verticalLayout_4;
    TotoLabel *outputLbl;
    QLabel *label_3;
    QTextBrowser *msgBox;
    QHBoxLayout *horizontalLayout_4;
    QLabel *label_4;
    QLineEdit *msgToSend;
    QPushButton *sendBtn;
    QPushButton *quitBtn;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *ClientFrm)
    {
        if (ClientFrm->objectName().isEmpty())
            ClientFrm->setObjectName(QStringLiteral("ClientFrm"));
        ClientFrm->resize(828, 971);
        centralWidget = new QWidget(ClientFrm);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout->addWidget(label);

        serverPort = new QSpinBox(centralWidget);
        serverPort->setObjectName(QStringLiteral("serverPort"));
        serverPort->setMaximum(10000);

        horizontalLayout->addWidget(serverPort);


        verticalLayout_3->addLayout(horizontalLayout);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        serverAdress = new QLineEdit(centralWidget);
        serverAdress->setObjectName(QStringLiteral("serverAdress"));

        horizontalLayout_2->addWidget(serverAdress);


        verticalLayout_3->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout_3);

        connectBtn = new QPushButton(centralWidget);
        connectBtn->setObjectName(QStringLiteral("connectBtn"));

        horizontalLayout_3->addWidget(connectBtn);


        verticalLayout_5->addLayout(horizontalLayout_3);

        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        outputLbl = new TotoLabel(centralWidget);
        outputLbl->setObjectName(QStringLiteral("outputLbl"));
        outputLbl->setMinimumSize(QSize(640, 480));
        outputLbl->setMaximumSize(QSize(640, 480));

        verticalLayout_4->addWidget(outputLbl);

        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_4->addWidget(label_3);

        msgBox = new QTextBrowser(centralWidget);
        msgBox->setObjectName(QStringLiteral("msgBox"));

        verticalLayout_4->addWidget(msgBox);


        verticalLayout_5->addLayout(verticalLayout_4);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        label_4 = new QLabel(centralWidget);
        label_4->setObjectName(QStringLiteral("label_4"));

        horizontalLayout_4->addWidget(label_4);

        msgToSend = new QLineEdit(centralWidget);
        msgToSend->setObjectName(QStringLiteral("msgToSend"));

        horizontalLayout_4->addWidget(msgToSend);

        sendBtn = new QPushButton(centralWidget);
        sendBtn->setObjectName(QStringLiteral("sendBtn"));

        horizontalLayout_4->addWidget(sendBtn);


        verticalLayout_5->addLayout(horizontalLayout_4);

        quitBtn = new QPushButton(centralWidget);
        quitBtn->setObjectName(QStringLiteral("quitBtn"));

        verticalLayout_5->addWidget(quitBtn);


        verticalLayout->addLayout(verticalLayout_5);

        ClientFrm->setCentralWidget(centralWidget);
        mainToolBar = new QToolBar(ClientFrm);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        ClientFrm->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(ClientFrm);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        ClientFrm->setStatusBar(statusBar);

        retranslateUi(ClientFrm);

        QMetaObject::connectSlotsByName(ClientFrm);
    } // setupUi

    void retranslateUi(QMainWindow *ClientFrm)
    {
        ClientFrm->setWindowTitle(QApplication::translate("ClientFrm", "ClientFrm", Q_NULLPTR));
        label->setText(QApplication::translate("ClientFrm", "Num\303\251ro du port", Q_NULLPTR));
        label_2->setText(QApplication::translate("ClientFrm", "Adresse du serveur", Q_NULLPTR));
        connectBtn->setText(QApplication::translate("ClientFrm", "Connexion", Q_NULLPTR));
        outputLbl->setText(QApplication::translate("ClientFrm", "waiting for images", Q_NULLPTR));
        label_3->setText(QApplication::translate("ClientFrm", "Messages", Q_NULLPTR));
        label_4->setText(QApplication::translate("ClientFrm", "Message \303\240 envoyer", Q_NULLPTR));
        sendBtn->setText(QApplication::translate("ClientFrm", "Envoyer au serveur", Q_NULLPTR));
        quitBtn->setText(QApplication::translate("ClientFrm", "Quitter", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class ClientFrm: public Ui_ClientFrm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CLIENTFRM_H
