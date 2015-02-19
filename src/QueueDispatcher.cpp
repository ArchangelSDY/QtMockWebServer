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


#include <QMutexLocker>
#include <QDebug>

#include "MockResponse.h"
#include "QueueDispatcher.h"
#include "RecordedRequest.h"

QueueDispatcher::QueueDispatcher() :
    m_failFastResponse(0)
{
}

QueueDispatcher::~QueueDispatcher()
{
    if (m_failFastResponse) {
        delete m_failFastResponse;
    }
}

MockResponse QueueDispatcher::dispatch(const RecordedRequest &request)
{
    const QString &requestLine = request.requestLine();
    if (!requestLine.isNull() && requestLine == "GET /favicon.ico HTTP/1.1") {
        qDebug() << "Served" << requestLine;
        return MockResponse().setResponseCode(404);
    }

    QMutexLocker locker(&m_queueMutex);

    if (m_responseQueue.isEmpty()) {
        if (m_failFastResponse) {
            return *m_failFastResponse;
        } else {
            return Dispatcher::peek();
        }
    }

    return m_responseQueue.dequeue();
}

MockResponse QueueDispatcher::peek()
{
    QMutexLocker locker(&m_queueMutex);
    if (!m_responseQueue.isEmpty()) {
        return m_responseQueue.head();
    }
    if (m_failFastResponse) {
        return *m_failFastResponse;
    }
    return Dispatcher::peek();
}

void QueueDispatcher::enqueueResponse(const MockResponse &response)
{
    QMutexLocker locker(&m_queueMutex);
    m_responseQueue.enqueue(response);
}

void QueueDispatcher::setFailFast(bool failFast)
{
    if (failFast) {
        MockResponse *failFastResponse = new MockResponse();
        failFastResponse->setResponseCode(404);
        setFailFast(failFastResponse);
    } else {
        setFailFast((MockResponse *)0);
    }
}

void QueueDispatcher::setFailFast(MockResponse *failFastResponse)
{
    if (m_failFastResponse) {
        delete m_failFastResponse;
    }

    m_failFastResponse = failFastResponse;
}
