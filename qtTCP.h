#pragma once

#include <QtWidgets/QWidget>
#include "ui_qtTCP.h"

class qtTCP : public QWidget
{
    Q_OBJECT

public:
    qtTCP(QWidget *parent = Q_NULLPTR);

private:
    Ui::qtTCPClass ui;
};
