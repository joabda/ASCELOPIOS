#ifndef QPUSHBUTTONEXTENDED_H
#define QPUSHBUTTONEXTENDED_H

#include <QPushButton>

class QPushButtonExtended : public QPushButton
{
    Q_OBJECT

public:
    explicit QPushButtonExtended(const char* text, QWidget* parent = nullptr);
    void setIndex(const int index);
    void button_clicked();

signals:
    void on_click(int index);

private:
    int index_;
};

#endif // QPUSHBUTTONEXTENDED_H
