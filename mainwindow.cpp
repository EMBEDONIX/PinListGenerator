#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    int cbIndex = ui->cbInputType->currentIndex();
    QString title;
    QString filterExt;
    QString fileName;
    switch(cbIndex) {
        case 0:
            title = QString("OPEN CADSTAR CPA");
            filterExt = "*.cpa";
            break;

        default:
            QMessageBox msg;
            msg.setWindowTitle("Error!");
            msg.setText("Invalid input type");
            msg.setInformativeText("Please select an item from the combobox");
            msg.exec();
            return;
    }

    fileName = QFileDialog::getOpenFileName(this,
                                 title,
                                 QDir::homePath(),
                                 filterExt);

    fileInput = new QFile(fileName, this);
    if(!fileInput->open(QIODevice::ReadOnly)) {
        QMessageBox::information(this, "Error!", "Can not open/read from " + fileName);
    }

    QTextStream in(fileInput);


    QRegExp regex("\"[^\"]*\"");
    while(!in.atEnd()) {
        QString line = in.readLine();
        if(line.contains("PADIDENTIFIER", Qt::CaseInsensitive)) {
            regex.indexIn(line);
            qDebug() << regex.capturedTexts();
            ui->tePins->appendPlainText(QString(regex.capturedTexts().first())
                                        .replace("\"", ""));
        }
    }
}
