#include "widget.h"
#include "ui_widget.h"
#include "httpstress.h"

#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDir>
#include <QTextCodec>
#include <QString>
#include <QScrollArea>
#include <QProgressBar>
#include <QStandardPaths>

#include <iostream>

using namespace std;


//////////////////////////
widget::widget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::widget)
{
    ui->setupUi(this);
    aftimer = NULL; //time_на_интервалы

    flag1 = 0;
    flag2 = 0;
    wgt = new QWidget();
    logLabel = new QLabel(wgt);
    scrollArea = new QScrollArea(wgt);
    gridLayout = new QGridLayout(scrollArea->viewport());
    scrollArea->setWidget(logLabel);
    wgt->setLayout(gridLayout);
    wgt->setWindowTitle("log");
     logLabel->adjustSize();
     logLabel->setFrameStyle(QFrame::Panel | QFrame::Sunken);
     logLabel->setAlignment(Qt::AlignCenter);
     logLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    scrollArea->setAlignment(Qt::AlignCenter);
    scrollArea->setMinimumSize(800,375);
    scrollArea->setBackgroundRole(QPalette::Dark);
    scrollArea->setWidgetResizable(true);
    gridLayout ->addWidget(scrollArea,0,0,0);

   QColor dark(Qt::darkGray);
   QPalette pl; pl.setColor (QPalette::Active, QPalette::Window, dark);
   wgt->setPalette(pl);
   wgt->setStyleSheet(
               "QWidget {border: 0.1px solid DarkCyan ;text-align: center;"
               "color:rgba(255,255,250,255);"
               "border-radius: 2px;"
               "border-width: 2px;"
               "border-image: 9,2,5,2; "
               "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(70, 70, 70, 255), stop:1 rgba(10, 0, 0, 200));"
               "}"
   );
   logLabel->setAutoFillBackground(true);

    connect(ui->requestBtn,SIGNAL(clicked()),this,SLOT(slot_requestBtnClicked()));
    //
    connect(ui->reqbtnpost,SIGNAL(clicked()),this,SLOT(slot_reqbtnpost()));
    connect(ui->reqbtnfile,SIGNAL(clicked()),this,SLOT(slot_reqbtnfile()));
    //
    connect(ui->tableWidget,SIGNAL(cellClicked(int, int)),this,SLOT(slot_clickItem(int,int)));
    //
    loadData();
}
//////////////////////////
widget::~widget()
{
    delete ui;
}
//////////////////////////
//update_widget
void widget::updateRequestInfo()
{
    int rowCount = ui->tableWidget->rowCount();
    for(int i = 0;i < rowCount; i++){
        ui->tableWidget->item(i,0)->setText(https[i]->getRequestTitle());
        ui->tableWidget->item(i,1)->setText(https[i]->getRequestUrl());
        ui->tableWidget->item(i,3)->setText("see_data");
        ui->tableWidget->item(i,4)->setText(QString::number(https[i]->getIntervalTime()));
        ui->tableWidget->item(i,5)->setText(QString::number(https[i]->getTimeOut()));
        ui->tableWidget->item(i,8)->setText(https[i]->getRequestTimes());
        ui->tableWidget->item(i,13)->setText(https[i]->getdelay());
        ui->tableWidget->item(i,14)->setText(https[i]->getmean());
    }
}
void widget::addItemTowidget(const QString &strUrl, const QString &title, const int &outTime,
                                 const int &intervalTime,const QString & type_info2)//, const QString &start, const QString &end
{
    int rowCount = ui->tableWidget->rowCount();
    int colCount = ui->tableWidget->columnCount();

    ui->tableWidget->insertRow(rowCount);
    ui->tableWidget->setItem(rowCount,0,new QTableWidgetItem(title));
    ui->tableWidget->setItem(rowCount,1,new QTableWidgetItem(strUrl));
    ui->tableWidget->setItem(rowCount,2,new QTableWidgetItem(type_info2));
    ui->tableWidget->setItem(rowCount,3,new QTableWidgetItem("data"));
    ui->tableWidget->setItem(rowCount,4,new QTableWidgetItem(QString::number(intervalTime)));
    ui->tableWidget->setItem(rowCount,5,new QTableWidgetItem(QString::number(outTime)));
    ui->tableWidget->setItem(rowCount,6,new QTableWidgetItem("новый"));

    ui->tableWidget->setItem(rowCount,7,new QTableWidgetItem("новый"));
    ui->tableWidget->setItem(rowCount,8,new QTableWidgetItem("0"));
    ui->tableWidget->setItem(rowCount,9,new QTableWidgetItem(QIcon(QPixmap(":/f/f/player.png")),tr("start")));
    ui->tableWidget->setItem(rowCount,10,new QTableWidgetItem(QIcon(QPixmap("::/f/f/edit.jpg")),tr("edit")));
    ui->tableWidget->setItem(rowCount,11,new QTableWidgetItem(QIcon(QPixmap(":/f/f/delete.png")),tr("del")));
    ui->tableWidget->setItem(rowCount,12,new QTableWidgetItem(QIcon(QPixmap(":/f/f/show.jpg")),tr("log")));
    ui->tableWidget->setItem(rowCount,13,new QTableWidgetItem("wait"));
    ui->tableWidget->setItem(rowCount,14,new QTableWidgetItem("wait"));
    //Центр текста
    ui->tableWidget->item(rowCount,0)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,1)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,2)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,3)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,4)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,5)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,6)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,7)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,8)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,9)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,10)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,11)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,12)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,13)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    ui->tableWidget->item(rowCount,14)->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
    //не редактируются flag totalizator
    ui->tableWidget->item(rowCount,0)->setFlags(ui->tableWidget->item(rowCount,0)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,1)->setFlags(ui->tableWidget->item(rowCount,0)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,2)->setFlags(ui->tableWidget->item(rowCount,1)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,3)->setFlags(ui->tableWidget->item(rowCount,2)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,4)->setFlags(ui->tableWidget->item(rowCount,3)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,5)->setFlags(ui->tableWidget->item(rowCount,4)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,6)->setFlags(ui->tableWidget->item(rowCount,5)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,7)->setFlags(ui->tableWidget->item(rowCount,6)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,8)->setFlags(ui->tableWidget->item(rowCount,7)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,9)->setFlags(ui->tableWidget->item(rowCount,8)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,10)->setFlags(ui->tableWidget->item(rowCount,9)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,11)->setFlags(ui->tableWidget->item(rowCount,10)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,12)->setFlags(ui->tableWidget->item(rowCount,11)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,13)->setFlags(ui->tableWidget->item(rowCount,12)->flags() & ~Qt::ItemIsEditable);
    ui->tableWidget->item(rowCount,14)->setFlags(ui->tableWidget->item(rowCount,13)->flags() & ~Qt::ItemIsEditable);
}
void widget::deleteItem(const int &row)//удаление
{
    delete https[row];
    https.removeAt(row);
    ui->tableWidget->removeRow(row);
}
void widget::showLog(const int &row)//журнал
{
    QFile file(https[row]->getLogFileName());
    if (file.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream toText(&file);

        toText.setCodec("UTF-8");
        logLabel->setText((toText.readAll()));
         wgt->show();
    }
}
void widget::startRequest(int row,int col)//Начать запрос
{

    https[row]->sendRequest();
    ui->tableWidget->item(row,7)->setText("processing");
    ui->tableWidget->item(row,col)->setText("stop");
    ui->tableWidget->item(row,col)->setIcon(QIcon(QPixmap(":/f/f/pause.png")));

    https[row]->startIntervalTimer();
   // https[row]->startIntervalTimer();

    slot_flushWidget();
    if (!aftimer)//(обновление)
    {
        aftimer = new QTimer();
        connect(aftimer,SIGNAL(timeout()),this,SLOT(slot_flushWidget()));////////////сним траблы
        aftimer->start(500);

    }
}
void widget::editRequest(int row,int col)//Изменить запрос
{
    ewidjet *edit = new ewidjet(this,https[row]);//пересыл
    edit->setStyleSheet(
                "QWidget {border: 0.1px solid DarkCyan ;text-align: center;"
                "color:rgba(255,255,250,255);"
                "border-radius: 2px;"
                "border-width: 2px;"
                "border-image: 9,2,5,2; "
                "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(70, 70, 70, 255), stop:1 rgba(10, 0, 0, 200));"
                "}"
                );
    edit->show();
}
void widget::stopRequest(int row,int col)//Стоп запрос
{
    ui->tableWidget->item(row,7)->setText("ожидание");
    ui->tableWidget->item(row,col)->setText("start");
    ui->tableWidget->item(row,col)->setIcon(QIcon(QPixmap(":/f/f/player.png")));
    https[row]->closeIntervalTimer();
    slot_flushWidget();
}
void widget::delRequest(int row,int col)
{
    QMessageBox box(QMessageBox::Warning,"Udalenie","Dell??");
    box.setStandardButtons (QMessageBox::Ok|QMessageBox::Cancel);
    box.setButtonText (QMessageBox::Ok,QString("da"));
    box.setButtonText (QMessageBox::Cancel,QString("cancel"));
    if(box.exec() == QMessageBox::Ok){
        deleteItem(row);
    }
}
void widget::loadData()
{
    QFile file(data_file_name);
    if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {qDebug()<<"Can't open the file!"<<endl;}
    QString strUrl = 0;
    int outTime = 0;
    QString title = 0;
    int timeInterval = 0;
    QString type_info2 = 0;
    QString pos_text = "_example_post";
    int stop2=0;
    QRegExp rx("*_postend*");
    rx.setPatternSyntax(QRegExp::Wildcard);
    QString peremen_t;
        while(!file.atEnd())
        {
            QTextCodec *codec=QTextCodec::codecForName("UTF-8");
            QString buf1 = codec->toUnicode(file.read(1));
            for (int i =0; i<buf1.size();++i)
            {
              if(peremen_t == "_object_")
                {
                  stop2 = 1;
                  peremen_t = "";
                  break;
                 }
              if(rx.exactMatch(peremen_t) == true && stop2 == 1)
                {
                  peremen_t.remove("_postend");
                  stop2 = 0;
                  //тут на пост
                  pos_text=peremen_t;
                   peremen_t = "";
                  qDebug()<<"reretbreak3 ";

                  httpstress* phttpstress = new httpstress();
                  https.enqueue(phttpstress);
                  connect(phttpstress,SIGNAL(signal_requestFinished(bool,const QString&)),
                          this,SLOT(slot_requestFinished(bool,const QString&)));
                  phttpstress->init(strUrl,outTime,timeInterval,title,type_info2,pos_text);//startTime,endTime,post_info
                  addItemTowidget(strUrl,title,outTime,timeInterval,type_info2);
                 break; //
                 }
              peremen_t += (buf1[i]);
              qDebug()<<"peremen_t " << peremen_t;

              if(buf1[i] == '\n' && stop2 ==0)//готовая строка
                {
                  for (int i =0; i<peremen_t.size();++i)
                   {
                      if(peremen_t.size() < 5)
                        {
                         break;
                         }
                    QStringList strlist=peremen_t.split(",");
                    qDebug()<< strlist;

                    strUrl = strlist.at(0).trimmed();
                    outTime = strlist.at(3).trimmed().toInt();
                    title = strlist.at(1).trimmed();
                    timeInterval = strlist.at(2).trimmed().toInt();
                    type_info2 = strlist.at(4).trimmed();
                    break;
                    }
               peremen_t = "";
               break; //'\n'
            }
         }
    }
}

