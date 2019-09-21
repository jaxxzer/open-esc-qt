#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <port-scan.h>
#include <QList>
#include <QSerialPortInfo>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_clicked(bool checked);

    void onPortScanFinished(QList<QSerialPortInfo>);

private:
    Ui::MainWindow *ui;

    PortScanner portScanner;
};
#endif // MAINWINDOW_H
