#include <QTest>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSignalSpy>
#include <QEventLoop>

#include "MockResponse.h"
#include "TestQtMockWebServer.h"

void TestQtMockWebServer::init()
{
}

void TestQtMockWebServer::cleanup()
{
    m_mockServer.shutdown();
}

void TestQtMockWebServer::waitForReply(QNetworkReply *reply)
{
    QEventLoop loop;
    QObject::connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}

void TestQtMockWebServer::regularResponse()
{
    MockResponse response = MockResponse().setBody("hello world");
    m_mockServer.enqueue(response);
    m_mockServer.play();

    QUrl url = m_mockServer.obtainUrl("/");
    QNetworkRequest expReq(url);
    expReq.setRawHeader(QByteArray("Accept-Language"), QByteArray("en-US"));

    QNetworkReply *reply = m_mgr.get(expReq);
    waitForReply(reply);

    QVERIFY2(reply->error() == QNetworkReply::NoError,
             "Reply with error");
    QCOMPARE(reply->readAll(), QByteArray("hello world"));

    RecordedRequest actReq = m_mockServer.takeRequest();
    QCOMPARE(actReq.requestLine(), QString("GET / HTTP/1.1"));
    QCOMPARE(actReq.header("Accept-Language"), QString("en-US"));
}
