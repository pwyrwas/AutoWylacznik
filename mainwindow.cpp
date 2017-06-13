#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include <QProcess>
#include <QDateTime>


//co dorobić!
// Komunikat jeśli wybrane zostanie uruchom po czasie i czas = 00:00 bo komputer wyłącza się bezpośrednio. Dodać informacje do potwierdzenia że wyłączy się natychmiast.
// Wyłączenie o zadanej godzinie (działa na razie jeżeli nie przekraczamy północy - pomyśleć co gdy ją przekraczamy)
// Zmiana wyświetlania czasu wyłączenia na odliczanie do wyłączenia. (QTimer ??)

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->timeEdit->hide();
    ui->timeEdit_2->hide();
    ui->wylacz->hide();
    ui->anuluj->hide();
    ui->lcdNumber->hide();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_radioButton_toggled(bool checked)
{
    QTime currentTime = QTime::currentTime();
    ui->timeEdit->setTime(currentTime);
    ui->timeEdit->show();
    ui->timeEdit_2->hide();
    ui->wylacz->show();

}

void MainWindow::on_radioButton_2_toggled(bool checked)
{
    ui->timeEdit->hide();
    ui->timeEdit_2->show();
    ui->wylacz->show();
}

void MainWindow::on_wylacz_clicked()
{
    timeEdit1 = ui->timeEdit;
    timeEdit2 = ui->timeEdit_2;
    ui->anuluj->show();



    if(ui->radioButton->isChecked())
    {
        QTime currentTime = QTime::currentTime();
        int hour = ui->timeEdit->time().hour();
        int minute = ui->timeEdit->time().minute();
        qDebug() << "Ustawiono wyłączenie komputera o określonej godzinie!";
        qDebug() << "Wyłaczenie komputera zaplanowano na: " << timeEdit1->time().toString();//<< hour <<  :  << minute;

        int secondsInputAll = QTime(0,0,0).secsTo(timeEdit1->time());
        int secondsInputCurrentTime = QTime(0,0,0).secsTo(currentTime);

        int timeToOffSystem = secondsInputAll - secondsInputCurrentTime;

        if(timeToOffSystem < 0)
            timeToOffSystem *= (-1);

        QProcess process;
        QMessageBox msgBox;
        QString exec = "shutdown -s -t " + QString::number(timeToOffSystem);
        qDebug() << exec;
        process.start(exec);
        process.waitForFinished();
        QString output(process.readAll());
        qDebug() << output;
        output += "Komputer zostanie wyłączony o: ";
        output += QString::number(timeToOffSystem);
        msgBox.setText(output);
        msgBox.exec();
        QTime reset(0,0);
        timeEdit2->setTime(reset);


    }
    if(ui->radioButton_2->isChecked())
    {
        int hour = timeEdit2->time().hour();
        int minute = timeEdit2->time().minute();
        qDebug() << "Ustawiono wyłączenie komputera za zdefiniowany czas!";
        QTime currentTime = QTime::currentTime();
        int seconds = QTime(0,0,0).secsTo(timeEdit2->time());
        QTime timeOffComputer = currentTime.addSecs(seconds);
        qDebug() << "Wyłaczenie komputera zaplanowano na: " << timeOffComputer.toString();//<< hour <<  :  << minute;

        // qDebug() << seconds;
        QProcess process;
        QMessageBox msgBox;
        QString exec = "shutdown -s -t " + QString::number(seconds);
        qDebug() << exec;
        process.start(exec);
        process.waitForFinished();
        QString output(process.readAll());
        qDebug() << output;
        output += "Komputer zostanie wyłączony o: ";
        output += timeOffComputer.toString();
        QString h,m,s,finalTime;
        h =QString::number(timeOffComputer.hour());
        m =QString::number(timeOffComputer.minute());
        s =QString::number(timeOffComputer.second());
        finalTime = h+":"+m;

        //Wyświetlanie czasu wyłączenia:
        ui->lcdNumber->show();
        ui->lcdNumber->setDigitCount(8);
        ui->lcdNumber->display(timeOffComputer.toString("hh:mm:ss"));
        msgBox.setText(output);
        msgBox.exec();
        QTime reset(0,0);
        timeEdit2->setTime(reset);
    }
}

void MainWindow::on_anuluj_clicked()
{
    QProcess process;
    QMessageBox msgBox;
    QString exec = "shutdown -a";
    QStringList params;
    params << "";
    process.start(exec);
    process.waitForFinished();
    QString output(process.readAll());
    qDebug() << output;
    output += "Zatrzymano automatyczne wyłączanie komputera!";
    ui->lcdNumber->hide();
    msgBox.setText(output);
    msgBox.exec();

}