//указатель на сохранение
void widget::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);
    on_savepbtn_clicked();
    on_saveplog2();

}
//////////////////////////
void widget::slot_requestBtnClicked()//zapros strget
{
    httpstress* phttpstress = new httpstress();
    https.enqueue(phttpstress);

    QString strUrl = ui->addressEdit->text().trimmed();
    int outTime = ui->timeoutEdit->text().toInt();
    QString title = ui->titleEdit->text();
    int timeInterval = ui->timeIntervalEdit->text().toInt();
    QString type_info2 = "get";
    QString pos_text = "nopost";

    //сигнал завершения запроса http
    connect(phttpstress,SIGNAL(signal_requestFinished(bool,const QString&)),
            this,SLOT(slot_requestFinished(bool,const QString&)));
    //Параметры инициализации
    phttpstress->init(strUrl,outTime,timeInterval,title,type_info2,pos_text);//startTime,endTime,
    //Добавить во все списки
    addItemTowidget(strUrl,title,outTime,timeInterval,type_info2);//,startTime,endTime
}
void widget::slot_reqbtnpost()//zapros str
{
    httpstress* phttpstress = new httpstress();// zapros strpost
    https.enqueue(phttpstress);

    QString strUrl = ui->addressEdit->text().trimmed();
    int outTime = ui->timeoutEdit->text().toInt();
    QString title = ui->titleEdit->text();
    int timeInterval = ui->timeIntervalEdit->text().toInt();
    QString type_info2 = "post";
    QString pos_text = ui->textEdit->toPlainText();

    connect(phttpstress,SIGNAL(signal_requestFinished(bool,const QString&)),/////////////////////
            this,SLOT(slot_requestFinished(bool,const QString&)));//ловим сигнал
    phttpstress->init(strUrl,outTime,timeInterval,title,type_info2,pos_text);//startTime,endTime,,post_info
    addItemTowidget(strUrl,title,outTime,timeInterval,type_info2);//,startTime,endTime
}
void widget::slot_reqbtnfile()//zapros str
{
    {
    wgt2 = new QWidget();
    wgt2->setParent(this, Qt::Window);
    urlLineEdit = new  QLineEdit(wgt2);
    urlLineEdit->setText(ui->addressEdit->text().trimmed());
    targetFolderLineEdit = new QLineEdit(wgt2);
    targetFolderLineEdit->setText("target_folder");
    selectTargetFolderPushButton  = new QPushButton(wgt2);
    selectTargetFolderPushButton->setText("select_folder");
    downloadPushButton  = new QPushButton(wgt2);
    downloadPushButton->setText("download_start");
    downloadProgressBar  = new QProgressBar(wgt2);
    downloadProgressBar->setTextVisible(true);
    cancelPushButton  = new QPushButton(wgt2);
    cancelPushButton->setText("cancel");

    gridLayout = new QGridLayout();
    wgt2->setLayout(gridLayout);
    wgt2->setWindowTitle("vibor_puti");

    gridLayout ->addWidget(urlLineEdit);
    gridLayout ->addWidget(targetFolderLineEdit);
    gridLayout ->addWidget(selectTargetFolderPushButton);
    gridLayout ->addWidget(downloadPushButton);
    gridLayout ->addWidget(downloadProgressBar);
    gridLayout ->addWidget(cancelPushButton);

   QColor dark(Qt::darkGray);
   QPalette pl; pl.setColor (QPalette::Active, QPalette::Window, dark);
   wgt2->setPalette(pl);
   wgt2->setStyleSheet(
               "QWidget {border: 0.1px solid DarkCyan ;text-align: center;"
               "color:rgba(255,255,250,255);"
               "border-radius: 2px;"
               "border-width: 2px;"
               "border-image: 9,2,5,2; "
               "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(70, 70, 70, 255), stop:1 rgba(10, 0, 0, 200));"
               "}"
   );
    wgt2->show();
    connect(downloadPushButton, &QPushButton::clicked, this, &widget::onDownloadButtonClicked);
    connect(selectTargetFolderPushButton, &QPushButton::clicked, this, &widget::onSelectTargetFolderButtonClicked);
       connect(cancelPushButton, &QPushButton::clicked, this, &widget::onCancelButtonClicked);
       connect(&m_downloader, &Downloader::updateDownloadProgress, this, &widget::onUpdateProgress);
       connect(&m_downloader,&Downloader::sonDownloadButtonClicked,this, &widget::onDownloadButtonClicked);
    }
}

