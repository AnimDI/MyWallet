
#include "widget.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpacerItem>
#include <QDebug>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("MainWindow");
    resize(800, 600);

    QGridLayout *gridLayout = new QGridLayout(this);

    QLabel *label_2 = new QLabel("ВОЙДИТЕ В АККАУНТ");
    label_2->setAlignment(Qt::AlignCenter);
    label_2->setStyleSheet(
        "QLabel {"
        "    color: #2e7d5e;"
        "    font-family: 'Segoe UI', 'Arial', sans-serif;"
        "    font-size: 32px;"
        "    font-weight: 700;"
        "    background-color: transparent;"
        "}"
        );
    gridLayout->addWidget(label_2, 0, 0);

    QVBoxLayout *verticalLayout = new QVBoxLayout();

    QHBoxLayout *horizontalLayout = new QHBoxLayout();
    QLabel *label = new QLabel("Логин:");
    label->setStyleSheet(
        "QLabel {"
        "    color: #2e7d5e;"
        "    font-family: 'Segoe UI', 'Arial', sans-serif;"
        "    font-size: 16px;"
        "    font-weight: 700;"
        "    background-color: transparent;"
        "}"
        );
    lineEditLogin = new QLineEdit();
    lineEditLogin->setStyleSheet(
        "QLineEdit {"
        "    background-color: white;"
        "    color: #1a3b2e;"
        "    font-family: 'Segoe UI', 'Arial', sans-serif;"
        "    font-size: 13px;"
        "    font-weight: 500;"
        "    border: 1.5px solid #2e7d5e;"
        "    border-radius: 18px;"
        "    padding: 4px 10px;"
        "    min-width: 140px;"
        "    min-height: 8px;"
        "    max-height: 24px;"
        "}"
        );
    horizontalLayout->addWidget(label);
    horizontalLayout->addWidget(lineEditLogin);
    verticalLayout->addLayout(horizontalLayout);

    QHBoxLayout *horizontalLayout_5 = new QHBoxLayout();
    QLabel *label_4 = new QLabel("Пароль:");
    label_4->setStyleSheet(
        "QLabel {"
        "    color: #2e7d5e;"
        "    font-family: 'Segoe UI', 'Arial', sans-serif;"
        "    font-size: 16px;"
        "    font-weight: 700;"
        "    background-color: transparent;"
        "}"
        );
    lineEditPassword = new QLineEdit();
    lineEditPassword->setEchoMode(QLineEdit::Password);
    lineEditPassword->setStyleSheet(
        "QLineEdit {"
        "    background-color: white;"
        "    color: #1a3b2e;"
        "    font-family: 'Segoe UI', 'Arial', sans-serif;"
        "    font-size: 13px;"
        "    font-weight: 500;"
        "    border: 1.5px solid #2e7d5e;"
        "    border-radius: 18px;"
        "    padding: 4px 10px;"
        "    min-width: 140px;"
        "    min-height: 8px;"
        "    max-height: 24px;"
        "}"
        );
    horizontalLayout_5->addWidget(label_4);
    horizontalLayout_5->addWidget(lineEditPassword);
    verticalLayout->addLayout(horizontalLayout_5);

    QHBoxLayout *horizontalLayout_3 = new QHBoxLayout();
    horizontalLayout_3->setStretch(0, 0);
    horizontalLayout_3->setStretch(1, 1);
    horizontalLayout_3->setStretch(2, 1);
    horizontalLayout_3->setStretch(3, 0);

    QSpacerItem *horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_3->addSpacerItem(horizontalSpacer);

    pushButton = new QPushButton("Войти");
    pushButton->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    color: #2e7d5e;"
        "    font-family: 'Segoe UI', 'Arial', sans-serif;"
        "    font-size: 16px;"
        "    font-weight: 700;"
        "    border: 2px solid #2e7d5e;"
        "    border-radius: 10px;"
        "    padding: 10px 22px;"
        "    min-width: 100px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #2e7d5e;"
        "    color: white;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1a5b42;"
        "    border-color: #1a5b42;"
        "}"
        );
    horizontalLayout_3->addWidget(pushButton);

    pushButton_2 = new QPushButton("Регистрация");
    pushButton_2->setStyleSheet(
        "QPushButton {"
        "    background-color: transparent;"
        "    color: #2e7d5e;"
        "    font-family: 'Segoe UI', 'Arial', sans-serif;"
        "    font-size: 16px;"
        "    font-weight: 700;"
        "    border: 2px solid #2e7d5e;"
        "    border-radius: 10px;"
        "    padding: 10px 22px;"
        "    min-width: 100px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #2e7d5e;"
        "    color: white;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1a5b42;"
        "    border-color: #1a5b42;"
        "}"
        );
    horizontalLayout_3->addWidget(pushButton_2);

    QSpacerItem *horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
    horizontalLayout_3->addSpacerItem(horizontalSpacer_2);

    verticalLayout->addLayout(horizontalLayout_3);

    gridLayout->addLayout(verticalLayout, 1, 0);

    connect(pushButton, &QPushButton::clicked, this, &Widget::button_loginClick);
    connect(pushButton_2, &QPushButton::clicked, this, &Widget::button_regClick);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./MyWalletDB.db");
    if(db.open()){
        qDebug() << "База данных открыта";
    } else {
        qDebug() << "Ошибка открытия БД";
    }

    query = new QSqlQuery(db);
    query->exec("CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY AUTOINCREMENT, login TEXT, password TEXT);");
    query->exec("CREATE TABLE IF NOT EXISTS spending(id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INTEGER, summ DOUBLE, type BOOLEAN, date TEXT, comment TEXT, FOREIGN KEY(user_id) REFERENCES users(id));");

    userId = -1;
    userLogin = "";
}

Widget::~Widget()
{
    delete query;
}

void Widget::button_loginClick()
{
    QString login = lineEditLogin->text();
    QString password = lineEditPassword->text();

    QSqlQuery query;
    query.prepare("SELECT id, password FROM users WHERE login = ?");
    query.addBindValue(login);
    query.exec();

    if (query.next()) {
        int id = query.value(0).toInt();
        QString correctPassword = query.value(1).toString();

        if (correctPassword == password) {
            userId = id;
            userLogin=login;
            window2 = new secondwindow(userId,userLogin);
            window2->show();
            this->close();
            //window2.show();

        } else {
        qDebug() << "Пользователь не найден";
    }
    }
}
void Widget::button_regClick()
{
    QString login = lineEditLogin->text();
    QString password = lineEditPassword->text();

    if (login.isEmpty() || password.isEmpty()) {
        qDebug() << "Заполните логин и пароль";
        return;
    }

    QSqlQuery query;

    query.prepare("SELECT login FROM userd WHERE login=?");
    query.addBindValue(login);
    if(query.exec()){
        if(query.next()){
            qDebug()<<"данный логин уже используется";
            return;
        }

    }



    query.prepare("INSERT INTO users (login, password) VALUES (?, ?)");
    query.addBindValue(login);
    query.addBindValue(password);

    if (query.exec()) {

        lineEditLogin->clear();
        lineEditPassword->clear();
    }
}