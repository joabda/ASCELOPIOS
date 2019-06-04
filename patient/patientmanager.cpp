#include "patientmanager.h"
#include "ui_patientmanager.h"

static QString labels[22] = {"First Name", "Last Name", "Date of Birth",
                    "Weight", "Referent", "Phone Number", "Payer",
                    "Double Test", "Triple Test", "Last Menstruation's Date",
                    "Pregnancy Start", "Cranio Caudale Lenght", "Nuchal Translucency",
                    "Pregnancy", "Pregnancy", "Grouping", "Ultrasound", "Diabetes",
                    "Smoking", "Trisomy 21", "Neural Tube Malformation", "Race"};

PatientManager::PatientManager(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PatientManager)
{
    ui->setupUi(this);
}

void PatientManager::createLabels()
{
    for (int i = 0; i < 22; i++)
            labels_.push_back(new QLabel(labels[i]));
}

void PatientManager::addNew()
{
    QVBoxLayout* mainLayout = new QVBoxLayout;
    setWindowTitle("Creating a new Patient File");

    QVBoxLayout* generalLabels = new QVBoxLayout;
    QVBoxLayout* pregnantLabels = new QVBoxLayout;
    for (int i = 0; i < 7; i++)
    {
        if(i < 7)
            generalLabels->addWidget(labels_[i]);
        else
            pregnantLabels->addWidget(labels_[i]);
    }

    QHBoxLayout* general = new QHBoxLayout;
    general->addLayout(generalLabels);

    QHBoxLayout* pregnant = new QHBoxLayout;
    pregnant->addLayout(pregnantLabels);

    mainLayout->addLayout(general);
    mainLayout->addLayout(pregnant);
}

PatientManager::~PatientManager()
{
    for(auto& i : labels_)
        delete i;
    delete ui;
}
