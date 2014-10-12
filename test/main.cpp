#include <QCoreApplication>
#include <QTest>

#include "TestMockResponse.h"
#include "TestRecordedRequest.h"
#include "TestQtMockWebServer.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);

    int err = 0;
    {
        TestRecordedRequest testRecordedRequest;
        err = qMax(err, QTest::qExec(&testRecordedRequest, app.arguments()));
    }

    {
        TestMockResponse testMockResponse;
        err = qMax(err, QTest::qExec(&testMockResponse, app.arguments()));
    }

    {
        TestQtMockWebServer testQtMockWebServer;
        err = qMax(err, QTest::qExec(&testQtMockWebServer, app.arguments()));
    }

    if (err != 0) {
        qWarning("There are errors in some of the tests above.");
    }

    return err;
}
