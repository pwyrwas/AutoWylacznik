#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include <QProcess>
#include <QDateTime>
#include <QTimer>

 QTimer* timer;
//co dorobić!
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
    timer = new QTimer(this);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_radioButton_toggled()
{
    QTime currentTime = QTime::currentTime();
    ui->timeEdit->setTime(currentTime);
    ui->timeEdit->show();
    ui->timeEdit_2->hide();
    ui->wylacz->show();

}

void MainWindow::on_radioButton_2_toggled()
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
    lcd = ui->lcdNumber;
    stopWyl = ui->anuluj;



    if(ui->radioButton->isChecked())
    {
        QTime currentTime = QTime::currentTime();
        qDebug() << "Ustawiono wyłączenie komputera o określonej godzinie!";
        qDebug() << "Wyłaczenie komputera zaplanowano na: " << timeEdit1->time().toString();//<< hour <<  :  << minute;

        int secondsInputAll = QTime(0,0,0).secsTo(timeEdit1->time());
        int secondsInputCurrentTime = QTime(0,0,0).secsTo(currentTime);

        int timeToOffSystem = secondsInputAll - secondsInputCurrentTime;

        if(timeToOffSystem < 0)
            timeToOffSystem *= (-1);

        if(timeToOffSystem > 30)
        {
            QProcess process;
            QMessageBox msgBox;
            QString exec = "shutdown -s -t " + QString::number(timeToOffSystem);
            qDebug() << exec;
            process.start(exec);
            process.waitForFinished();
            QString output(process.readAll());
            QTime timeToDisplay = currentTime;
            timeToDisplay = timeToDisplay.addSecs(timeToOffSystem);
            output += "Komputer zostanie wyłączony o: ";
            output += timeToDisplay.toString();
            msgBox.setText(output);
            msgBox.exec();
            QTime reset(0,0);
            timeEdit2->setTime(reset);


            lcd->show();
            lcd->setDigitCount(8);
            lcd->display(timeToDisplay.toString("hh:mm:ss"));
        }else
        {
            QString exec = "shutdown -s -t 60";
            int ret;
            ret = QMessageBox::warning(this, tr("Automatyczny Wyłącznik"), tr("Komputer zostanie wyłączonyw ciągu 1 minuty! \nCzy chcesz aby komputer został teraz wyłączony ?"), QMessageBox::Ok |QMessageBox::Cancel);
            switch (ret) {
                case QMessageBox::Ok:
                    turnOffNow(exec);
                    break;
                case QMessageBox::Cancel:
                    break;
             default:
                    break;
            }
        }
    }
    if(ui->radioButton_2->isChecked())
    {
        //int hour = timeEdit2->time().hour();
        //int minute = timeEdit2->time().minute();
        qDebug() << "Ustawiono wyłączenie komputera za zdefiniowany czas!";
        QTime currentTime = QTime::currentTime();
        int seconds = QTime(0,0,0).secsTo(timeEdit2->time());
        QTime timeOffComputer = currentTime.addSecs(seconds);
        qDebug() << "Wyłaczenie komputera zaplanowano na: " << timeOffComputer.toString();//<< hour <<  :  << minute;

        if(seconds != 0)
        {
            qDebug() << seconds;
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
            QTime timeToEnd;
            //QTime reset(0,0);
            //timeEdit2->setTime(reset);
            timeToEnd.setHMS(0,0,0);
            timeToEnd = timeToEnd.addSecs(seconds);
            //qDebug() << timeToEnd << "Ten czas" <<seconds;
            *objTime = timeOffComputer;

            timer->setInterval(1);
            connect(timer, SIGNAL(timeout()), this, SLOT(showTime()));
            timer->start(seconds);
            showTime();
        }else
        {
           QString exec = "shutdown -s -t 60";
           int ret;
           ret = QMessageBox::warning(this, tr("Automatyczny Wyłącznik"), tr("Komputer zostanie wyłączonyw ciągu 1 minuty! \nCzy chcesz aby komputer został teraz wyłączony ?"), QMessageBox::Ok |QMessageBox::Cancel);
           switch (ret) {
               case QMessageBox::Ok:
                   turnOffNow(exec);
                   break;
               case QMessageBox::Cancel:
                   break;
            default:
                   break;
           }
        }
    }
}
void MainWindow::turnOffNow(QString exec)
{
    QProcess process;
    process.start(exec);
    process.waitForFinished();
    //qDebug() << exec;
}
void MainWindow::showTime()
{
    //co wywołanie timera odejmuje od zadanego czasu czas pozostały. W ten sposób wyświetlam nowo uzyskany czas w lcd
    QTime current = QTime::currentTime();
    int secondsToEnd = QTime(0,0,0).secsTo(*objTime);
    int secondsNow = QTime(0,0,0).secsTo(current);

    int timeToOffSystem = secondsToEnd - secondsNow;
    QTime time = time.addSecs(timeToOffSystem);
    lcd->display(time.toString());

}

void MainWindow::on_anuluj_clicked()
{
    if(timer->isActive())
    {
        timer->stop();
        qDebug() << "Timer stop";
    }
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
    stopWyl->hide();
    msgBox.setText(output);
    msgBox.exec();



}
