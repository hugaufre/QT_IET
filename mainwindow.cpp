#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusbar->addPermanentWidget(ui->connectionLabel);

    ui->infoLabel->hide();
    ui->trainingEndB->hide();

    //DATABSE--------------------------------------------------------------------------------------------
    {

    IESDataBase = QSqlDatabase::addDatabase("QSQLITE");
    IESDataBase.setDatabaseName("C:/Users/hugos/OneDrive/Documents/Cours/Projet/IESDataBase.db");

    if(IESDataBase.open()){

        ui->connectionLabel->setText("Connexion à la base de données OK...!");
    } else{

       ui->connectionLabel->setText("Impossible de se connecter à la base de données");
    }

    IESDataBase.open();
    this->modelPlayer = new QSqlQueryModel();
    this->modelPlayerInfo = new QSqlQueryModel();
    this->modelPlayerInfo2 = new QSqlQueryModel();
    this->modelTraining = new QSqlQueryModel();

    modelPlayer->setQuery("SELECT Noms,Prenoms,N°deTag FROM Joueurs");
    modelPlayerInfo2->setQuery("SELECT Prenoms,Noms,N°deTag FROM Joueurs");
    modelTraining->setQuery("SELECT * FROM Seance");


    ui->playerTableView->setModel(modelPlayer);
    ui->showPlayerTable->setModel(modelPlayerInfo2);
    ui->trainingTableView->setModel(modelTraining);




    //DATABSE--------------------------------------------------------------------------------------------
    }

    //SERIALPORT-----------------------------------------------------------------------------------------
    {

    listeDesPorts = QSerialPortInfo::availablePorts();
    for(int i = 0; i<listeDesPorts.length(); i++){
        ui->comboBoxPort->addItem(listeDesPorts[i].portName());
    }
    peripherique = new QSerialPort(this);

    connect(peripherique, SIGNAL(readyRead()), this, SLOT(lirePeripherique()));

    //SERIALPORT-----------------------------------------------------------------------------------------
    }

    //IMAGES---------------------------------------------------------------------------------------------
    {

    QPixmap greenCircle("C:/Users/hugos/OneDrive/Documents/Cours/Projet/voyant_vert.png");

        QPixmap redCircle("C:/Users/hugos/OneDrive/Documents/Cours/Projet/voyant_rouge.png");

        ui->greenCLabel->setPixmap(greenCircle.scaled(120,120, Qt::KeepAspectRatioByExpanding));

        ui->redCLabel->setPixmap(redCircle.scaled(120,120, Qt::KeepAspectRatioByExpanding));

        ui->redCLabel->hide();


    //IMAGES---------------------------------------------------------------------------------------------
    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::lirePeripherique()
{

    serialData = peripherique->readLine();
    serialBuffer += QString::fromStdString(serialData.toStdString());
    ui->label_readPeripherique->setText(serialBuffer);



}

void MainWindow::on_rfidEdit_returnPressed()
{

    QSqlQuery query;

    QString rfidEDIT = ui->rfidEdit->text();

    replacment(rfidEDIT);

    int counter = 0;

    if(rfidEDIT.length() == 10){

        query.exec("SELECT Prenoms,Noms,N°deTag, datation_instant_shoot FROM Joueurs WHERE N°deTag =  \""+ rfidEDIT + "\"");
       //modelPlayer->setQuery("SELECT Prenoms,Noms,N°deTag, datation_instant_shoot FROM Joueurs WHERE N°deTag =  \""+ rfidEDIT + "\"");


        while(query.next()){ //On recherche avec une boucle dans la base de données si le TAG est attribué.

                          qDebug() << "counter" << counter;
                          counter++;
                          qDebug() << "counter" << counter;

        }

        if(counter == 0){


            QMessageBox::warning(this,"Information!","Aucun enregistrement n'a été fait avec ce tag, veuillez ajouter un joueur avec ce tag pour l'utiliser");

        } else {

            modelPlayerInfo2->setQuery("SELECT Prenoms,Noms,N°deTag, datation_instant_shoot FROM Joueurs WHERE N°deTag =  \""+ rfidEDIT + "\"");
        }

    }



}

void MainWindow::on_delEdit_returnPressed()
{

    QString delrfid = ui->delEdit->text(); //Suppression d'un joueur avec son Tag
        replacment(delrfid);



        if(delrfid.length() != 10){

        }else{

            QMessageBox reponse;

            reponse.setText(tr("Voulez-vous supprimez ce joueur ?"));
            QAbstractButton* pButtonYes = reponse.addButton(tr("Oui"), QMessageBox::YesRole); reponse.addButton(tr("Non"), QMessageBox::NoRole);

             reponse.exec();

            if (reponse.clickedButton()==pButtonYes) {

                modelPlayer->setQuery("DELETE FROM Joueurs WHERE N°deTag = \"" +delrfid+ "\"");

                ui->delEdit->clear();

                modelPlayerInfo2->setQuery("SELECT Prenoms,Noms,N°deTag FROM Joueurs");
                modelPlayerInfo->setQuery("SELECT Noms,N°deTag FROM Joueurs");
                modelPlayer->setQuery("SELECT Prenoms,Noms,N°deTag FROM Joueurs");

            }else{


                 }
        }

}

void MainWindow::on_addPlayerB_clicked()
{

    QSqlQuery query;
    QString addRFID = ui->addRFID->text();

    replacment(addRFID);

    QString lname = ui->lnameEdit->text();
    QString fname = ui->fnameEdit->text();


    if(addRFID.length() == 10){ //On vérifie si le n° de tag comporte bien 10 chiffres

        query.prepare("INSERT INTO Joueurs (Prenoms, Noms, N°deTag)" //Commande pour ajouter un joueur...
              "VALUES (?, ?, ?)");
        query.addBindValue(fname);                                        //...avec tout les paramètres
        query.addBindValue(lname);
        query.addBindValue(addRFID);



        if(query.exec()){ //On vérifie si le joueur a été enregistré

            QMessageBox::information(this,"Information","Enregistrment effectué");
            modelPlayerInfo2->setQuery("SELECT Prenoms, Noms, N°deTag FROM Joueurs");
            modelPlayerInfo->setQuery("SELECT Noms,N°deTag FROM Joueurs");
            modelPlayer->setQuery("SELECT Prenoms, Noms, N°deTag FROM Joueurs");//On affiche le joueur.



           /* query.prepare("INSERT INTO Seance (joueur,date,tag_id)"
                  "VALUES (?,?,?)");
            query.addBindValue(fname + " " + lname);
            query.addBindValue(0);
            query.addBindValue(addRFID);

            if(query.exec()){

                qDebug() << "Ajouté pour la séance !";

            }else{

                QMessageBox::information(this,"","Erreur..." + query.lastError().text());
            } */



        }else{ //Dans le cas ou il y a une erreur

            QMessageBox::information(this,"","Erreur..." + query.lastError().text());
        }


    }else{ //Dans le cas ou le tag n'a pas 10 chiffres


           QMessageBox::warning(this,"Information","Le n° du tag doit comporter 10 chiffres !");


    }

}

void MainWindow::replacment(QString &rfid)
{

    //Remplacement des caractères en cas de majuscule désactivé

    rfid.replace("&","1");
    rfid.replace("é","2");
    rfid.replace("\"","3");
    rfid.replace("'","4");
    rfid.replace("(","5");
    rfid.replace("-","6");
    rfid.replace("è","7");
    rfid.replace("_","8");
    rfid.replace("ç","9");
    rfid.replace("à","0");
    rfid.replace("\r\n", NULL);
    rfid.replace("\r", NULL);
    rfid.replace("\n", NULL);

}

void MainWindow::delay(int s)
{

    QTime dieTime= QTime::currentTime().addSecs(s);
            while (QTime::currentTime() < dieTime)
    QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

}

void MainWindow::on_playerTableView_doubleClicked(const QModelIndex &index)
{


    QString name = index.data(0).toString();

    QSqlQuery q;

    q.prepare("SELECT N°deTag FROM Joueurs WHERE Prenoms = \"" + name + "\"");

    q.exec();

    q.next();

       qDebug() << "Resultat: " << q.value(0).toString();


    //query.exec("SELECT Noms FROM Players WHERE Prenoms = \""+ index.data(0).toString() +"\"");


    QMessageBox reponse;                                                            //apparition de la fenêtre
    reponse.setText("Le joueur choisi est :  " + index.data(0).toString());
    reponse.setInformativeText("Voulez-vous lancez la séance ?");                   //Avertir de l'utilisateur si il veut lancer une séance


    //Deux options : soit un bouton "oui" ou soit un bouton "non"
    QAbstractButton* pButtonYes = reponse.addButton(tr("Oui"), QMessageBox::YesRole); reponse.addButton(tr("Non"), QMessageBox::NoRole);

    reponse.exec();

    if (reponse.clickedButton()==pButtonYes) { //Si l'utilisateur clique sur "Oui"

        qDebug() << "Yes";

        trainingFunction(); //Ce code execute la fonction de la séance

    }

        else{ //Si l'utilisateur clique sur "Non"

        qDebug() << "No";

        ui->seanceRFID->clear();

       }

}

void MainWindow::on_activePortButton_clicked()
{


    if(peripherique->isOpen()){

        peripherique->close();
    }

    peripherique->setPortName(portSelectionne);
    peripherique->setBaudRate(QSerialPort::Baud9600);
    peripherique->setDataBits(QSerialPort::Data8);
    peripherique->setParity(QSerialPort::NoParity);
    peripherique->setFlowControl(QSerialPort::NoFlowControl);
    peripherique->setStopBits(QSerialPort::OneStop);

    connecte = peripherique->open(QIODevice::ReadWrite);

    if((connecte && out.contains("USB"))){

        QMessageBox::warning(this,"Information", "Laser pret !");

    }else{

        QMessageBox::warning(this,"Information", "Erreur Port Serie... !");

    }

    peripherique->clear();
}

void MainWindow::on_comboBoxPort_currentIndexChanged(const QString &arg1)
{
    int sel=ui->comboBoxPort->currentData().toInt();
        out="Siretta Modem Connection Info: \n";
        out+=listeDesPorts[sel].portName()+" "+listeDesPorts[sel].description()+"\n";
        out+=listeDesPorts[sel].systemLocation()+"\n";
        if (listeDesPorts[sel].hasVendorIdentifier()&& listeDesPorts[sel].hasProductIdentifier())
           out+=listeDesPorts[sel].manufacturer()+" ("+ QString::number(listeDesPorts[sel].vendorIdentifier(),16) + ":" + QString::number(listeDesPorts[sel].productIdentifier(),16) + ")";

        ui->label_readPeripherique->setText(out);

        portSelectionne = listeDesPorts[sel].portName();
        ui->label_Port->setText("Port : " + portSelectionne);

}

void MainWindow::on_laserON_clicked()
{
    peripherique->write("1");
}

void MainWindow::on_laserOFF_clicked()
{
    peripherique->write("0");
}

void MainWindow::trainingFunction() //Fonction de la séance
{
    QSqlQuery q;
    modelTraining->setQuery("SELECT * FROM Seance");          //Affichage des données du joueur (table séance)
    QMessageBox::warning(this,"Attention!", "Laser activé");  //Message pour prévenir que le laser s'est activé
    peripherique->write("1");                                 //Activation du laser
    ui->seanceRFID->clear();                                  //Ce supprime le Tag scanné de l'edit de texte
    ui->trainingEndB->show();                                 //Le bouton "Terminer la séance" apparait
    serialBuffer.clear();                                     //expliqué dans "Fonctionnement de la barrière immatérielle"

    q.prepare("SELECT joueur FROM Seance");
    q.exec();
    q.next();

    QString nomDuJoueur = q.value(0).toString();
    e = 0;
    int tirs = 1;

    while(e == 0){ // Une fois la séance en marche, on utilise une boucle avec une condition e

    if(serialBuffer.contains("G")) { //Dans le cas où la carte écrit "G" (si la barrière est coupé)

                qDebug() << "GOAL!";

                q.prepare("UPDATE Seance SET date = :date, nombredetirs = :tir WHERE Joueur = :nomjoueur");

                q.bindValue(":date",0);                     //On ajoute toute les informations
                q.bindValue(":tir", tirs);                  //
                q.bindValue(":nomjoueur",nomDuJoueur);      //
                tirs = tirs+1;                              // +1 tir

                ui->greenCLabel->hide();
                ui->redCLabel->show();
                ui->infoLabel->show();
                delay(5);

                if (!q.exec()){

                    qDebug() << "Error" + q.lastError().text();
                }

                modelTraining->setQuery("SELECT * FROM Seance");
                ui->infoLabel->hide();
                ui->greenCLabel->show();
                ui->redCLabel->hide();
                peripherique->write("1");
                serialBuffer.clear();

    } else if (serialBuffer.contains("O")){

                qDebug() << "TIME OUT!";

                QMessageBox::warning(this,"Information", "Laser désactivé, Séance terminée");

                exportSeanceDataBase();
                modelTraining->setQuery("DELETE FROM Seance");
                ui->labelStatut->setText("Etat de la seance : Terminée");
                delay(5);
                ui->labelStatut->setText("Etat de la seance : Pas de seance actuellement");
                serialBuffer.clear();

                e = 1;

    }

        delay(1);

    }

    QMessageBox::warning(this,"Information", "Laser désactivé, Séance Terminée");

    exportSeanceDataBase();

    modelTraining->setQuery("DELETE FROM Seance");

    ui->labelStatut->setText("Etat de la seance : Terminée");
    ui->infoLabel->show();
    ui->infoLabel->setText("Données exporté !");

    delay(5);

    ui->labelStatut->setText("Etat de la seance : Pas de seance actuellement");
    ui->greenCLabel->show();
    ui->redCLabel->hide();
    ui->infoLabel->hide();
    ui->trainingEndB->hide();



}

void MainWindow::on_seanceRFID_returnPressed()
{

    QString rfid = ui->seanceRFID->text();

    replacment(rfid);

    QSqlQuery q;

    q.prepare("SELECT Prenoms, Noms FROM Joueurs WHERE N°deTag = \"" + rfid + "\"");

    q.exec();

    if (!q.next()){


        qDebug() << "RFID inconnu";

        QMessageBox::information(this, "Erreur!", "Tag RFID Inconnu");
        ui->seanceRFID->clear();

    }else{

       qDebug() << "Resultat: " << q.value(0).toString();

       QString prenoms = q.value(0).toString();
       QString noms = q.value(1).toString();





    QMessageBox reponse;                                                            //apparition de la fenêtre
    reponse.setWindowTitle("Confirmation");
    reponse.setText("Le joueur choisi est :  " + prenoms + " "+ noms);
    reponse.setInformativeText("Voulez-vous lancez la séance ?");                   //Avertir de l'utilisateur si il veut lancer une séance


    //Deux options : soit un bouton "oui" ou soit un bouton "non"
    QAbstractButton* pButtonYes = reponse.addButton(tr("Oui"), QMessageBox::YesRole); reponse.addButton(tr("Non"), QMessageBox::NoRole);

    reponse.exec();

    if (reponse.clickedButton()==pButtonYes) { //Si l'utilisateur clique sur "Oui"

        qDebug() << "Yes";

        on_activePortButton_clicked();

        q.prepare("INSERT INTO Seance (joueur,date,tag_id,nombredetirs)" //Commande pour ajouter un joueur...
              "VALUES (?,?,?,?)");
        q.addBindValue(prenoms + " " + noms);                                        //...avec tout les paramètres
        q.addBindValue(0);
        q.addBindValue(rfid);
        q.addBindValue(0);

        if(q.exec()){

            qDebug() << "Ajouté pour la séance !";

        }else{

            QMessageBox::information(this,"","Erreur..." + q.lastError().text());
        }
        ui->seanceRFID->setReadOnly(true);
        ui->seanceRFID->setPlaceholderText("Seance en cours...");

        trainingFunction(); //Ce code execute la fonction de la séance

        ui->seanceRFID->setReadOnly(false);
        ui->seanceRFID->setPlaceholderText("Scanner un Tag RFID");

    }

        else{ //Si l'utilisateur clique sur "Non"

        qDebug() << "No";

        ui->seanceRFID->clear();

    }

    }
}

void MainWindow::exportSeanceDataBase()
{
    QSqlQuery q;
    modelTraining->setQuery("SELECT * FROM Seance");


    q.prepare("SELECT * FROM Seance");
    q.exec();
    q.next();

    QFile file("c:/Users/hugos/OneDrive/Documents/Cours/Projet/InfoSeanceListe/InfoJoueur" + q.value(0).toString() + ".csv");
        if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)){
            qDebug() << "Failed!";
            return;
        }

        QTextStream out(&file);
        //file.seek(80);


        QString tableSeance =
                  q.value(0).toString() + ","
                + q.value(1).toString() + ","
                + q.value(2).toString() + ","
                + q.value(3).toString();

        qDebug() << "stream: " << tableSeance;
        out << tableSeance;


}

void MainWindow::on_trainingEndB_clicked()
{
    e = 1;

    qDebug() << "Finish !";

}