void widget::onDownloadButtonClicked()
{
    {
    QString strUrl = urlLineEdit->text().trimmed();
    QString title = ui->titleEdit->text();
    QString type_info2 = "file";
    m_downloader.get(targetFolderLineEdit->text(), urlLineEdit->text());
    }
}

void widget::onSelectTargetFolderButtonClicked()
{
    QString targetFolder = QFileDialog::getExistingDirectory(this,tr("Select folder"),
                         QStandardPaths::writableLocation(QStandardPaths::DownloadLocation),
                         QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
            targetFolderLineEdit->setText(targetFolder);
}
void widget::onCancelButtonClicked()
{
    flag1 = 0;
    m_downloader.cancelDownload();
    downloadProgressBar->setMaximum(100);
    downloadProgressBar->setValue(0);
}
void widget::onUpdateProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    downloadProgressBar->setMaximum(bytesTotal);
    downloadProgressBar->setValue(bytesReceived);
}
////////////////////////////////////////////
void widget::slot_requestFinished(bool bSuccess, const QString &strResult)
{       qDebug() << "slot_requestFinished" ;
    qDebug() << bSuccess;
    if(bSuccess)
    {
        qDebug()<< strResult;

    }
}
//////////////////////////
void widget::slot_clickItem(const int &row, const int &col)//элемент в таблице
{
    qDebug()<<row<<"-"<<col;
    QString itemText = ui->tableWidget->item(row,col)->text();
    ui->textEdit->setText(https[row]->getpos_text());

    if(col == 9)
    {
        //Начать паузу
        if(itemText == "stop"){
            stopRequest(row,col);
        }else{
            startRequest(row,col);

        }
    }
    else if(col == 2)
    {
        if (itemText == "get")
             {
        logLabel->setText(https[row]->getreqdata());
             }

        if (itemText == "post")
            {
            logLabel->setText(ui->textEdit->toPlainText());////////////

            }
        if (itemText == "file")
            {
            logLabel->setText(ui->textEdit->toPlainText());////////////////
            }
        logLabel->setTextFormat(Qt::PlainText);//только текст не преобразуем в страницу
        logLabel->setAlignment(Qt::AlignLeft);
        wgt->show();

    }
    else if(col == 3)
    {
        logLabel->setText(https[row]->getreqdata());
        logLabel->setTextFormat(Qt::PlainText);//только текст не преобразуем в страницу
        logLabel->setAlignment(Qt::AlignLeft);
        wgt->show();

    }
    else if(col == 10)
    {
        editRequest(row,col);

    }
    else if(col == 11)
    {
        delRequest(row,col);
    }
    else if(col == 12)
    {
        qDebug()<< "Просмотр журналов";
        showLog(row);
    }
    else if(col == 13)
    {
        qDebug()<< "Просмотр delays";
        showLog2(row);
    }
    else if(col == 14)
    {
        qDebug()<< "Просмотр delays||means";
        showLog2(row);
    }
}
//////////////////////////
void widget::slot_flushWidget()
{
    updateRequestInfo();
}
void widget::on_allStopPbtn_clicked()//Приостановить все
{

    int rowCount = ui->tableWidget->rowCount();
    for(int i = 0;i < rowCount; i++)
    {
        ui->tableWidget->item(i,7)->setText("ожидание");

        QString itemText = ui->tableWidget->item(i,9)->text();
        //Начать паузу
        if(itemText == "stop")
        {
            stopRequest(i,9);
        }
    }
}
void widget::on_allStartPbtn_clicked()
{
    int rowCount = ui->tableWidget->rowCount();
    for(int i = 0;i < rowCount; i++){
        ui->tableWidget->item(i,7)->setText("zaprosok");

        QString itemText = ui->tableWidget->item(i,9)->text();
        //stop
        if(itemText == "start"){
            startRequest(i,9);
            qDebug() << "Is SSL Enabled? " << QSslSocket::supportsSsl();
            qDebug() << "SSL Library Build Version (Qt compiled against): " << QSslSocket::sslLibraryBuildVersionString();
            qDebug() << "SSL Library Version String (available locally): " << QSslSocket::sslLibraryVersionString();
            qDebug() << QSslSocket::supportsSsl() << QSslSocket::sslLibraryBuildVersionString() << QSslSocket::sslLibraryVersionString();
        }
    }
}
void widget::on_savepbtn_clicked()
{
    QDir *fileHander = new QDir;
    bool exist = fileHander->exists("data");
    if(!exist)
        bool ok = fileHander->mkdir("data");
    fileHander->remove(data_file_name);
    QString fileName = data_file_name;
    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly  | QIODevice::Text|QIODevice::Append))
    {
        qDebug()<<"can't open";
    }
     QQueue<httpstress*>::iterator item;
     for (item = https.begin(); item != https.end(); ++item) {
           QString data_line = (*item)->getRequestUrl().trimmed() + ","
                  + (*item)->getRequestTitle().trimmed() +","
                 + QString::number((*item)->getIntervalTime()).trimmed() +","
                 + QString::number((*item)->getTimeOut()).trimmed() +","
                 + (*item)->getype_info2().trimmed() +"\n_object_\n"
                 + (*item)->getpos_text() +"_postend\n";
        QTextStream in(&file);
        in<<data_line<<"\n";
   }  file.close();
}
void widget::on_saveplog2()
{
    QDir *filedelay = new QDir;
    bool exist = filedelay->exists("data");
    if(!exist)
        bool ok = filedelay->mkdir("data");

    filedelay->remove(data_file_name2);
    QString fileName2 = data_file_name2;
    QFile file(fileName2);
    if(!file.open(QIODevice::WriteOnly  | QIODevice::Text|QIODevice::Append))
    {
        qDebug()<<"can't open";
    }
     QQueue<httpstress*>::iterator item2;
     for (item2 = https.begin(); item2 != https.end(); ++item2) {
          QString data_line2 =  (*item2)->getRequestUrl().trimmed() + "||"
                  + (*item2)->getRequestTitle().trimmed() +"||"
                 + QString::number((*item2)->getIntervalTime()).trimmed() +"||"
                 + QString::number((*item2)->getTimeOut()).trimmed() +"||"
                 + (*item2)->getdelay().trimmed() +"||"
                 + (*item2)->getmean().trimmed() +"||"
                 + (*item2)->getype_info2().trimmed();
        QTextStream in(&file);
        in<<data_line2<<"\n_object_\n";
     }
     file.close();
}
void widget::showLog2(const int &row)//журнал
{
    QFile file2(https[row]->getLogFileName2());
    if (file2.open(QFile::ReadOnly | QFile::Text))
    {
        QTextStream toText(&file2);
        toText.setCodec("UTF-8");
        logLabel->setText((toText.readAll()));
        wgt->show();
    }
}
void widget::on_checkBox_clicked(bool checked)//для лога
{
    QQueue<httpstress*>::iterator item;
    for (item = https.begin(); item != https.end(); ++item) {
      (*item)->setLogSwitch(checked);
    }
}
