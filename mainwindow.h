#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QSerialPort>

#include <QPushButton>
#include <QComboBox>

#include <QMainWindow>
#include <windows.h>

#include<QLabel>
#include<QDebug>

#include<QGridLayout>
#include<QTextBrowser>

#include<QLineEdit>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

QSerialPort *p_Port1;

QPushButton *p_Sapply;
QPushButton *p_Sdisable;
QPushButton *p_Sconnected;

QComboBox *p_SerialPort;

QLabel *p_Sstatus;
QLabel *p_Sportstatus;

QGridLayout *p_Serial;
QTextBrowser *p_READ;

QLineEdit *p_COMMAND;

QString TCHARToString(const TCHAR* ptsz);

void SCAN_Serial();
void Serial_status();

public slots:
void serial_button_click();
void text_Reading();
void text_Writing();
void disable_click();

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
