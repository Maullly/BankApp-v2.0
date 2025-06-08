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

    qDebug() << "Próba otwarcia bazy danych:" << dbPath;

    db.setDatabaseName(dbPath);

    if (!db.open()) {
        QMessageBox::critical(nullptr, "Blad bazy danych", "Nie mozna otworzyc bazy danych:\n" + db.lastError().text());
    }
}
void AdminPanel::loadAccounts()
{
	initDatabase();  // Upewnij siê, ¿e baza danych jest zainicjalizowana
    QSqlQueryModel* model = new QSqlQueryModel(this);
    model->setQuery(
        "SELECT "
        "id AS 'Numer konta', "
        "first_name AS 'Imie', "
        "last_name AS 'Nazwisko', "
        "birth_date AS 'Data urodzenia', "
        "email AS 'Email', "
        "CASE verified WHEN 1 THEN 'TAK' ELSE 'NIE' END AS 'Zatwierdzony', "
		"CASE is_admin WHEN 1 THEN 'TAK' ELSE 'NIE' END AS 'Administrator' "
        "FROM users"
    );

    if (model->lastError().isValid()) {
        qDebug() << "B³¹d zapytania SQL:" << model->lastError().text();
    }

    ui.AdminView->setModel(model);                    
    ui.AdminView->resizeColumnsToContents();      
    ui.AdminView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void AdminPanel::changePin()
{
    auto model = qobject_cast<QSqlQueryModel*>(ui.AdminView->model());     
    QModelIndex index = ui.AdminView->currentIndex();                      
    if (!index.isValid()) {
        QMessageBox::warning(this, "Blad", "Wybierz konto do zmiany PIN-u!");
        return;
    }

    QString accountNumber = model->data(model->index(index.row(), 0)).toString();
    QString newPin = QInputDialog::getText(this, "Zmiana PIN", "Wprowadz nowy PIN:");

    if (newPin.isEmpty()) {
        QMessageBox::warning(this, "Blad", "PIN nie moze byc pusty!");
        return;
    }

    QRegularExpression pinRegex("^[0-9]{4}$");
    if (!pinRegex.match(newPin).hasMatch()) {
        QMessageBox::warning(this, "Blad", "PIN musi sk³adac sie dok³adnie z 4 cyfr!");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE users SET pin = :pin WHERE id = :id");
    query.bindValue(":pin", newPin);
    query.bindValue(":id", accountNumber);

    if (!query.exec()) {
        QMessageBox::critical(this, "Blad", "Nie udalo sie zaktualizowac PIN-u:\n" + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Sukces", "PIN zostal zmieniony!");
}

void AdminPanel::changePassword()
{
    auto model = qobject_cast<QSqlQueryModel*>(ui.AdminView->model());      // <-- ZMIANA
    QModelIndex index = ui.AdminView->currentIndex();                       // <-- ZMIANA
    if (!index.isValid()) {
        QMessageBox::warning(this, "Blad", "Wybierz konto do zmiany hasla!");
        return;
    }

    QString accountNumber = model->data(model->index(index.row(), 0)).toString();
    QString newPassword = QInputDialog::getText(this, "Zmiana hasla", "Wprowadz nowe haslo:");

    if (newPassword.isEmpty()) {
        QMessageBox::warning(this, "Blad", "Haslo nie moze byc puste!");
        return;
    }

    QSqlQuery query;
    query.prepare("UPDATE users SET password = :password WHERE id = :id");
    query.bindValue(":password", newPassword);
    query.bindValue(":id", accountNumber);

    if (!query.exec()) {
        QMessageBox::critical(this, "Blad", "Nie udalo siê zaktualizowac hasla:\n" + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Sukces", "Haslo zostalo zmienione!");
}

void AdminPanel::acceptAccount()
{
    auto model = qobject_cast<QSqlQueryModel*>(ui.AdminView->model());
    QModelIndex index = ui.AdminView->currentIndex();

    if (!index.isValid()) {
        QMessageBox::warning(this, "Blad", "Wybierz konto do zatwierdzenia!");
        return;
    }

    QString accountNumber = model->data(model->index(index.row(), 0)).toString();

    // Sprawdzenie z bazy danych
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT verified FROM users WHERE id = :id");
    checkQuery.bindValue(":id", accountNumber);

    if (!checkQuery.exec() || !checkQuery.next()) {
        QMessageBox::critical(this, "Blad", "Nie udalo sie sprawdzic statusu konta:\n" + checkQuery.lastError().text());
        return;
    }

    int verified = checkQuery.value(0).toInt();
    if (verified == 1) {
        QMessageBox::information(this, "Informacja", "Konto jest juz zatwierdzone.");
        return;
    }

    // Zatwierdzanie
    QSqlQuery query;
    query.prepare("UPDATE users SET verified = 1 WHERE id = :id");
    query.bindValue(":id", accountNumber);

    if (!query.exec()) {
        QMessageBox::critical(this, "Blad", "Nie udalo sie zatwierdzic konta:\n" + query.lastError().text());
        return;
    }

    QMessageBox::information(this, "Zatwierdzono", "Konto zostalo zatwierdzone.");
    loadAccounts();
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
        QMessageBox::warning(this, "Blad", "Nie znaleziono BankApp.exe!");
    }

    close();
}
void AdminPanel::showEvent(QShowEvent* event)
{
	QWidget::showEvent(event);
	loadAccounts();  // Za³aduj konta przy ka¿dym otwarciu panelu
}