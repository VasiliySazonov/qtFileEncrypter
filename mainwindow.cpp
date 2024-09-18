#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_encrypt, &QPushButton::pressed, [this] () {
        QString filePath = QFileDialog::getOpenFileName(nullptr, "Open a file ", "/", "All Files (*.*)");

        if (!QFile::exists(filePath))
            return;

        QFile input(filePath);
        input.open(QFile::ReadOnly);
        QByteArray fileData = input.readAll();
        input.close();

        QByteArray encryptedData = Encryption::encrypt(fileData, ui->lineEdit->text());

        int reply = QMessageBox::question(nullptr, "Save as", "File is encrypted and ready to be saved", QMessageBox::Save | QMessageBox::No);

        if (reply == QMessageBox::Save)
        {
            QString outputFilePath = QFileDialog::getSaveFileName(nullptr, "Save encrypted file", "/", "Tree fish encrypted (*.tfe)");

            QFile output(outputFilePath);
            output.open(QFile::WriteOnly);
            output.write(encryptedData);
            output.close();
        }
    });

    connect(ui->pushButton_decrypt, &QPushButton::pressed, [this]() {
        QString filePath = QFileDialog::getOpenFileName(nullptr, "Open a file ", "", "Tree fish encrypted (*.tfe)");

        if (!QFile::exists(filePath))
            return;

        QFile input(filePath);
        input.open(QFile::ReadOnly);
        QByteArray fileData = input.readAll();
        input.close();

        QByteArray decryptedData = Encryption::decrypt(fileData, ui->lineEdit->text());

        int reply = QMessageBox::question(nullptr, "Save as", "File is decrypted and ready to be saved", QMessageBox::Save | QMessageBox::No);

        if (reply == QMessageBox::Save)
        {
            QString outputFilePath = QFileDialog::getSaveFileName(nullptr, "Save encrypted file", "", "All files (*.*)");

            QFile output(outputFilePath);
            output.open(QFile::WriteOnly);
            output.write(decryptedData);
            output.close();
        }
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

