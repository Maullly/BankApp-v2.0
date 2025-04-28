#include "../include/LastTransactions.h"
#include "../include/LoggedInWindow.h"
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
LastTransactionsWindow::LastTransactionsWindow(QWidget* parent)
{
	ui.setupUi(this);
	connect(ui.BackButton, &QPushButton::clicked, this, &LastTransactionsWindow::on_BackButton_clicked);
    connect(ui.RefreshButton, &QPushButton::clicked, this, &LastTransactionsWindow::loadTransactions);
}

LastTransactionsWindow::~LastTransactionsWindow()
{

}

void LastTransactionsWindow::setLog(LoggedInWindow* mainWindow)
{
	main = mainWindow;
}

void LastTransactionsWindow::setAccountNumber(std::string accNum)
{
	accountNumber = accNum;
}

void LastTransactionsWindow::on_BackButton_clicked()
{
	qDebug() << "Back button clicked";
	if (main) {
		main->show();
		close();
	}
	else
	{
		QMessageBox::critical(this, "Error", "Main window not set");
	}
}

void LastTransactionsWindow::loadTransactions() {
    QFile file("./src/transactions.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "B³¹d", "Nie mo¿na otworzyæ pliku z transakcjami!");
        return;
    }

    QTextStream in(&file);
    ui.TransactionsTable->setRowCount(0);  // Czyœcimy tabelê przed dodaniem nowych danych

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList parts = line.split(",");

        if (parts.size() >= 7) {  // Teraz mamy 7 kolumn (ostatnia to numer konta)
            QString numerKontaWPliku = parts[6].trimmed();  // Pobieramy numer konta i usuwamy bia³e znaki

            if (numerKontaWPliku == QString::fromStdString(accountNumber)) { // Sprawdzamy zgodnoœæ konta
                int row = ui.TransactionsTable->rowCount();
                ui.TransactionsTable->insertRow(row); // Dodajemy nowy wiersz

                // Tworzymy QTableWidgetItem dla ka¿dej kolumny
                QTableWidgetItem* imieItem = new QTableWidgetItem(parts[0]);
                QTableWidgetItem* nazwiskoItem = new QTableWidgetItem(parts[1]);
                QTableWidgetItem* dataItem = new QTableWidgetItem(parts[2]);
                QTableWidgetItem* rodzajItem = new QTableWidgetItem(parts[3]);
                QTableWidgetItem* stanPrzedItem = new QTableWidgetItem(parts[4]);
                QTableWidgetItem* stanPoItem = new QTableWidgetItem(parts[5]);

                // Wstawiamy dane do odpowiednich kolumn
                ui.TransactionsTable->setItem(row, 0, imieItem);
                ui.TransactionsTable->setItem(row, 1, nazwiskoItem);
                ui.TransactionsTable->setItem(row, 2, dataItem);
                ui.TransactionsTable->setItem(row, 3, rodzajItem);
                ui.TransactionsTable->setItem(row, 4, stanPrzedItem);
                ui.TransactionsTable->setItem(row, 5, stanPoItem);
            }
        }
    }

    file.close();
}