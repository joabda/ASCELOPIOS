#ifndef QWIDGETEXTENDED_H
#define QWIDGETEXTENDED_H

#include <QWidget>
#include <QString>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QStyle>
#include <QStyleOption>
#include <QStylePainter>
#include "qframe_extended.h"

enum { PRESSED, RELEASED, MOVED };

class QWidgetExtended : public QWidget
{
    Q_OBJECT

public:
    explicit QWidgetExtended(QWidget* parent = nullptr);

protected:
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void paintEvent(QPaintEvent* pe);

signals:
    void mousePress(QFrameExtended* frame);
    void mouseRelease(QFrameExtended* frame);
    void mouseMove(QFrameExtended* frame);


private:
    void signalEvent(QMouseEvent* event, int code);
};

#endif
