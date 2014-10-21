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

    QUrl url = m_mockServer->getUrl("/");
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

    QUrl url = m_mockServer->getUrl("/");
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

void TestQtMockWebServer::expect100ContinueWithoutBody()
{
    m_mockServer->enqueue(MockResponse());
    m_mockServer->play();

    QUrl url = m_mockServer->getUrl("/");
    QNetworkRequest expReq(url);
    expReq.setRawHeader(QByteArray("Expect"), QByteArray("100-continue"));

    QNetworkReply *reply = m_mgr.put(expReq, QByteArray());
    waitForReply(reply);

    RecordedRequest actReq = m_mockServer->takeRequest();
    QCOMPARE(m_mockServer->requestCount(), 1);
    QCOMPARE(actReq.requestLine(), QString("PUT / HTTP/1.1"));
    QCOMPARE(actReq.header("Expect"), QString("100-continue"));
    QCOMPARE(actReq.header("Content-Length"), QString::number(0));
    QCOMPARE(actReq.bodySize(), 0L);
    QVERIFY2(actReq.body().isEmpty(), "Recorded request body is not empty");

    reply->deleteLater();
}

void TestQtMockWebServer::redirect()
{
    // Server should run first to ensure port is ready and url can be obtained
    m_mockServer->play();

    m_mockServer->enqueue(MockResponse()
            .setStatus("HTTP/1.1 302 Found")
            .addHeader("Location",
                       m_mockServer->getUrl("/new-path").toString())
            .setBody("This page has moved!"));
    m_mockServer->enqueue(MockResponse()
            .setBody("This is the new location!"));

    QNetworkRequest expFirst(m_mockServer->getUrl("/"));
    QNetworkReply *replyFirst = m_mgr.get(expFirst);
    waitForReply(replyFirst);

    QCOMPARE(replyFirst->readAll(), QByteArray("This page has moved!"));
    QUrl redirectUrl = replyFirst->attribute(
        QNetworkRequest::RedirectionTargetAttribute).toUrl();
    QCOMPARE(redirectUrl, m_mockServer->getUrl("/new-path"));
    replyFirst->deleteLater();

    QNetworkRequest expRedirect = QNetworkRequest(redirectUrl);
    QNetworkReply *replyRedirect = m_mgr.get(expRedirect);
    waitForReply(replyRedirect);

    QCOMPARE(replyRedirect->readAll(), QByteArray("This is the new location!"));
    replyRedirect->deleteLater();

    RecordedRequest actFirst = m_mockServer->takeRequest();
    QCOMPARE(actFirst.requestLine(), QString("GET / HTTP/1.1"));
    RecordedRequest actRedirect = m_mockServer->takeRequest();
    QCOMPARE(actRedirect.requestLine(), QString("GET /new-path HTTP/1.1"));
}
