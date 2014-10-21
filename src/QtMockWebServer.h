// Copyright 2014 Archangel.SDY@gmail.com
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#ifndef QTMOCKWEBSERVER_H
#define QTMOCKWEBSERVER_H

#include <QObject>
#include <QQueue>

class QString;
class QTcpSocket;
class QUrl;
class Dispatcher;
class MockResponse;
class ThreadTcpServer;
class RecordedRequest;

class QtMockWebServer : public QObject
{
    Q_OBJECT

    friend class ConnectionHandler;
public:
    QtMockWebServer();
    ~QtMockWebServer();

    int port() const;
    QString hostName() const;

    QUrl getUrl(const QString &path);

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
