#include "qlabel_event.h"

#include <QDebug>

QLabelEvent::QLabelEvent(QWidget* parent) : QLabel(parent)
{
    event = nullptr;
}

QLabelEvent::~QLabelEvent() {
    if (event != nullptr)
        delete event;
}

void QLabelEvent::mousePressEvent(QMouseEvent* e )
{
    emit clicked(this, e->button());
}

void QLabelEvent::keyPressEvent(QKeyEvent* e)
{
    emit keyPressed(e->key());
}

void QLabelEvent::setEvent(Event* eventParam)
{
    if (event != nullptr)
        delete event;
    event = eventParam;
    drawUI();
}

Event* QLabelEvent::getEvent() const
{
   return event;
}

bool QLabelEvent::markSelection(const bool selected)
{
    if (event == nullptr)
        return false;
    if (selected)
        setObjectName("selected");
    else
        setObjectName("");
    //Update the ui
    setStyleSheet(this->styleSheet());
    return true;
}

bool QLabelEvent::drawInvalidEvent()
{
    if (event == nullptr)
        return false;
    setText(QString("<s>") + QString(this->event->getName().c_str()) + QString("</s>"));
    setStyleSheet(QString("QLabel { font-size: 14px; background-color : #AAAAAA; color: #000000; };"));
    setFixedHeight(26);
    setMargin(0);
    setEnabled(false);
    return true;
}

bool QLabelEvent::drawUI()
{
    if (event == nullptr)
        return false;
    if (event->isInvalid())
        return drawInvalidEvent();

    setText(event->getName().c_str());
    QString textColor("#000000");
    if (is_color_dark(event->getCategory()->getColor()))
        textColor = "#FFFFFF";
    setStyleSheet(QString("QLabel#selected {border-top: 1px solid #FF0000; border-bottom: 1px solid #FF0000; font-weight: bold; font-size: 13px;} QLabel { font-size: 14px; border-radius: 2px; background-color : ") + QString(this->event->getCategory()->getColor().c_str()) + QString("; color: ") + textColor + QString("};"));
    setFixedHeight(26);
    setMargin(0);
    QString tooltip_text;
    if (event->getName().length() > 20)
        tooltip_text = QString("<b>Name: </b>") + event->getName().c_str() + QString("\n");
    if (event->getPlace() != "")
        tooltip_text = tooltip_text + QString("<b>Place: </b>") + event->getPlace().c_str() + QString("\n");
    if (event->getDescription() != "")
        tooltip_text = tooltip_text + QString("<b>Description: </b>") + event->getDescription().c_str();
    setToolTip(tooltip_text);
    return true;
}

/* This is an utility function and should be moved in another class, but for the moment no one else is using it. */
bool QLabelEvent::is_color_dark(const string& colorName)
{
    QColor color(colorName.c_str());
    //Formula to calculate luminance from ITU-R BT.709
    int colorLuminance = int(0.2126 * color.red() + 0.7152 * color.green() + 0.0722 * color.blue());
    if (colorLuminance < 50)
        return true;
    else
        return false;
}

