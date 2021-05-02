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
#include <QAbstractButton>
#include <QFile>

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

    void lirePeripherique();

    void on_rfidEdit_returnPressed();

    void on_delEdit_returnPressed();

    void on_addPlayerB_clicked();

    void replacment(QString &rfid);

    void delay(int s);

    void on_playerTableView_doubleClicked(const QModelIndex &index);

    void on_activePortButton_clicked();

    void on_comboBoxPort_currentIndexChanged(const QString &arg1);

    void on_laserON_clicked();

    void on_laserOFF_clicked();

    void trainingFunction();

    void on_seanceRFID_returnPressed();

    void exportSeanceDataBase();

    void on_trainingEndB_clicked();

private:
    Ui::MainWindow *ui;

    QSqlDatabase IESDataBase;
    QSqlQueryModel *modelPlayer;
    QSqlQueryModel *modelPlayerInfo;
    QSqlQueryModel *modelPlayerInfo2;
    QSqlQueryModel *modelTraining;

    QString portSelectionne;
    QList<QSerialPortInfo> listeDesPorts;
    QSerialPort *peripherique;
    bool connecte = false;
    QByteArray serialData;
    QString serialBuffer;
    QString out;
    int e = 0;

};
#endif // MAINWINDOW_H
