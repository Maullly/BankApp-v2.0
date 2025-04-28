#include "./core/include/BankApp.h"
#include <QtWidgets/QApplication>
#include <fstream>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
void checkAndCreateFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file) {  // Jeœli plik nie istnieje, utwórz go
        std::ofstream newFile(filename);
        newFile.close();
    }
}
void createDatabase() {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("bank.db");
    if (!db.open()) {
        qDebug() << "B³¹d otwierania bazy danych:" << db.lastError().text();
        return;
	}
    else {
        qDebug() << "Baza danych otwarta.";
    }
    // Utworzenie tabeli uzytkownikow
    QSqlQuery query;
    QString createTableQuery =
        "CREATE TABLE IF NOT EXISTS users ("
        "id TEXT PRIMARY KEY CHECK (LENGTH(id) = 8), "  // ID jako tekst (np. "00000001")
		"password TEXT NOT NULL, "
        "pin INTEGER NOT NULL CHECK (pin BETWEEN 1000 AND 9999), "
        "first_name TEXT NOT NULL, "
        "last_name TEXT NOT NULL, "
        "birth_date TEXT NOT NULL, "
        "email TEXT UNIQUE NOT NULL, "
        "city TEXT NOT NULL, "
        "postal_code TEXT NOT NULL, "
        "street TEXT NOT NULL, "
        "house_number TEXT NOT NULL, "
        "balance REAL NOT NULL DEFAULT 0 CHECK (balance >= 0) "
        ");";

    if (!query.exec(createTableQuery)) {
        qDebug() << "B³¹d tworzenia tabeli:" << query.lastError().text();
    }
    else {
        qDebug() << "Tabela utworzona lub ju¿ istnieje.";
    }

    QSqlQuery transactions; // do poprawy 
    QString createTable = R"(
        CREATE TABLE IF NOT EXISTS transactions (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            imie TEXT NOT NULL,
            nazwisko TEXT NOT NULL,
            data TEXT NOT NULL,
            rodzaj TEXT NOT NULL,
            balans_przed REAL NOT NULL,
            balans_po REAL NOT NULL,
            numer_konta INTEGER NOT NULL
        );
    )";

    if (!transactions.exec(createTable)) {
        qDebug() << "B³¹d tworzenia tabeli transactions: " << query.lastError().text();
	}
    else {
        qDebug() << "Tabela transactions utworzona lub ju¿ istnieje.";
    }
}
int main(int argc, char *argv[])
{
    // Sprawdzenie i utworzenie plików, jeœli nie istniej¹
    checkAndCreateFile("./src/login.txt");
    checkAndCreateFile("./src/transactions.txt");
    QApplication a(argc, argv);
	createDatabase();
    BankApp w;
    w.show();
    return a.exec();
}
