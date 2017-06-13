#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRadioButton>
#include <QTimeEdit>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_radioButton_toggled(bool checked);

    void on_radioButton_2_toggled(bool checked);

    void on_wylacz_clicked();

    void on_anuluj_clicked();

private:
    Ui::MainWindow *ui;
    QTimeEdit *timeEdit1;
    QTimeEdit *timeEdit2;

};

#endif // MAINWINDOW_H
