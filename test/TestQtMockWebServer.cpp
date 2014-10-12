#include <QTest>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QSignalSpy>

#include "MockResponse.h"
#include "RecordedRequest.h"
#include "TestQtMockWebServer.h"

void TestQtMockWebServer::init()
{
    m_mockServer = new QtMockWebServer();
}

void TestQtMockWebServer::cleanup()
{
    m_mockServer->shutdown();
    delete m_mockServer;
}

void TestQtMockWebServer::waitForReply(QNetworkReply *reply)
{
    QSignalSpy spy(reply, SIGNAL(finished()));
    bool finished = spy.wait();
    QVERIFY2(finished, "No reply responsed");
    QCOMPARE(spy.count(), 1);
}

void TestQtMockWebServer::regularResponse()
{
    MockResponse response = MockResponse().setBody("hello world");
    m_mockServer->enqueue(response);
    m_mockServer->play();

    QUrl url = m_mockServer->obtainUrl("/");
    QNetworkRequest expReq(url);
    expReq.setRawHeader(QByteArray("Accept-Language"), QByteArray("en-US"));

    QNetworkReply *reply = m_mgr.get(expReq);
    waitForReply(reply);

    QVERIFY2(reply->error() == QNetworkReply::NoError,
             "Reply with error");
    QCOMPARE(reply->readAll(), QByteArray("hello world"));

    RecordedRequest actReq = m_mockServer->takeRequest();
    QCOMPARE(actReq.requestLine(), QString("GET / HTTP/1.1"));
    QCOMPARE(actReq.header("Accept-Language"), QString("en-US"));
    reply->deleteLater();
}

void TestQtMockWebServer::expect100ContinueWithBody()
{
    m_mockServer->enqueue(MockResponse());
    m_mockServer->play();

    QUrl url = m_mockServer->obtainUrl("/");
    QNetworkRequest expReq(url);
    expReq.setRawHeader(QByteArray("Expect"), QByteArray("100-continue"));

    QNetworkReply *reply = m_mgr.put(expReq, QByteArray("hello"));
    waitForReply(reply);

    RecordedRequest actReq = m_mockServer->takeRequest();
    QCOMPARE(m_mockServer->requestCount(), 1);
    QCOMPARE(actReq.requestLine(), QString("PUT / HTTP/1.1"));
    QCOMPARE(actReq.header("Expect"), QString("100-continue"));
    QCOMPARE(actReq.header("Content-Length"), QString::number(5));
    QCOMPARE(actReq.bodySize(), 5L);
    QCOMPARE(actReq.body(), QByteArray("hello"));

    reply->deleteLater();
}
