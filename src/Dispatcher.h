#ifndef DISPATCHER_H
#define DISPATCHER_H

class MockResponse;
class RecordedRequest;

class Dispatcher
{
public:
    virtual ~Dispatcher() {}

    virtual MockResponse dispatch(const RecordedRequest &request) = 0;
    virtual MockResponse peek();
};

#endif // DISPATCHER_H
