#include "../include/Osoba.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
static std::string Plik = "../login.txt";
Osoba::Osoba(const std::string& numerKonta, const std::string& pin, const std::string& password, const std::string& imie,
    const std::string& nazwisko, const std::string& dataUrodzenia, const std::string& email,
    const std::string& miasto, const std::string& kodPocztowy, const std::string& ulica,
    const std::string& numerDomu, double stanKonta)
    : numerKonta(numerKonta), pin(pin), imie(imie), nazwisko(nazwisko),
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
        INSERT INTO users (numerKonta, pin, password, imie, nazwisko, dataUrodzenia, email, miasto, kodPocztowy, ulica, numerDomu, balance)
        VALUES (:numerKonta, :pin, :imie, :nazwisko, :dataUrodzenia, :email, :miasto, :kodPocztowy, :ulica, :numerDomu, :balance);
    )");

    query.bindValue(":numerKonta", QString::fromStdString(numerKonta));
    query.bindValue(":pin", QString::fromStdString(pin));
    query.bindValue(":password", QString::fromStdString(password));
    query.bindValue(":imie", QString::fromStdString(imie));
    query.bindValue(":nazwisko", QString::fromStdString(nazwisko));
    query.bindValue(":dataUrodzenia", QString::fromStdString(dataUrodzenia));
    query.bindValue(":email", QString::fromStdString(email));
    query.bindValue(":miasto", QString::fromStdString(miasto));
    query.bindValue(":kodPocztowy", QString::fromStdString(kodPocztowy));
    query.bindValue(":ulica", QString::fromStdString(ulica));
    query.bindValue(":numerDomu", QString::fromStdString(numerDomu));
    query.bindValue(":balance", stanKonta);

    if (!query.exec()) {
        qDebug() << "Błąd dodawania użytkownika: " << query.lastError().text();
        return false;
    }

    qDebug() << "Użytkownik dodany do bazy!";
    return true;
}

void Osoba::zapiszDoPliku() const {
    std::ofstream plik("./src/login.txt", std::ios::app);
    if (plik.is_open()) {
        plik << numerKonta << "," << pin << "," << imie << "," << nazwisko << ","
            << dataUrodzenia << "," << email << "," << miasto << ","
            << kodPocztowy << "," << ulica << "," << numerDomu << "," << stanKonta << "\n";
        plik.close();
    }
    else {
        std::cerr << "Nie można otworzyć pliku do zapisu!\n";
    }
}
void Osoba::dodajTransakcje(const std::string& opis, double stanPrzed, double stanPo) {
    std::ofstream file("./src/transactions.txt", std::ios::app); // Otwieramy plik w trybie dopisywania
    if (!file) {
        std::cerr << "Nie można otworzyć pliku transactions.txt do zapisu!" << std::endl;
        return;
    }

    // Pobranie aktualnej daty
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);
    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y");  // Format daty: DD-MM-YYYY
    std::string currentDate = oss.str();

    // Zapisujemy dane do pliku w nowym formacie
    file << imie << "," << nazwisko << "," << currentDate << ","
        << opis << "," << stanPrzed << "," << stanPo << "," << numerKonta << "\n";

    file.close();
}

bool Osoba::sprawdzLogowanie(const std::string& konto, const std::string& credential)
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Baza danych nie jest otwarta!";
        return false;
    }

    // rozpoznaj PIN: dokładnie 4 cyfry
    bool isPin = (credential.size() == 4 &&
        std::all_of(credential.begin(), credential.end(), ::isdigit));

    QSqlQuery query;
    query.prepare("SELECT pin, password FROM users WHERE id = :id");
    query.bindValue(":id", QString::fromStdString(konto));
    if (!query.exec() || !query.next()) {
        qDebug() << "Błąd zapytania lub brak konta:" << query.lastError().text();
        return false;
    }

    QString pinZBazy = query.value(0).toString();
    QString hasloZBazy = query.value(1).toString();
    QString credQt = QString::fromStdString(credential);

    // jeśli credential wygląda jak PIN → porównaj z pinZBazy, inaczej z hasloZBazy
    if (isPin)
        return (credQt == pinZBazy);
    else
        return (credQt == hasloZBazy);
}
std::string Osoba::generujNoweId()
{
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        qDebug() << "Baza danych nie jest otwarta!";
        return "";
    }

    QSqlQuery query;
    query.prepare("SELECT numerKonta FROM users ORDER BY numerKonta DESC LIMIT 1;");
    if (!query.exec() || !query.next()) {
        qDebug() << "Błąd zapytania lub brak danych: " << query.lastError().text();
        return "00000001"; // Jeśli brak kont, zwróć pierwsze
    }

    QString lastId = query.value(0).toString();
    int idNum = lastId.toInt();
    idNum += 1;

    return QString("%1").arg(idNum, 8, 10, QChar('0')).toStdString(); // np. 00000001
}