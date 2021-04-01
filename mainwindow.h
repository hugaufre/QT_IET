#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QTimer>
#include <QTime>
#include <QTabWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QSqlRecord>
#include <QThread>
#include <QLineEdit>
#include <QSqlQuery>
#include <QPixmap>
#include <QMessageBox>
#include <QSqlError>
#include <QtDebug>

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
    void on_rfidEdit_returnPressed();

    void on_delEdit_returnPressed();

    void on_addPlayerB_clicked();

    void replacment(QString &rfid);

private:
    Ui::MainWindow *ui;
    QSqlDatabase IESDataBase;
    QSqlQueryModel *modelPlayer;
    QSqlQueryModel *modelTraining;
};
#endif // MAINWINDOW_H
