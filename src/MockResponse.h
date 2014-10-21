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


#ifndef MOCKRESPONSE_H
#define MOCKRESPONSE_H

#include <QList>
#include <QString>

class QBuffer;

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
