#ifndef PATIENTMANAGER_H
#define PATIENTMANAGER_H

#include <QDialog>
#include <QVector>
#include <QVBoxLayout>
#include <QLabel>
#include "patient/pregnantpatient.h"

namespace Ui {
class PatientManager;
}

class PatientManager : public QDialog
{
    Q_OBJECT

public:
    explicit PatientManager(QWidget* parent = nullptr);
    void addNew();
    ~PatientManager();

private:
    Ui::PatientManager* ui;
    QVector<QLabel*> labels_;
    QVector<Patient*> container_;
    void createLabels();
};

#endif // PATIENTMANAGER_H
