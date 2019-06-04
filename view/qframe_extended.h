#ifndef QFRAMEEXTENDED_H
#define QFRAMEEXTENDED_H

#include <QObject>
#include <QFrame>
#include <QDebug>
#include "util/dateutil.h"

class QFrameExtended : public QFrame
{
    Q_OBJECT

public:
    explicit QFrameExtended(QWidget* parent = nullptr) : QFrame(parent), date_(nullptr) { }
    ~QFrameExtended() { delete date_; }
    Date* getDate() const { return date_; }
    void setDate(Date* d)
    {
        if (date_ != nullptr)
            delete date_;
        date_ = d;
    }

private:
    Date* date_;
};

#endif // QFRAMEEXTENDED_H
