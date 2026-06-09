#include "secondwindow.h"
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QListWidget>
#include <QLineEdit>
#include <QStatusBar>
#include <QSqlQuery>
#include <QSqlError>
#include <QColor>
#include <QDate>
#include <QDebug>

secondwindow::secondwindow(int userId, QString userLogin, QWidget *parent)
    : QMainWindow(parent), m_userId(userId), m_userLogin(userLogin)
{
    setWindowTitle("Мой кошелёк");
    resize(1010, 691);

    QWidget *central = new QWidget(this);
    setCentralWidget(central);

    QGridLayout *mainLayout = new QGridLayout(central);

    QHBoxLayout *topLayout = new QHBoxLayout();

    // баланс
    QGridLayout *balanceLayout = new QGridLayout();
    balance = new QLabel("0");
    balance->setStyleSheet("color: #2e7d5e; font-size: 30px; font-weight: 800;");
    QLabel *balance_label = new QLabel("Баланс:");
    balance_label->setStyleSheet("color: #2e7d5e; font-size: 30px; font-weight: 800;");
    balanceLayout->addWidget(balance_label, 0, 0);
    balanceLayout->addWidget(balance, 0, 1);

    doxod = new QLabel("0");
    doxod->setStyleSheet("color: #2e7d5e; font-size: 18px; font-weight: 700;");
    QLabel *doxod_label = new QLabel("Доходы:");
    doxod_label->setStyleSheet("color: #2e7d5e; font-size: 17px; font-weight: 700;");
    rasxod = new QLabel("0");
    rasxod->setStyleSheet("color: #d14545; font-size: 18px; font-weight: 700;");
    QLabel *rasxod_label = new QLabel("Расходы:");
    rasxod_label->setStyleSheet("color: #2e7d5e; font-size: 17px; font-weight: 700;");

    balanceLayout->addWidget(doxod_label, 1, 0);
    balanceLayout->addWidget(doxod, 1, 1);
    balanceLayout->addWidget(rasxod_label, 2, 0);
    balanceLayout->addWidget(rasxod, 2, 1);
    topLayout->addLayout(balanceLayout);

    // добавление
    QGridLayout *formLayout = new QGridLayout();
    QLabel *dateLabel = new QLabel("Дата");
    dateLabel->setAlignment(Qt::AlignCenter);
    QLabel *summLabel = new QLabel("Сумма");
    summLabel->setAlignment(Qt::AlignCenter);
    QLabel *commentLabel = new QLabel("Комментарий");
    commentLabel->setAlignment(Qt::AlignCenter);

    enterDate = new QLineEdit();
    enterSumm = new QLineEdit();
    enterSumm->setValidator(new QIntValidator(-999999999, 999999999, this));
    etnerText = new QLineEdit();

    formLayout->addWidget(dateLabel, 0, 0);
    formLayout->addWidget(summLabel, 1, 0);
    formLayout->addWidget(commentLabel, 2, 0);
    formLayout->addWidget(enterDate, 0, 2);
    formLayout->addWidget(enterSumm, 1, 2);
    formLayout->addWidget(etnerText, 2, 2);

    topLayout->addLayout(formLayout);

    // кнопка "+"
    pushButton = new QPushButton("+");
    pushButton->setFixedSize(80, 80);
    pushButton->setStyleSheet("background-color: #2e7d5e; color: white; font-size: 70px; font-weight: bold; border-radius: 40px; text-align: center;");
    topLayout->addWidget(pushButton);

    mainLayout->addLayout(topLayout, 0, 0);

    // Список и кнопки управления
    QHBoxLayout *bottomLayout = new QHBoxLayout();

    // кнопка "Удалить"
    reload_2 = new QPushButton("Удалить");
    reload_2->setFixedSize(80, 80);
    reload_2->setStyleSheet("background-color: #2e7d5e; color: white; font-size: 14px; font-weight: bold; border-radius: 40px; text-align: center;");

    list = new QListWidget();
    list->setMaximumSize(1000, 450);
    list->setFont(QFont("Arial", 14));
    list->setStyleSheet("background-color: white; border: 2px solid #2e7d5e; padding: 15px;");

    // кнопка "Обновить"
    reload = new QPushButton("Обновить");
    reload->setFixedSize(80, 80);
    reload->setStyleSheet("background-color: #2e7d5e; color: white; font-size: 14px; font-weight: bold; border-radius: 40px; text-align: center;");

    bottomLayout->addWidget(reload_2);
    bottomLayout->addWidget(list);
    bottomLayout->addWidget(reload);

    mainLayout->addLayout(bottomLayout, 1, 0);

    statusbar = new QStatusBar(this);
    setStatusBar(statusbar);

    // подключения сигналов
    connect(pushButton, &QPushButton::clicked, this, &secondwindow::on_pushButton_clicked);
    connect(reload, &QPushButton::clicked, this, &secondwindow::on_reload_clicked);
    connect(reload_2, &QPushButton::clicked, this, &secondwindow::on_reload_2_clicked);
    connect(list, &QListWidget::itemDoubleClicked, this, &secondwindow::on_list_itemDoubleClicked);

    // база данных
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./MyWalletDB.db");
    if (!db.open()) {
        qDebug() << "Ошибка открытия БД";
    }

    QSqlQuery query;
    query.exec("CREATE TABLE IF NOT EXISTS users(id INTEGER PRIMARY KEY AUTOINCREMENT, login TEXT, password TEXT);");
    query.exec("CREATE TABLE IF NOT EXISTS spending(id INTEGER PRIMARY KEY AUTOINCREMENT, user_id INTEGER, summ DOUBLE, type BOOLEAN, date TEXT, comment TEXT);");

    on_reload_clicked();
}

