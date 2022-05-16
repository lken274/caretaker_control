#ifndef VITALSBOX_H
#define VITALSBOX_H

#include <QFrame>
#include <QPushButton>
#include <QLabel>

class VitalsBox : public QFrame
{
    Q_OBJECT
public:
    explicit VitalsBox(QFrame *parent = 0, QString valueColor = "red");
    bool SetValue(int value);
    bool SetValue(int value1, int value2);

signals:

public slots:

protected:
    QPushButton *mIcon;
    QLabel      *mLabel;
    QLabel      *mValue;
};

#endif // VITALSBOX_H
