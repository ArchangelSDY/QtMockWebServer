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


#ifndef QUEUEDISPATCHER_H
#define QUEUEDISPATCHER_H

#include <QQueue>

#include "Dispatcher.h"

class MockResponse;

/**
 * @brief A dispatcher using queue policy.
 */
class QueueDispatcher : public Dispatcher
{
public:
    QueueDispatcher();
    virtual ~QueueDispatcher();

    /**
     * @brief Serve incoming request with next response in queue.
     * @param request The incoming request.
     * @return Next response in queue.
     */
    virtual MockResponse dispatch(const RecordedRequest &request);
    /**
     * @brief Head response in queue if queue is not empty.
     *        Otherwise the fail-fast response is returned if set.
     *        If both are empty, Dispatcher::peek() is returned.
     * @return Guess of next resposne.
     */
    virtual MockResponse peek();

    /**
     * @brief Enqueue given response.
     * @param response A mock response.
     */
    void enqueueResponse(const MockResponse &response);
    /**
     * \todo It will be useful after queue is implemented blockable.
     * @brief Set if we should dispatch a immediate fail response when
     *        the queue is empty.
     * @param failFast True if should fail fast.
     */
    void setFailFast(bool failFast);
    /**
     * \todo It will be useful after queue is implemented blockable.
     * @brief Set the fail-fast response which will be used when the queue is
     *        empty.
     * @param failFastResponse
     */
    void setFailFast(MockResponse *failFastResponse);

protected:
    QQueue<MockResponse> m_responseQueue;

private:
    MockResponse *m_failFastResponse;
};

#endif // QUEUEDISPATCHER_H
