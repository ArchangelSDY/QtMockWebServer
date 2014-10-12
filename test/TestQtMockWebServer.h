#ifndef TESTQTMOCKWEBSERVER_H
#define TESTQTMOCKWEBSERVER_H

#include <QNetworkAccessManager>
#include <QObject>

#include "QtMockWebServer.h"

class TestQtMockWebServer : public QObject
{
    Q_OBJECT
private slots:
    void init();
    void cleanup();
    void regularResponse();

private:
    void waitForReply(QNetworkReply *reply);

    QNetworkAccessManager m_mgr;
    QtMockWebServer m_mockServer;
};

#endif // TESTQTMOCKWEBSERVER_H
