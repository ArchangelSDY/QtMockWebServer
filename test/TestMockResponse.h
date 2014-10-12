#ifndef TESTMOCKRESPONSE_H
#define TESTMOCKRESPONSE_H

#include <QObject>

class TestMockResponse : public QObject
{
    Q_OBJECT
private slots:
    void defaultMockResponse();
    void setBodyAdjustHeaders();
    void addHeader();
    void setHeader();

};

#endif // TESTMOCKRESPONSE_H