secondwindow::~secondwindow()
{
}

void secondwindow::on_pushButton_clicked()
{
    QSqlQuery query;
    int summ = enterSumm->text().toInt();
    QString coment = etnerText->text();
    QString date = enterDate->text();

    if (summ == 0 || coment.isEmpty() || date.isEmpty()) {
        statusbar->showMessage("Заполните все поля", 3000);
        return;
    }

    if (summ > 0) {
        query.prepare("INSERT INTO spending (user_id, summ, type, date, comment) VALUES (?, ?, 1, ?, ?)");
    } else {
        query.prepare("INSERT INTO spending (user_id, summ, type, date, comment) VALUES (?, ?, 0, ?, ?)");
    }

    query.addBindValue(m_userId);
    query.addBindValue(summ);
    query.addBindValue(date);
    query.addBindValue(coment);

    if (query.exec()) {
        statusbar->showMessage("Операция добавлена", 3000);
        enterSumm->clear();
        etnerText->clear();
        enterDate->clear();
        on_reload_clicked();
    } else {
        statusbar->showMessage("Ошибка: " + query.lastError().text(), 5000);
    }
}

void secondwindow::on_reload_clicked()
{
    list->clear();
    QSqlQuery query;
    query.prepare("SELECT id, date, summ, comment, type FROM spending WHERE user_id = ? ORDER BY date DESC");
    query.addBindValue(m_userId);
    query.exec();

    double endSumm = 0, doxodSum = 0, rasxodSum = 0;

    while (query.next()) {
        int id = query.value(0).toInt();
        QString date = query.value(1).toString();
        double summ = query.value(2).toDouble();
        QString comment = query.value(3).toString();
        int type = query.value(4).toInt();

        endSumm += summ;

        QListWidgetItem *item = new QListWidgetItem();
        item->setData(Qt::UserRole, id);
        item->setText("Дата: " + date + " Сумма: " + QString::number(summ) + " Комментарий: " + comment);

        if (type == 1) {
            item->setForeground(QColor(46, 125, 94));
            doxodSum += summ;
        } else {
            item->setForeground(QColor(209, 69, 69));
            rasxodSum += summ;
        }

        list->addItem(item);
    }

    balance->setText(QString::number(endSumm));
    rasxod->setText(QString::number(rasxodSum));
    doxod->setText(QString::number(doxodSum));
}

void secondwindow::on_reload_2_clicked()
{
    QListWidgetItem *currentItem = list->currentItem();
    if (!currentItem) {
        statusbar->showMessage("Не выбран элемент", 3000);
        return;
    }

    int id = currentItem->data(Qt::UserRole).toInt();
    QSqlQuery query;
    query.prepare("DELETE FROM spending WHERE id = ? AND user_id = ?");
    query.addBindValue(id);
    query.addBindValue(m_userId);

    if (query.exec()) {
        statusbar->showMessage("Удалено", 3000);
        on_reload_clicked();
    } else {
        statusbar->showMessage("Ошибка удаления: " + query.lastError().text(), 5000);
    }
}

void secondwindow::on_list_itemDoubleClicked(QListWidgetItem *item)
{
    int id = item->data(Qt::UserRole).toInt();
    // Здесь можно открыть окно редактора
    statusbar->showMessage("Редактирование записи " + QString::number(id), 3000);
}