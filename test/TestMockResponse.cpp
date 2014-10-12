#include <QTest>

#include "MockResponse.h"
#include "TestMockResponse.h"

void TestMockResponse::defaultMockResponse()
{
    MockResponse response;
    QList<QString> headers = QList<QString>() << "Content-Length: 0";
    QCOMPARE(response.headers(), headers);
    QCOMPARE(response.status(), QString("HTTP/1.1 200 OK"));
}

void TestMockResponse::setBodyAdjustHeaders()
{
    MockResponse response = MockResponse().setBody("ABC");
    QList<QString> headers = QList<QString>() << "Content-Length: 3";
    QCOMPARE(response.headers(), headers);
    QCOMPARE(response.body(), QByteArray("ABC"));
    QCOMPARE(response.status(), QString("HTTP/1.1 200 OK"));
}

void TestMockResponse::addHeader()
{
    MockResponse response = MockResponse()
        .clearHeaders()
        .addHeader("Cookie: q=1")
        .addHeader("Cookie", "t=2");
    QList<QString> headers = QList<QString>() << "Cookie: q=1" << "Cookie: t=2";
    QCOMPARE(response.headers(), headers);
}

void TestMockResponse::setHeader()
{
    MockResponse response = MockResponse()
        .clearHeaders()
        .addHeader("Cookie: q=1")
        .addHeader("Cookie: t=2")
        .addHeader("Cookies: delicious");
    response.setHeader("cookie", "r=robot");
    QList<QString> headers = QList<QString>()
            << "Cookies: delicious"
            << "cookie: r=robot";
    QCOMPARE(response.headers(), headers);
}
