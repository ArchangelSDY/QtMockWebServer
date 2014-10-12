#ifndef TESTRECORDEDREQUEST_H
#define TESTRECORDEDREQUEST_H

#include <QObject>

class TestRecordedRequest : public QObject
{
    Q_OBJECT
private slots:
    void accessors();
};

#endif // TESTRECORDEDREQUEST_H
