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

#include <QMutex>
#include <QObject>
#include <QQueue>

class QString;
class QTcpSocket;
class QUrl;
class Dispatcher;
class MockResponse;
class ThreadTcpServer;
class RecordedRequest;

/**
 * @brief A mock web server serving requests using pre-queued mock responses
 *        by default.
 */
class QtMockWebServer : public QObject
{
    Q_OBJECT

    friend class ConnectionHandler;
public:
    QtMockWebServer();
    ~QtMockWebServer();

    /**
     * @brief Port of the mock server listening. Note that it's only avaliable
     *        after server started by play().
     * @return Server port number
     */
    int port() const;
    /**
     * @brief host name of the mock server listening. Note that it's only
     *        available after server started by play().
     * @return Server host name.
     */
    QString hostName() const;

    /**
     * @brief Get absolute url of given path. Note that it's only available
     *        after server started by play().
     * @param path Requested path.
     * @return Absolute url of given path.
     */
    QUrl getUrl(const QString &path);

    /**
     * \todo Not fully implemented.
     * @brief Set max length limit of response body.
     * @param maxBodyLength Max body length limit.
     */
    void setBodyLimit(int maxBodyLength);

    /**
     * @brief Take next recorded request the server received.
     * @return Next recorded request.
     */
    RecordedRequest takeRequest();
    /**
     * @brief Count of requests the server received.
     * @return Count of requests.
     */
    int requestCount() const;

    /**
     * @brief Enqueue a mock response to be served.
     * @param response The mock response.
     */
    void enqueue(const MockResponse &response);

    /**
     * @brief Start the server.
     */
    void play();
    /**
     * @brief Try to start the server on given port.
     * @param port Start the server on this port.
     */
    void play(int port);
    /**
     * @brief Shut down the server.
     */
    void shutdown();

    /**
     * @brief Set a custom dispatcher which decides how to serve requests.
     * @param dispatcher The custome dispatcher.
     */
    void setDispatcher(Dispatcher *dispatcher);

private:
    void enqueueRequest(const RecordedRequest &request);

    QQueue<RecordedRequest> m_requestQueue;
    QMutex m_mutex;
    QAtomicInt m_requestCount;
    int m_port;
    int m_bodyLimit;
    Dispatcher *m_dispatcher;
    ThreadTcpServer *m_server;
};

#endif // QTMOCKWEBSERVER_H
