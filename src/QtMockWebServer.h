#ifndef QTMOCKWEBSERVER_H
#define QTMOCKWEBSERVER_H

#include <QQueue>
#include <QString>
#include <QTcpServer>

#include "Dispatcher.h"
#include "MockResponse.h"
#include "RecordedRequest.h"

class QtMockWebServer : public QObject
{
    Q_OBJECT
public:
    QtMockWebServer();
    ~QtMockWebServer();

    int port() const;
    QString hostName() const;

    QUrl obtainUrl(const QString &path);

    void setBodyLimit(int maxBodyLength);

    RecordedRequest takeRequest();
    int requestCount() const;

    void enqueue(const MockResponse &response);

    void play();
    void play(int port);
    void shutdown();

    void setDispatcher(Dispatcher *dispatcher);

private slots:
    void newConnection();

private:
    bool processOneRequest(QTcpSocket *socket, int &sequenceNumber);
    RecordedRequest readRequest(QTcpSocket *socket, int &sequenceNumber);
    QString readAsciiUntilCrlf(QTcpSocket *socket);
    void readEmptyLine(QTcpSocket *socket);
    void writeResponse(QTcpSocket *socket, const MockResponse &response);

    QQueue<RecordedRequest> m_requestQueue;
    int m_port;
    int m_bodyLimit;
    Dispatcher *m_dispatcher;
    QTcpServer *m_server;
};

#endif // QTMOCKWEBSERVER_H
