//#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
//    #include <windows.h>
//#else

//#endif

#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    p_Port1 = new QSerialPort();

    p_Sapply = new QPushButton;
    p_Sdisable = new QPushButton;
    p_Sconnected = new QPushButton;

    p_SerialPort = new QComboBox;

    p_Sportstatus = new QLabel;
    p_Sstatus = new QLabel;

    p_Serial = new QGridLayout;
    p_READ = new QTextBrowser;

    p_COMMAND = new QLineEdit;

    SCAN_Serial();

    p_Port1->setBaudRate(115200);

    p_Serial->setSpacing(5);
    p_Serial->addWidget(p_SerialPort,0,0,1,1,Qt::AlignLeft);
    p_Serial->addWidget(p_Sapply,0,1,1,1,Qt::AlignLeft);
    p_Serial->addWidget(p_Sdisable,0,2,1,1,Qt::AlignRight);
    p_Serial->addWidget(p_Sstatus,1,0,1,2,Qt::AlignCenter);
    p_Serial->addWidget(p_READ,2,0,5,5,Qt::AlignCenter);
    p_Serial->addWidget(p_Sportstatus,1,4,1,1,Qt::AlignRight);
    p_Serial->addWidget(p_COMMAND,9,0,1,5,Qt::AlignCenter);

    p_Sapply->setText("Apply");
    p_Sdisable->setText("Disable");
    p_Sportstatus->setText("Port name");
    p_Sstatus->setText("Connect Status");

    p_COMMAND->setEchoMode(QLineEdit::EchoMode());

    connect(p_Sapply,SIGNAL(clicked()),this,SLOT(serial_button_click()));
    connect(p_Port1, SIGNAL(readyRead()),this, SLOT(text_Reading()));
    connect(p_Sdisable,SIGNAL(clicked()),this,SLOT(disable_click()));
    connect(p_COMMAND,SIGNAL(returnPressed()),this, SLOT(text_Writing()));

    ui->centralWidget->setLayout(p_Serial);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SCAN_Serial()
{
    HKEY hKey;
    RegOpenKey(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), &hKey);

    TCHAR szData[20], szName[100];
    DWORD index = 0, dwSize=100, dwSize2 = 20, dwType = REG_SZ;
    memset(szData, 0x00, sizeof(szData));
    memset(szName, 0x00, sizeof(szName));

    while (ERROR_SUCCESS == RegEnumValue(hKey, index, szName, &dwSize, NULL, NULL, NULL, NULL)){
       index++;

       RegQueryValueEx(hKey, szName, NULL, &dwType, (LPBYTE)szData, &dwSize2);
       QString tem = TCHARToString(szData);
       p_SerialPort->addItem(tem);

       memset(szData, 0x00, sizeof(szData));
       memset(szName, 0x00, sizeof(szName));
       dwSize = 100;
       dwSize2 = 20;
   }

   RegCloseKey(hKey);
}

QString MainWindow::TCHARToString(const TCHAR* ptsz){
 int len = wcslen((wchar_t*)ptsz);
 char* psz = new char[2*len + 1];
 wcstombs(psz, (wchar_t*)ptsz, 2*len + 1);
 QString s = psz;
 delete [] psz;
 return s;
}

void MainWindow::serial_button_click()
{
    p_Port1->setPortName(p_SerialPort->currentText());

    if(!p_Port1->open(QIODevice::ReadWrite))
    {
        qDebug()<<"Serial port open error\n";
        p_Sstatus->setText("connect error");
    }
    else
    {
        p_Sstatus->setText("Connect success");
        //qDebug()<<p_Port1->portName();
        p_Sportstatus->setText(p_Port1->portName());
    }

}

void MainWindow::text_Reading(){

    QByteArray read_Data;
    read_Data = p_Port1->readAll();

   // p_READ->setText(read_Data);
    p_READ->insertHtml(read_Data);
}

void MainWindow::text_Writing()
{
    QByteArray write_Data;
    write_Data = p_COMMAND->text().toUtf8();

    qDebug()<<p_COMMAND->text();
    qDebug()<<"Write";

    p_COMMAND->clear();
}

void MainWindow::disable_click()
{
    if(p_Port1->isOpen())
    {
        p_Port1->close();
        qDebug()<<"Serial Port close success";
        p_Sportstatus->setText("Port name");
        p_Sstatus->setText("Nothing connect");
    }
    else
    {
        qDebug()<<"close canceled";
    }

}
