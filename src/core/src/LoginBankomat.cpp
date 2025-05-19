#include "../include/LoginBankomat.h"
#include "ui_LoginBankomat.h"
#include <QMessageBox>
#include "../include/BankApp.h"
#include "../include/BankomatLoggedIn.h"
LoginBankomat::LoginBankomat(QWidget* parent)
{
    ui.setupUi(this);

    // Po³¹cz przyciski numeryczne z funkcj¹ dodawania cyfr
    connect(ui.ZeroButton, &QPushButton::clicked, this, &LoginBankomat::onNumberClicked);
    connect(ui.OneButton, &QPushButton::clicked, this, &LoginBankomat::onNumberClicked);
    connect(ui.TwoButton, &QPushButton::clicked, this, &LoginBankomat::onNumberClicked);
    connect(ui.ThreeButon, &QPushButton::clicked, this, &LoginBankomat::onNumberClicked);
    connect(ui.FourButton, &QPushButton::clicked, this, &LoginBankomat::onNumberClicked);
    connect(ui.FiveButton, &QPushButton::clicked, this, &LoginBankomat::onNumberClicked);
    connect(ui.SixButton, &QPushButton::clicked, this, &LoginBankomat::onNumberClicked);
    connect(ui.SevenButton, &QPushButton::clicked, this, &LoginBankomat::onNumberClicked);
    connect(ui.EightButton, &QPushButton::clicked, this, &LoginBankomat::onNumberClicked);
    connect(ui.NineButton, &QPushButton::clicked, this, &LoginBankomat::onNumberClicked);

    // Po³¹cz kasowanie z odpowiedni¹ funkcj¹
    connect(ui.ClearButton, &QPushButton::clicked, this, &LoginBankomat::onClearClicked);
    connect(ui.DeleteButton, &QPushButton::clicked, this, &LoginBankomat::onBackspaceClicked);

    // Po³¹cz przycisk logowania
    connect(ui.OkButton, &QPushButton::clicked, this, &LoginBankomat::onLoginClicked);
    connect(ui.BackButton, &QPushButton::clicked, this, &LoginBankomat::on_BackButton_clicked);
}

LoginBankomat::~LoginBankomat()
{
}

void LoginBankomat::onNumberClicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if (!button) return;

    QString digit = button->text();

    // Jeœli pole numeru konta jest puste, uzupe³niamy najpierw je, potem PIN
    if (ui.AccountEdit->text().length() < 8) {
        ui.AccountEdit->setText(ui.AccountEdit->text() + digit);
    }
    else if (ui.PinEdit->text().length() < 4) {
        ui.PinEdit->setText(ui.PinEdit->text() + "*");  // Maskujemy wpisywane has³o
        enteredPin += digit;  // Przechowujemy prawdziwy PIN
    }
}

void LoginBankomat::onBackspaceClicked()
{
    if (!ui.PinEdit->text().isEmpty()) {
        ui.PinEdit->setText(ui.PinEdit->text().left(ui.PinEdit->text().length() - 1));
        enteredPin.chop(1);  // Usuwamy ostatni¹ cyfrê PIN
    }
    else if (!ui.AccountEdit->text().isEmpty()) {
        ui.AccountEdit->setText(ui.AccountEdit->text().left(ui.AccountEdit->text().length() - 1));
    }
}

void LoginBankomat::onClearClicked()
{
    ui.AccountEdit->clear();
    ui.PinEdit->clear();
    enteredPin.clear();
}

void LoginBankomat::onLoginClicked()
{
    QString login = ui.AccountEdit->text().trimmed();
    QString credential = QString::fromStdString(enteredPin.toStdString());


    if (login.isEmpty() || credential.isEmpty()) {
        QMessageBox::warning(this, "B³¹d", "Wszystkie pola musz¹ byæ wype³nione!");
        return;
    }

    if (Osoba::sprawdzPin(login.toStdString(), credential.toStdString())) {
        // sprawdzLogowanie wykryje 4-cyfrowy PIN i zwróci true
        QMessageBox::information(this, "Sukces", "Zalogowano pomyœlnie!");
        if (log) { delete log; log = nullptr; }

        auto atm = new BankomatLoggedIn();
        atm->setLog(main);
        atm->AccNumber(login.toStdString());
        atm->show();
        close();
    }
    else {
        QMessageBox::warning(this, "B³¹d", "Niepoprawne dane logowania!");
    }
}

void LoginBankomat::setLog(BankApp* mainWindow)
{
    main = mainWindow;
}
void LoginBankomat::on_BackButton_clicked()
{
    if (main) {
        main->show();
        close();
    }
    else
    {
        QMessageBox::critical(this, "Error", "Main window not set");
    }
}