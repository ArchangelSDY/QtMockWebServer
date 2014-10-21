QtMockWebServer [![Build Status](https://travis-ci.org/ArchangelSDY/QtMockWebServer.svg?branch=master)](https://travis-ci.org/ArchangelSDY/QtMockWebServer)
===============

A mock web server implemented using Qt for testing HTTP clients. This project
is inspired by the mock web server in project [okhttp](https://github.com/square/okhttp/tree/master/mockwebserver),
with small changes to API names to make it a bit more Qt style.
Thanks to their great work!


### Example

```cpp
// Create a QtMockWebServer.
MockWebServer server;

// Schedule some responses.
server.enqueue(MockResponse().setBody("hello, world!"));
server.enqueue(MockResponse().setBody("sup, bra?"));
server.enqueue(MockResponse().setBody("yo dog"));

// Start the server.
server.play();

// Ask the server for its URL. You'll need this to make HTTP requests.
QUrl baseUrl = server.getUrl("/v1/chat/");

// Exercise your application code, which should make those HTTP requests.
// Responses are returned in the same order that they are enqueued.
Chat chat = new Chat(baseUrl);

chat.loadMore();
assertEquals("hello, world!", chat.messages());

chat.loadMore();
chat.loadMore();
QCOMPARE(chat.messages(), QString("hello, world!\nsup, bra?\nyo dog"));

// Optional: confirm that your app made the HTTP requests you were expecting.
RecordedRequest request1 = server.takeRequest();
QCOMPARE(request1.path(), QString("/v1/chat/messages/"));
QVERIFY2(!request1.header("Authorization").isEmpty(), "Authorization header is empty.");

RecordedRequest request2 = server.takeRequest();
QCOMPARE(request2.path(), QString("/v1/chat/messages/2"));

RecordedRequest request3 = server.takeRequest();
QCOMPARE(request3.path(), QString("/v1/chat/messages/3"));

// Shut down the server. Instances cannot be reused.
server.shutdown();
```


### TODO

* HTTPS support.
* Throttle control.


### License

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

       http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.

