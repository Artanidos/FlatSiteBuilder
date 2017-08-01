#include "moduldialog.h"
#include "flatbutton.h"
#include <QGridLayout>
#include <QPushButton>

ModulDialog::ModulDialog()
{
    m_result = 0;
    setWindowTitle("Insert Module");
    QGridLayout *grid = new QGridLayout();
    FlatButton *b1 = new FlatButton(":/images/text_normal.png", ":/images/text_hover.png");
    FlatButton *b2 = new FlatButton(":/images/image_normal.png", ":/images/image_hover.png");
    FlatButton *b3 = new FlatButton(":/images/slider_normal.png", ":/images/slider_hover.png");

    grid->addWidget(b1, 0, 0);
    grid->addWidget(b2, 0, 1);
    grid->addWidget(b3, 0, 2);

    QPushButton *closeButton = new QPushButton("Close");

    QHBoxLayout *buttonsLayout = new QHBoxLayout;
    buttonsLayout->addStretch(1);
    buttonsLayout->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(grid);
    mainLayout->addStretch(1);
    mainLayout->addSpacing(12);
    mainLayout->addLayout(buttonsLayout);
    setLayout(mainLayout);

    connect(closeButton, SIGNAL(clicked(bool)), this, SLOT(close()));
    connect(b1, SIGNAL(clicked()), this, SLOT(close1()));
    connect(b2, SIGNAL(clicked()), this, SLOT(close2()));
    connect(b3, SIGNAL(clicked()), this, SLOT(close3()));
}

void ModulDialog::close1()
{
    setResult(1);
    close();
}

void ModulDialog::close2()
{
    setResult(2);
    close();
}

void ModulDialog::close3()
{
    setResult(3);
    close();
}
