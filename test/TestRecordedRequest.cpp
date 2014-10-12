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

    QCOMPARE(request.header("cookie"), QString("q=1"));
    QList<QString> cookieHeaders = QList<QString>() << "q=1" << "t=2";
    QCOMPARE(request.headers("cookie"), cookieHeaders);
    QCOMPARE(request.header("x-whitespace"), QString("left"));
    QList<QString> wsHeaders = QList<QString>() << "left" << "right" << "both";
    QCOMPARE(request.headers("x-whitespace"), wsHeaders);
    QCOMPARE(request.body(), body);
}
