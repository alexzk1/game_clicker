#ifndef SHOWHK_H
#define SHOWHK_H

#include <QtWidgets/QDialog>

namespace Ui {
class ShowHK;
}

class ShowHK : public QDialog
{
    Q_OBJECT
    
public:
    explicit ShowHK(QWidget *parent = 0);
    ~ShowHK();
    void setText(QString s);
private slots:
    void on_pushButton_clicked();

private:
    Ui::ShowHK *ui;
};

#endif // SHOWHK_H
