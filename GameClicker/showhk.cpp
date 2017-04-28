#include "showhk.h"
#include "ui_showhk.h"

ShowHK::ShowHK(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ShowHK)
{
    ui->setupUi(this);
}

ShowHK::~ShowHK()
{
    delete ui;
}

void ShowHK::on_pushButton_clicked()
{
    close();
}

void ShowHK::setText(QString s)
{
    ui->textBrowser->setText(s);
}
