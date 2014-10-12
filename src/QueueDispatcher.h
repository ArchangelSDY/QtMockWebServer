#ifndef QUEUEDISPATCHER_H
#define QUEUEDISPATCHER_H

#include <QQueue>

#include "Dispatcher.h"

class MockResponse;

class QueueDispatcher : public Dispatcher
{
public:
    QueueDispatcher();
    virtual ~QueueDispatcher();

    virtual MockResponse dispatch(const RecordedRequest &request);
    virtual MockResponse peek();

    void enqueueResponse(const MockResponse &response);
    void setFailFast(bool failFast);
    void setFailFast(MockResponse *failFastResponse);

protected:
    QQueue<MockResponse> m_responseQueue;

private:
    MockResponse *m_failFastResponse;
};

#endif // QUEUEDISPATCHER_H
