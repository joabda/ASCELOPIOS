#ifndef PATIENT_H
#define PATIENT_H

#include <QString>
#include <QDate>

class Patient
{
public:
    Patient(const QString& firstName, const QString& lastName, const QString& phoneNumber,
             const double weight, const QDate& birth, const QString& referent, const QString& payer);
    virtual ~Patient();

protected:
    QString firstName_;
    QString lastName_;
    QString phoneNumber_;
    double weight_;
    QDate birth_;
    QString referent_;
    QString payer_;
};

#endif // PATIENT_H
