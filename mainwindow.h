#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRadioButton>
#include <QTimeEdit>
#include <QLCDNumber>
#include <QPushButton>
#include <QTime>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTime*  objTime;

private slots:
    void on_radioButton_toggled();

    void on_radioButton_2_toggled();

    void on_wylacz_clicked();

    void on_anuluj_clicked();

    void showTime();




private:
    Ui::MainWindow *ui;
    QTimeEdit *timeEdit1;
    QTimeEdit *timeEdit2;
    QLCDNumber *lcd;
    QPushButton *stopWyl;
    void turnOffNow(QString exec);

};

#endif // MAINWINDOW_H
