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


#include <QTest>

#include "RecordedRequest.h"
#include "TestRecordedRequest.h"

void TestRecordedRequest::accessors()
{
    QList<QString> headers;
    headers << "User-Agent: qt"
            << "Cookie: q=1"
            << "Cookie: t=2"
            << "X-Whitespace: left"
            << "X-Whitespace:right "
            << "X-Whitespace: both ";
    QList<int> chunkSizes;
    QByteArray body("ABC");
    QString requestLine("GET / HTTP/1.1");

    RecordedRequest request(requestLine, headers, chunkSizes,
                            body.length(), body, 0);

    QCOMPARE(request.path(), QString("/"));
    QCOMPARE(request.header("cookie"), QString("q=1"));
    QList<QString> cookieHeaders = QList<QString>() << "q=1" << "t=2";
    QCOMPARE(request.headers("cookie"), cookieHeaders);
    QCOMPARE(request.header("x-whitespace"), QString("left"));
    QList<QString> wsHeaders = QList<QString>() << "left" << "right" << "both";
    QCOMPARE(request.headers("x-whitespace"), wsHeaders);
    QCOMPARE(request.body(), body);
}
