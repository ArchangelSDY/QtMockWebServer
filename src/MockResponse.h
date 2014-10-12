#ifndef MOCKRESPONSE_H
#define MOCKRESPONSE_H

#include <QBuffer>
#include <QList>
#include <QString>

class MockResponse
{
public:
    MockResponse();

    QString status() const;
    MockResponse &setResponseCode(int code);
    MockResponse &setStatus(const QString &status);

    QList<QString> headers() const;
    MockResponse &clearHeaders();
    MockResponse &addHeader(const QString &header);
    MockResponse &addHeader(const QString &key, const QString &value);
    MockResponse &setHeader(const QString &key, const QString &value);
    MockResponse &removeHeader(const QString &key);

    QByteArray body() const;
    MockResponse &setBody(const QByteArray &body);
    MockResponse &setChunkedBody(const QByteArray &body, int maxChunkSize);

private:
    static const char *CHUNKED_BODY_HEADER;

    QString m_status;
    QList<QString> m_headers;
    QByteArray m_body;
};

#endif // MOCKRESPONSE_H
