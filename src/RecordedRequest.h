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
