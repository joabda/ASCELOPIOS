#ifndef QLABELEVENT_H
#define QLABELEVENT_H

#include <QLabel>
#include <QMouseEvent>
#include "model/event.h"

class QLabelEvent : public QLabel
{
    Q_OBJECT

public:
    explicit QLabelEvent(QWidget* parent = nullptr);
    ~QLabelEvent();
    bool markSelection(const bool selected);
    void setEvent(Event* event);
    Event* getEvent() const;
    bool drawUI();
    bool is_color_dark(const string& colorName);

protected:
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void keyPressEvent(QKeyEvent* e);

signals:
    void clicked(QLabelEvent *event, Qt::MouseButton button);
    void keyPressed(int key);

private:
    Event* event;
    bool drawInvalidEvent();

};

#endif // QLABELEVENT_H
