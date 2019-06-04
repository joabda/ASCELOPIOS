#include "pregnantpatient.h"

PregnantPatient::PregnantPatient(const QString& firstName, const QString& lastName, const QString & phoneNumber,
                                 const double weight, const QDate& birth, const QString& referent, const QString& payer)
    : Patient (firstName, lastName, phoneNumber, weight, birth, referent, payer)
{

}

PregnantPatient::~PregnantPatient()
{

}
