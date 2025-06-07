#include "AdminPanel.h"
#include <QMessageBox>
#include <QInputDialog>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQueryModel>
#include <QProcess>
#include <QDir>
#include <QRegularExpression>
#include <QDebug>
#include "../BankApp-v2.0/src/core/include/BankApp.h"

AdminPanel::AdminPanel(QWidget* parent)
{
    ui.setupUi(this);
    connect(ui.ChangePinAdminButton, &QPushButton::clicked, this, &AdminPanel::changePin);
    connect(ui.ChangePasswordAdminButton, &QPushButton::clicked, this, &AdminPanel::changePassword);
    connect(ui.LogOutAdminButton, &QPushButton::clicked, this, &AdminPanel::on_LogOutButton_clicked);
    connect(ui.AcceptAccAdminButton, &QPushButton::clicked, this, &AdminPanel::acceptAccount);
    connect(ui.RefreshButton, &QPushButton::clicked, this, &AdminPanel::loadAccounts);

}

AdminPanel::~AdminPanel()
{
}
void AdminPanel::initDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    QString dbPath = QCoreApplication::applicationDirPath() + "/../../src/bank.db";

    qDebug() << "Pr�ba otwarcia bazy danych:" << dbPath;

    db.setDatabaseName(dbPath);

    if (!db.open()) {
        QMessageBox::critical(nullptr, "B��d bazy danych", "Nie mo�na otworzy� bazy danych:\n" + db.lastError().text());
    }
}
void AdminPanel::loadAccounts()
{
	initDatabase();  // Upewnij si�, �e baza danych jest zainicjalizowana
    QSqlQueryModel* model = new QSqlQueryModel(this);
    model->setQuery(
        "SELECT "
        "id AS 'Numer konta', "
        "first_name AS 'Imie', "
        "last_name AS 'Nazwisko', "
        "birth_date AS 'Data urodzenia', "
        "email AS 'Email', "
        "CASE verified WHEN 1 THEN 'TAK' ELSE 'NIE' END AS 'Zatwierdzony' "
        "FROM users"
    );

    if (model->lastError().isValid()) {
        qDebug() << "B��d zapytania SQL:" << model->lastError().text();
    }

    ui.AdminView->setModel(model);                    
    ui.AdminView->resizeColumnsToContents();          
}

void AdminPanel::changePin()
{
    auto model = qobject_cast<QSqlQueryModel*>(ui.AdminView->model());     
    QModelIndex index = ui.AdminView->currentIndex();                      
    if (!index.isValid()) {
        QMessageBox::warning(this, "B��d", "Wybierz konto do zmiany PIN-u!");
        return;
    }

    QString accountNumber = model->data(model->index(index.row(), 0)).toString();
    QString newPin = QInputDialog::getText(this, "Zmiana PIN", "Wprowad� nowy PIN:");

    if (newPin.isEmpty()) {
        QMessageBox::warning(this, "B��d", "PIN nie mo�e by� pusty!");
        return;
    }

    QRegularExpression pinRegex("^[0-9]{4}$");
    if (!pinRegex.match(newPin).hasMatch()) {
        QMessageBox::warning(this, "B��d", "PIN musi sk�ada� si� dok�adnie z 4 cyfr!");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE users SET pin = :pin WHERE id = :id");
    query.bindValue(":pin", newPin);
    query.bindValue(":id", accountNumber);

    if (!query.exec()) {
        QMessageBox::critical(this, "B��d", "Nie uda�o si� zaktualizowa� PIN-u:\n" + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Sukces", "PIN zosta� zmieniony!");
}

void AdminPanel::changePassword()
{
    auto model = qobject_cast<QSqlQueryModel*>(ui.AdminView->model());      // <-- ZMIANA
    QModelIndex index = ui.AdminView->currentIndex();                       // <-- ZMIANA
    if (!index.isValid()) {
        QMessageBox::warning(this, "B��d", "Wybierz konto do zmiany has�a!");
        return;
    }

    QString accountNumber = model->data(model->index(index.row(), 0)).toString();
    QString newPassword = QInputDialog::getText(this, "Zmiana has�a", "Wprowad� nowe has�o:");

    if (newPassword.isEmpty()) {
        QMessageBox::warning(this, "B��d", "Has�o nie mo�e by� puste!");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE users SET password = :password WHERE id = :id");
    query.bindValue(":password", newPassword);
    query.bindValue(":id", accountNumber);

    if (!query.exec()) {
        QMessageBox::critical(this, "B��d", "Nie uda�o si� zaktualizowa� has�a:\n" + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Sukces", "Has�o zosta�o zmienione!");
}

void AdminPanel::acceptAccount()
{
    auto model = qobject_cast<QSqlQueryModel*>(ui.AdminView->model());      // <-- ZMIANA
    QModelIndex index = ui.AdminView->currentIndex();                       // <-- ZMIANA
    if (!index.isValid()) {
        QMessageBox::warning(this, "B��d", "Wybierz konto do zatwierdzenia!");
        return;
    }

    QString accountNumber = model->data(model->index(index.row(), 0)).toString();

    QSqlQuery query;
    query.prepare("UPDATE users SET verified = 1 WHERE id = :id");
    query.bindValue(":id", accountNumber);

    if (!query.exec()) {
        QMessageBox::critical(this, "B��d", "Nie uda�o si� zatwierdzi� konta:\n" + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Zatwierdzono", "Konto zosta�o zatwierdzone.");
    loadAccounts(); // Od�wie� dane
}

void AdminPanel::setLog(BankApp* mainApp)
{
    main = mainApp;
}

void AdminPanel::on_LogOutButton_clicked()
{
    QString bankAppExePath = QDir(QCoreApplication::applicationDirPath()).filePath("BankApp.exe");

    if (QFile::exists(bankAppExePath)) {
        QProcess::startDetached(bankAppExePath);
    }
    else {
        QMessageBox::warning(this, "B��d", "Nie znaleziono BankApp.exe!");
    }

    close();
}
