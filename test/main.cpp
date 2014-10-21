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
