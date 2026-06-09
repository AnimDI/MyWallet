#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QSqlTableModel>
#include <QString>
#include <QDebug>
#include <QLineEdit>
#include <QPushButton>
#include <secondwindow.h>
// #include <secondwindow.h>
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private:

    QSqlDatabase db;
    QSqlQuery *query;
    QSqlTableModel *model;
    int userId;
    QString userLogin;
    QLineEdit *lineEditLogin;

    QLineEdit *lineEditPassword;
    QPushButton *pushButton;
    QPushButton *pushButton_2;
    secondwindow *window2;



private slots:
    void button_loginClick();
    void button_regClick();
};
#endif // WIDGET_H