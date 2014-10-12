#include "RecordedRequest.h"

RecordedRequest::RecordedRequest()
{
}

RecordedRequest::RecordedRequest(
        const QString &requestLine, const QList<QString> &headers,
        const QList<int> &chunkSizes, long bodySize,
        const QByteArray &body, int sequenceNumber //, Socket
        ) :
    m_requestLine(requestLine) ,
    m_headers(headers) ,
    m_chunkSizes(chunkSizes) ,
    m_bodySize(bodySize) ,
    m_body(body) ,
    m_sequenceNumber(sequenceNumber)
{
    if (!requestLine.isNull()) {
        int methodEnd = requestLine.indexOf(' ');
        int pathEnd = requestLine.indexOf(' ', methodEnd + 1);
        m_method = requestLine.mid(0, methodEnd);
        m_path = requestLine.mid(methodEnd + 1, pathEnd);
    }
}

bool RecordedRequest::isNull() const
{
    return m_requestLine.isNull();
}

QString RecordedRequest::requestLine() const
{
    return m_requestLine;
}

QString RecordedRequest::method() const
{
    return m_method;
}

QString RecordedRequest::path() const
{
    return m_path;
}

QList<QString> RecordedRequest::headers() const
{
    return m_headers;
}

QString RecordedRequest::header(const QString &name) const
{
    QString searched = name + ":";
    foreach (const QString &header, m_headers) {
        if (header.startsWith(searched, Qt::CaseInsensitive)) {
            return header.mid(searched.length()).trimmed();
        }
    }
    return QString();
}

QList<QString> RecordedRequest::headers(const QString &name) const
{
    QList<QString> results;
    QString searched = name + ":";
    foreach (const QString &header, m_headers) {
        if (header.startsWith(searched, Qt::CaseInsensitive)) {
            results << header.mid(searched.length()).trimmed();
        }
    }
    return results;
}

QList<int> RecordedRequest::chunkSizes() const
{
    return m_chunkSizes;
}

long RecordedRequest::bodySize() const
{
    return m_bodySize;
}

QByteArray RecordedRequest::body() const
{
    return m_body;
}

QString RecordedRequest::utf8Body() const
{
    return QString::fromUtf8(m_body);
}

int RecordedRequest::sequenceNumber() const
{
    return m_sequenceNumber;
}
