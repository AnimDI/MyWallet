#ifndef SECONDWINDOW_H
#define SECONDWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QListWidgetItem>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <QStatusBar>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QIntValidator>
#include <redactor.h>
class secondwindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit secondwindow(int userId, QString userLogin, QWidget *parent = nullptr);
    ~secondwindow();

private slots:
    void on_pushButton_clicked();
    void on_reload_clicked();
    void on_reload_2_clicked();
    void on_list_itemDoubleClicked(QListWidgetItem *item);

private:
    int m_userId;
    QString m_userLogin;

    QListWidget *list;
    QLineEdit *enterSumm;
    QLineEdit *etnerText;
    QLineEdit *enterDate;
    QPushButton *pushButton;
    QPushButton *reload;
    QPushButton *reload_2;
    QLabel *balance;
    QLabel *rasxod;
    QLabel *doxod;
    QStatusBar *statusbar;
    QLabel *status;
    redactor *editor;
    QSqlDatabase db;
};

#endif // SECONDWINDOW_H