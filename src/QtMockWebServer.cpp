#include <QDebug>
#include <QHostInfo>
#include <QRunnable>
#include <QTcpSocket>
#include <QThreadPool>
#include <QUrl>

#include "QtMockWebServer.h"
#include "QueueDispatcher.h"

QtMockWebServer::QtMockWebServer() :
    m_port(-1) ,
    m_bodyLimit(INT_MAX) ,
    m_dispatcher(new QueueDispatcher()) ,
    m_server(new QTcpServer())
{
    QObject::connect(m_server, SIGNAL(newConnection()),
            this, SLOT(newConnection()));
}

QtMockWebServer::~QtMockWebServer()
{
    delete m_dispatcher;
    delete m_server;
}

int QtMockWebServer::port() const
{
    return m_port;
}

QString QtMockWebServer::hostName() const
{
    return QHostInfo::localHostName();
}

QUrl QtMockWebServer::obtainUrl(const QString &path)
{
    QString urlStr = QString("http://%1:%2%3")
            .arg(hostName(), QString::number(port()), path);
    return QUrl(urlStr);
}

void QtMockWebServer::setBodyLimit(int maxBodyLength)
{
    m_bodyLimit = maxBodyLength;
}

RecordedRequest QtMockWebServer::takeRequest()
{
    return m_requestQueue.dequeue();
}

int QtMockWebServer::requestCount() const
{
    // TODO: return total req count
    return 0;
}

void QtMockWebServer::enqueue(const MockResponse &response)
{
    QueueDispatcher *dispatcher = static_cast<QueueDispatcher *>(m_dispatcher);
    dispatcher->enqueueResponse(response);
}

void QtMockWebServer::play()
{
    play(0);
}

void QtMockWebServer::play(int port)
{
    m_server->listen(QHostAddress::Any, port);
    m_port = m_server->serverPort();
    // QThreadPool::globalInstance()->start(new ServerRunnable(this));
}

void QtMockWebServer::shutdown()
{
    m_server->close();
}

void QtMockWebServer::setDispatcher(Dispatcher *dispatcher)
{
    if (dispatcher) {
        delete m_dispatcher;
        m_dispatcher = dispatcher;
    }
}

void QtMockWebServer::newConnection()
{
    QTcpSocket *socket = m_server->nextPendingConnection();
    int sequenceNumber = 0;

    while (!processOneRequest(socket, sequenceNumber)) {
    }

    if (sequenceNumber == 0) {
        qDebug() << "Connection didn't make a request";
    }

    socket->disconnectFromHost();
    socket->waitForDisconnected();
    socket->deleteLater();
}

bool QtMockWebServer::processOneRequest(QTcpSocket *socket, int &sequenceNumber)
{
    RecordedRequest request = readRequest(socket, sequenceNumber);
    if (request.isNull()) {
        return false;
    }

    // TODO: incre count

    m_requestQueue.enqueue(request);
    MockResponse response = m_dispatcher->dispatch(request);

    writeResponse(socket, response);

    sequenceNumber++;
    return true;
}

RecordedRequest QtMockWebServer::readRequest(QTcpSocket *socket,
                                             int &sequenceNumber)
{
    QString request = readAsciiUntilCrlf(socket);
    if (request.isEmpty()) {
        return RecordedRequest();
    }

    QList<QString> headers;
    long contentLength = -1;
    bool chunked = false;
    bool expectContinue = false;
    QString header;
    while (!((header = readAsciiUntilCrlf(socket)).isEmpty())) {
        headers.append(header);
        QString lowercaseHeader = header.toLower();
        if (contentLength == -1
                && lowercaseHeader.startsWith("content-length:")) {
            contentLength = header.mid(15).trimmed().toLong();
        }

        if (lowercaseHeader.startsWith("transfer-encoding:")
                && lowercaseHeader.mid(18).trimmed() == "chunked") {
            chunked = true;
        }

        if (lowercaseHeader.startsWith("expect:")
                && lowercaseHeader.mid(7).trimmed() == "100-continue") {
            expectContinue = true;
        }
    }

    if (expectContinue) {
        socket->write("HTTP/1.1 100 Continue\r\n");
        socket->write("Content-Length: 0\r\n");
        socket->write("\r\n");
        socket->flush();
    }

    bool hasBody = false;
    QByteArray body;
    QList<int> chunkSizes;
    if (contentLength != -1) {
        hasBody = (contentLength > 0);
        body = socket->read(contentLength);
    } else if (chunked) {
        hasBody = true;
        forever {
            int chunkSize =
                readAsciiUntilCrlf(socket).trimmed().toInt(0, 16);
            if (chunkSize == 0) {
                readEmptyLine(socket);
                break;
            }
            chunkSizes.append(chunkSize);
            body += socket->read(chunkSize);
            readEmptyLine(socket);
        }
    }

    if (request.startsWith("OPTIONS ")
            || request.startsWith("GET ")
            || request.startsWith("HEAD ")
            || request.startsWith("TRACE ")
            || request.startsWith("CONNECT ")) {
        if (hasBody) {
            qDebug() << "Request must not have a body" << request;
            return RecordedRequest();
        }
    } else if (!request.startsWith("POST "
            && !request.startsWith("PUT ")
            && !request.startsWith("PATCH ")
            && !request.startsWith("DELETE "))) {
        qDebug() << "Unexpected method" << request;
        return RecordedRequest();
    }

    return RecordedRequest(request, headers, chunkSizes,
                           body.size(), body, sequenceNumber);

}

QString QtMockWebServer::readAsciiUntilCrlf(QTcpSocket *socket)
{
    QString ret;
    forever {
        char c;
        if (socket->getChar(&c)) {
            if (c == '\n' && !ret.isEmpty() && ret.endsWith('\r')) {
                return ret.left(ret.length() - 1);
            } else {
                ret.append(c);
            }
        } else {
            return ret;
        }
    }
}

void QtMockWebServer::readEmptyLine(QTcpSocket *socket)
{
    QString line = readAsciiUntilCrlf(socket);
    if (!line.isEmpty()) {
        qDebug() << "Expected empty line but" << line;
    }
}

void QtMockWebServer::writeResponse(QTcpSocket *socket, const MockResponse &response)
{
    socket->write((response.status() + "\r\n").toLatin1());
    foreach (const QString &header, response.headers()) {
        socket->write((header + "\r\n").toLatin1());
    }
    socket->write("\r\n");
    socket->flush();

    if (!response.body().isEmpty()) {
        socket->write(response.body());
    }
}
