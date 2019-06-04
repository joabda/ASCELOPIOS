#ifndef PREGNANTPATIENT_H
#define PREGNANTPATIENT_H
#include "patient/patient.h"

class PregnantPatient : public Patient
{
public:
    PregnantPatient(const QString& firstName, const QString& lastName, const QString& phoneNumber,
                    const double weight, const QDate& birth, const QString& referent, const QString& payer);
    ~PregnantPatient();
private:

    QDate lastMenstruation_;
    QDate pregnancyStart_;
    double cranioLength_;

    unsigned pregnancy_; // Number of fetus

    bool echoConfirmation_;
    bool diabetes_;
    bool smoking_;
    bool trisomy21_;
    bool neuralTubeMalformation_;
};

#endif // PREGNANTPATIENT_H
