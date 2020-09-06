#include "Ewidjet.h"
#include "ui_ewidjet.h"

ewidjet::ewidjet(QWidget *parent):
    QWidget(parent),
    ui(new Ui::ewidjet)
{
    ui->setupUi(this);
}
ewidjet::ewidjet(widget *widget,httpstress *httppocaz):
    QWidget(),
    ui(new Ui::ewidjet)
{
   zadh = httppocaz;
   zadw = widget;

   ui->setupUi(this);
   showHttpValue();
}
ewidjet::~ewidjet()
{
    delete ui;
}
void ewidjet::showHttpValue()
{
    if (zadh)
    {
        ui->requestUrl->setText(zadh->getRequestUrl());
        ui->requestTitle->setText(zadh->getRequestTitle());
        ui->intervalTime->setText(QString::number(zadh->getIntervalTime()));
        ui->timeOut->setText(QString::number(zadh->getTimeOut()));
        ui->pos3->setText(zadh->getpos_text());
    }
}
void ewidjet::on_sureBtn_clicked()//clckOK
{
    QString url = ui->requestUrl->text();
    QString title = ui->requestTitle->text();

    int intervalTime = ui->intervalTime->text().toInt();
    int timeOut = ui->timeOut->text().toInt();

    QString type_info2;
    QString pos_text = ui->pos3->text();

    zadh->init(url,timeOut,intervalTime,title,type_info2,pos_text);
    zadw->updateRequestInfo();

    this->close();
    this->deleteLater();
}
void ewidjet::on_cancleBtn_clicked()
{
    this->close();
    this->deleteLater();
}
