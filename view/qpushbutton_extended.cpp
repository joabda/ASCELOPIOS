#include "qpushbutton_extended.h"

#include <QDebug>

QPushButtonExtended::QPushButtonExtended(const char* text, QWidget* parent) : QPushButton(text, parent)
{
    //clicked can't be overwritten
    connect(this, &QPushButtonExtended::clicked, this, &QPushButtonExtended::button_clicked);
}

void QPushButtonExtended::button_clicked()
{
    emit on_click(index_);
}

void QPushButtonExtended::setIndex(const int index)
{
    index_ = index;
}
