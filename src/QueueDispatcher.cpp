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

    if (m_failFastResponse && m_responseQueue.isEmpty()) {
        return *m_failFastResponse;
    }

    return m_responseQueue.dequeue();
}

MockResponse QueueDispatcher::peek()
{
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
