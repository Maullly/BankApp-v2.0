#include "../include/Osoba.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QRegularExpression> 
static std::string Plik = "../login.txt";
Osoba::Osoba(const std::string& numerKonta, const std::string& password, const std::string& pin, const std::string& imie,
    const std::string& nazwisko, const std::string& dataUrodzenia, const std::string& email,
    const std::string& miasto, const std::string& kodPocztowy, const std::string& ulica,
    const std::string& numerDomu, double stanKonta)
    : numerKonta(numerKonta), password(password), pin(pin), imie(imie), nazwisko(nazwisko),
    dataUrodzenia(dataUrodzenia), email(email), miasto(miasto),
    kodPocztowy(kodPocztowy), ulica(ulica), numerDomu(numerDomu), stanKonta(stanKonta) {
}




std::string Osoba::getNumerKonta() const { return numerKonta; }
std::string Osoba::getPassword() const { return password; }
std::string Osoba::getPin() const { return pin; }
std::string Osoba::getImie() const { return imie; }
std::string Osoba::getNazwisko() const { return nazwisko; }
std::string Osoba::getDataUrodzenia() const { return dataUrodzenia; }
std::string Osoba::getEmail() const { return email; }
std::string Osoba::getMiasto() const { return miasto; }
std::string Osoba::getKodPocztowy() const { return kodPocztowy; }
std::string Osoba::getUlica() const { return ulica; }
std::string Osoba::getNumerDomu() const { return numerDomu; }
double Osoba::getStanKonta() const { return stanKonta; }


void Osoba::setPin(const std::string& newPin) { pin = newPin; }
void Osoba::setPassword(const std::string& newPassword) { password = newPassword; }
void Osoba::setEmail(const std::string& newEmail) { email = newEmail; }

bool Osoba::wyplacSrodki(double kwota) {
    if (stanKonta >= kwota) {
        stanKonta -= kwota;
        return true;
    }
    return false;
}

bool Osoba::dodajDoBazy() {
    QSqlQuery query;
    query.prepare(R"(
        INSERT INTO users (id, password, pin, first_name, last_name, birth_date, email, city, postal_code, street, house_number, balance)
        VALUES (:id, :password, :pin, :first_name, :last_name, :birth_date, :email, :city, :postal_code, :street, :house_number, :balance);
    )");

    query.bindValue(":id", QString::fromStdString(numerKonta));
    query.bindValue(":password", QString::fromStdString(password));
    query.bindValue(":pin", QString::fromStdString(pin));
    query.bindValue(":first_name", QString::fromStdString(imie));
    query.bindValue(":last_name", QString::fromStdString(nazwisko));
    query.bindValue(":birth_date", QString::fromStdString(dataUrodzenia));
    query.bindValue(":email", QString::fromStdString(email));
    query.bindValue(":city", QString::fromStdString(miasto));
    query.bindValue(":postal_code", QString::fromStdString(kodPocztowy));
    query.bindValue(":street", QString::fromStdString(ulica));
    query.bindValue(":house_number", QString::fromStdString(numerDomu));
    query.bindValue(":balance", stanKonta);

    if (!query.exec()) {
        qDebug() << "Błąd dodawania użytkownika: " << query.lastError().text();
        return false;
    }

    qDebug() << "Użytkownik dodany do bazy!";
    return true;
}



void Osoba::dodajTransakcje(const std::string& opis, double stanPrzed, double stanPo) {
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Baza danych nie jest otwarta!";
        return;
    }

    QSqlQuery insert;
    QString currentDate = QDate::currentDate().toString("yyyy-MM-dd");

    insert.prepare(R"(
        INSERT INTO transactions (user_id, transaction_date, transaction_type, balance_before, balance_after, account_number)
        VALUES (:user_id, :transaction_date, :transaction_type, :balance_before, :balance_after, :account_number)
    )");

    insert.bindValue(":user_id", QString::fromStdString(numerKonta)); 
    insert.bindValue(":transaction_date", currentDate);
    insert.bindValue(":transaction_type", QString::fromStdString(opis));
    insert.bindValue(":balance_before", stanPrzed);
    insert.bindValue(":balance_after", stanPo);
    insert.bindValue(":account_number", QString::fromStdString(numerKonta));

    if (!insert.exec()) {
        qDebug() << "Błąd zapisu transakcji:" << insert.lastError().text();
    }
    else {
        qDebug() << "Transakcja zapisana pomyślnie.";
    }
}

bool Osoba::sprawdzHaslo(const std::string& konto, const std::string& password)
{
    QSqlQuery query;
    query.prepare("SELECT password FROM users WHERE id = :id");
    query.bindValue(":id", QString::fromStdString(konto));

    if (!query.exec()) {
        qDebug() << "Błąd zapytania hasła: " << query.lastError().text();
        return false;
    }

    if (query.next()) {
        QString correctPassword = query.value("password").toString();
        return password == correctPassword.toStdString();
    }

    return false;
}
bool Osoba::sprawdzPin(const std::string& konto, const std::string& pin)
{
    QSqlQuery query;
    query.prepare("SELECT pin FROM users WHERE id = :id");
    query.bindValue(":id", QString::fromStdString(konto));

    if (!query.exec()) {
        qDebug() << "Błąd zapytania PIN-u: " << query.lastError().text();
        return false;
    }

    if (query.next()) {
        QString correctPin = query.value("pin").toString();
        return pin == correctPin.toStdString();
    }

    return false;
}
std::string Osoba::generujNoweId()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Baza danych nie jest otwarta!";
        return "";
    }

    QSqlQuery query(db);
    QString sql = "SELECT id FROM users ORDER BY CAST(id AS INTEGER) DESC LIMIT 1;";
    if (!query.prepare(sql)) {
        qDebug() << "Nie udało się przygotować zapytania:" << sql << query.lastError().text();
        return "";
    }

    if (!query.exec()) {
        qDebug() << "Błąd wykonania zapytania:" << sql << query.lastError().text();
        return "";
    }

    if (!query.next()) {
        qDebug() << "Brak danych w tabeli users – pierwsze konto.";
        return "00000001";
    }

    QString lastId = query.value(0).toString();
    bool ok;
    int idNum = lastId.toInt(&ok);
    if (!ok) {
        qDebug() << "Nieprawidłowy format id:" << lastId;
        return "";
    }

    idNum += 1;

    return QString("%1").arg(idNum, 8, 10, QChar('0')).toStdString();
}
