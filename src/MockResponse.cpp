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


#include <QBuffer>
#include <QDebug>

#include "MockResponse.h"

const char *MockResponse::CHUNKED_BODY_HEADER = "Transfer-encoding: chunked";

MockResponse::MockResponse() :
    m_status(QString("HTTP/1.1 200 OK"))
{
    setBody(QByteArray());
}

QString MockResponse::status() const
{
    return m_status;
}

MockResponse &MockResponse::setResponseCode(int code)
{
    m_status = QString("HTTP/1.1 %1 OK").arg(code);
    return *this;
}

MockResponse &MockResponse::setStatus(const QString &status)
{
    m_status = status;
    return *this;
}

QList<QString> MockResponse::headers() const
{
    return m_headers;
}

MockResponse &MockResponse::clearHeaders()
{
    m_headers.clear();
    return *this;
}

MockResponse &MockResponse::addHeader(const QString &header)
{
    m_headers.append(header);
    return *this;
}

MockResponse &MockResponse::addHeader(const QString &key, const QString &value)
{
    return addHeader(QString("%1: %2").arg(key, value));
}

MockResponse &MockResponse::setHeader(const QString &key, const QString &value)
{
    removeHeader(key);
    return addHeader(key, value);
}

MockResponse &MockResponse::removeHeader(const QString &key)
{
    QString search = QString("%1:").arg(key);
    QList<QString>::iterator i;
    for (i = m_headers.begin(); i != m_headers.end();) {
        if (i->startsWith(search, Qt::CaseInsensitive)) {
            i = m_headers.erase(i);
        } else {
            ++i;
        }
    }
    return *this;
}

QByteArray MockResponse::body() const
{
    return m_body;
}

MockResponse &MockResponse::setBody(const QByteArray &body)
{
    setHeader("Content-Length", QString::number(body.size()));
    m_body = body;
    return *this;
}

MockResponse &MockResponse::setChunkedBody(const QByteArray &body,
                                           int maxChunkSize)
{
    removeHeader("Content-Length");
    m_headers.append(CHUNKED_BODY_HEADER);

    QBuffer inBuf;
    inBuf.setData(body);
    QByteArray out;
    QBuffer outBuf(&out);

    inBuf.open(QIODevice::ReadOnly);
    outBuf.open(QIODevice::WriteOnly);
    while (!inBuf.atEnd()) {
        long chunkSize = qMin(body.size(), maxChunkSize);
        outBuf.write(QString::number(chunkSize, 16).toUtf8());
        outBuf.write("\r\n");
        outBuf.write(inBuf.read(chunkSize));
        outBuf.write("\r\n");
    }
    outBuf.write("0\r\n\r\n");

    inBuf.close();
    outBuf.close();

    m_body = out;
    return *this;
}
