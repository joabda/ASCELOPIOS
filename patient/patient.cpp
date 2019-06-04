#include "patient.h"

Patient::Patient(const QString& firstName, const QString& lastName, const QString & phoneNumber,
                 const double weight, const QDate& birth, const QString& referent, const QString& payer)
{
    firstName_ = firstName;
    lastName_ = lastName;
    phoneNumber_ = phoneNumber;
    weight_ = weight;
    birth_ = birth;
    referent_ = referent;
    payer_ = payer;
}

Patient::~Patient()
{

}
