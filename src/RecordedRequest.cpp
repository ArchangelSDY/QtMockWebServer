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


#include <QStringList>

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
        QStringList parts = requestLine.split(' ');
        if (parts.count() >= 3) {
            m_method = parts[0];
            m_path = parts[1];
        }
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
