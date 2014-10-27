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

/**
 * @brief The recorded request to be validated.
 */
class RecordedRequest
{
public:
    RecordedRequest();
    RecordedRequest(const QString &requestLine, const QList<QString> &headers,
                    const QList<int> &chunkSizes, long bodySize,
                    const QByteArray &body, int sequenceNumber //, Socket
                    );

    /**
     * @brief Check if empty.
     * @return True if empty.
     */
    bool isNull() const;
    /**
     * @brief The request line.
     * @return The request line.
     */
    QString requestLine() const;
    /**
     * @brief The request method such as "GET"/"POST", etc.
     * @return The request method.
     */
    QString method() const;
    /**
     * @brief The request path such as "/index.html".
     * @return The request path.
     */
    QString path() const;
    /**
     * @brief A list of request headers.
     * @return A list of request headers.
     */
    QList<QString> headers() const;
    /**
     * @brief Value of given header name.
     * @param name Header name.
     * @return Value of given header name.
     */
    QString header(const QString &name) const;
    /**
     * @brief A list of values of given header name. Headers such as "Cookies"
     *        may have multiple ones.
     * @param name Header name.
     * @return A list of values.
     */
    QList<QString> headers(const QString &name) const;
    /**
     * @brief Sizes of each body chunk.
     * @return Sizes of each body chunk.
     */
    QList<int> chunkSizes() const;
    /**
     * @brief Size of request body.
     * @return Size of request body.
     */
    long bodySize() const;
    /**
     * @brief Content of request body.
     * @return Content of request body.
     */
    QByteArray body() const;
    /**
     * @brief String of request body decoded using "UTF-8".
     * @return String of request body decoded using "UTF-8".
     */
    QString utf8Body() const;
    /**
     * @brief Sequence number of this request.
     * @return Sequence number of this request.
     */
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
