#include "./core/include/BankApp.h"
#include <QtWidgets/QApplication>
#include <fstream>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDriver>
#include <QDebug>
#include <QCoreApplication>

void createDatabase() {
    qDebug() << "Dostêpne sterowniki SQL:" << QSqlDatabase::drivers();

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    if (!db.isValid()) {
        qDebug() << "Sterownik QSQLITE nie jest dostêpny!";
        return;
    }

    db.setDatabaseName("./src/bank.db");
    if (!db.open()) {
        qDebug() << "B³¹d otwierania bazy danych:" << db.lastError().text();
        return;
    }
    else {
        qDebug() << "Baza danych otwarta.";
    }

    // Tabela u¿ytkowników
    QSqlQuery query;
    QString createUsersQuery = R"(
        CREATE TABLE IF NOT EXISTS users (
            id TEXT PRIMARY KEY CHECK (LENGTH(id) = 8),
            password TEXT NOT NULL,
            pin TEXT NOT NULL,
            first_name TEXT NOT NULL,
            last_name TEXT NOT NULL,
            birth_date TEXT NOT NULL,
            email TEXT UNIQUE NOT NULL,
            city TEXT NOT NULL,
            postal_code TEXT NOT NULL,
            street TEXT NOT NULL,
            house_number TEXT NOT NULL,
            balance REAL NOT NULL DEFAULT 0 CHECK (balance >= 0)
        );
    )";

    if (!query.exec(createUsersQuery)) {
        qDebug() << "B³¹d tworzenia tabeli users:" << query.lastError().text();
    }
    else {
        qDebug() << "Tabela users utworzona lub ju¿ istnieje.";
    }

    // Tabela transakcji
    QSqlQuery transactions;
    QString createTransactionsQuery = R"(
        CREATE TABLE IF NOT EXISTS transactions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id TEXT NOT NULL,
            transaction_date TEXT NOT NULL,
            transaction_type TEXT NOT NULL,
            balance_before REAL NOT NULL,
            balance_after REAL NOT NULL,
            account_number TEXT NOT NULL,
            FOREIGN KEY(user_id) REFERENCES users(id)
        );
    )";

    if (!transactions.exec(createTransactionsQuery)) {
        qDebug() << "B³¹d tworzenia tabeli transactions:" << transactions.lastError().text();
    }
    else {
        qDebug() << "Tabela transactions utworzona lub ju¿ istnieje.";
    }
}

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    BankApp w;
    createDatabase();
    w.show();
    return a.exec();
}
