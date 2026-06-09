#include "redactor.h"
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSqlQuery>
#include <QDebug>

redactor::redactor(int userId, int recordId, QWidget *parent)
    : QDialog(parent), m_userId(userId), m_recordId(recordId)
{
    setWindowTitle("Редактирование");
    setFixedSize(600, 400);

    QWidget *central = new QWidget(this);
    QGridLayout *mainLayout = new QGridLayout(central);
    setLayout(mainLayout);

    QLabel *dateLabel = new QLabel("Дата");
    dateLabel->setAlignment(Qt::AlignCenter);
    QLabel *summLabel = new QLabel("Сумма");
    summLabel->setAlignment(Qt::AlignCenter);
    QLabel *commentLabel = new QLabel("Комментарий");
    commentLabel->setAlignment(Qt::AlignCenter);

    enterDate = new QLineEdit();
    enterDate->setStyleSheet("QLineEdit { background-color: white; color: #1a3b2e; font-family: 'Segoe UI', 'Arial', sans-serif; font-size: 13px; font-weight: 500; border: 1.5px solid #2e7d5e; border-radius: 18px; padding: 4px 10px; min-width: 140px; min-height: 8px; max-height: 24px; }");

    enterSumm = new QLineEdit();
    enterSumm->setStyleSheet("QLineEdit { background-color: white; color: #1a3b2e; font-family: 'Segoe UI', 'Arial', sans-serif; font-size: 13px; font-weight: 500; border: 1.5px solid #2e7d5e; border-radius: 18px; padding: 4px 10px; min-width: 140px; min-height: 8px; max-height: 24px; }");

    etnerText = new QLineEdit();
    etnerText->setStyleSheet("QLineEdit { background-color: white; color: #1a3b2e; font-family: 'Segoe UI', 'Arial', sans-serif; font-size: 13px; font-weight: 500; border: 1.5px solid #2e7d5e; border-radius: 18px; padding: 4px 10px; min-width: 140px; min-height: 8px; max-height: 24px; }");

    saveButton = new QPushButton("Сохранить");
    saveButton->setStyleSheet("QPushButton { background-color: transparent; color: #2e7d5e; font-family: 'Segoe UI', 'Arial', sans-serif; font-size: 16px; font-weight: 700; border: 2px solid #2e7d5e; border-radius: 10px; padding: 10px 22px; min-width: 100px; } QPushButton:hover { background-color: #2e7d5e; color: white; } QPushButton:pressed { background-color: #1a5b42; border-color: #1a5b42; }");

    mainLayout->addWidget(dateLabel, 0, 0);
    mainLayout->addWidget(summLabel, 1, 0);
    mainLayout->addWidget(commentLabel, 2, 0);
    mainLayout->addWidget(enterDate, 0, 2);
    mainLayout->addWidget(enterSumm, 1, 2);
    mainLayout->addWidget(etnerText, 2, 2);
    mainLayout->addWidget(saveButton, 3, 0, 1, 3);

    QSqlQuery query;
    query.prepare("SELECT date, summ, comment, type FROM spending WHERE id = ? AND user_id = ?");
    query.addBindValue(m_recordId);
    query.addBindValue(m_userId);

    if (query.exec() && query.next()) {
        enterDate->setText(query.value(0).toString());
        enterSumm->setText(QString::number(query.value(1).toDouble()));
        etnerText->setText(query.value(2).toString());
    }

    connect(saveButton, &QPushButton::clicked, this, &redactor::on_saveButton_clicked);
}

redactor::~redactor() {}

void redactor::on_saveButton_clicked()
{
    QString date = enterDate->text();
    int summ = enterSumm->text().toInt();
    QString comment = etnerText->text();

    QSqlQuery query;

    if (summ > 0) {
        query.prepare("UPDATE spending SET summ = ?, type = 1, date = ?, comment = ? WHERE id = ? AND user_id = ?");
    } else {
        query.prepare("UPDATE spending SET summ = ?, type = 0, date = ?, comment = ? WHERE id = ? AND user_id = ?");
    }

    query.addBindValue(summ);
    query.addBindValue(date);
    query.addBindValue(comment);
    query.addBindValue(m_recordId);
    query.addBindValue(m_userId);

    if (query.exec()) {
        this->close();
    } else {
        qDebug() << "Ошибка обновления";
    }
}