#ifndef RECORDEDREQUEST_H
#define RECORDEDREQUEST_H

#include <QList>
#include <QString>

class RecordedRequest
{
public:
    RecordedRequest();
    RecordedRequest(const QString &requestLine, const QList<QString> &headers,
                    const QList<int> &chunkSizes, long bodySize,
                    const QByteArray &body, int sequenceNumber //, Socket
                    );

    bool isNull() const;
    QString requestLine() const;
    QString method() const;
    QString path() const;
    QList<QString> headers() const;
    QString header(const QString &name) const;
    QList<QString> headers(const QString &name) const;
    QList<int> chunkSizes() const;
    long bodySize() const;
    QByteArray body() const;
    QString utf8Body() const;
    int sequenceNumber() const;

private:
    QString m_requestLine;
    QString m_method;
    QString m_path;
    QList<QString> m_headers;
    QList<int> m_chunkSizes;
    long m_bodySize;
    QByteArray m_body;
    int m_sequenceNumber;
};

#endif // RECORDEDREQUEST_H
