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

/**
 * @brief The mock response to be sent to client.
 */
class MockResponse
{
public:
    /**
     * @brief An empty success response.
     */
    MockResponse();

    /**
     * @brief Status line of response
     * @return Status line of response such as "HTTP/1.1 200 OK".
     */
    QString status() const;
    /**
     * @brief Set status code of response.
     * @param code Status code.
     * @return Reference of this object.
     */
    MockResponse &setResponseCode(int code);
    /**
     * @brief Set status line of response.
     * @param status Status line.
     * @return Reference of this object.
     */
    MockResponse &setStatus(const QString &status);

    /**
     * @brief A list of response header.
     * @return A list of response of response headers.
     */
    QList<QString> headers() const;
    /**
     * @brief Clear all response headers.
     * @return Reference of this object.
     */
    MockResponse &clearHeaders();
    /**
     * @brief Add a response header.
     * @param header The header to be added.
     * @return Reference of this object.
     */
    MockResponse &addHeader(const QString &header);
    /**
     * @brief Add a response header by key/value pairs.
     * @param key Header name.
     * @param value Header value.
     * @return Reference of this object.
     */
    MockResponse &addHeader(const QString &key, const QString &value);
    /**
     * @brief Set header value with given name.
     * @param key Header name.
     * @param value Header value.
     * @return Reference of this object.
     */
    MockResponse &setHeader(const QString &key, const QString &value);
    /**
     * @brief Remove header with given name.
     * @param key Header name.
     * @return Reference of this object.
     */
    MockResponse &removeHeader(const QString &key);

    /**
     * @brief Body of response.
     * @return Body of response.
     */
    QByteArray body() const;
    /**
     * @brief Set body of response.
     * @param body Body of response.
     * @return Reference of this object.
     */
    MockResponse &setBody(const QByteArray &body);
    /**
     * @brief Set chunked body of response.
     * @param body Whole body of response.
     * @param maxChunkSize Max size limit of each chunk.
     * @return Reference of this object.
     */
    MockResponse &setChunkedBody(const QByteArray &body, int maxChunkSize);

private:
    static const char *CHUNKED_BODY_HEADER;

    QString m_status;
    QList<QString> m_headers;
    QByteArray m_body;
};

#endif // MOCKRESPONSE_H
