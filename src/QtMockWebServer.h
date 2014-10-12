#ifndef QTMOCKWEBSERVER_H
#define QTMOCKWEBSERVER_H

#include <QQueue>
#include <QString>

#include "Dispatcher.h"
#include "MockResponse.h"
#include "RecordedRequest.h"

class QTcpSocket;
class ThreadTcpServer;

class QtMockWebServer : public QObject
{
    Q_OBJECT

    friend class ConnectionHandler;
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

private:
    bool processOneRequest(QTcpSocket *socket, int &sequenceNumber);
    RecordedRequest readRequest(QTcpSocket *socket, int &sequenceNumber);
    QString readAsciiUntilCrlf(QTcpSocket *socket);
    void readEmptyLine(QTcpSocket *socket);
    void writeResponse(QTcpSocket *socket, const MockResponse &response);

    QQueue<RecordedRequest> m_requestQueue;
    QAtomicInt m_requestCount;
    int m_port;
    int m_bodyLimit;
    Dispatcher *m_dispatcher;
    ThreadTcpServer *m_server;
};

#endif // QTMOCKWEBSERVER_H
