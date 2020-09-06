#include "httpstress.h"
#include <QTextCodec>
#include <QMessageBox>
#include <QDir>
#include <QThreadPool>

#include <QEventLoop>
#include <QNetworkCookieJar>

const int time_n = 10000; //10s
///////////////////////////////////////////////////////////////
httpstress::httpstress(QObject *parent) :
    QObject(parent)

{ qDebug()<< "httpstress";
    aNetworkManager = new QNetworkAccessManager(this); //newc
    aNetworkReply = NULL;

    aIntervalTimer = NULL;
    aIntervalTime = 10000;
    aOutTime = 10000;

    err = 0;
    err2 = 0;
    err3 = 1;

    mean1 = 0;
    mean2 = 0;
    mean3 = 0;

    //cook="";


    aTitle = "name";
    apos_text  = "_example_post";
    aRequestTimes = 0;//колво прошедших запросов
    delay1 =0;
    aRequestState = "not_start"; //статус_д
    aLogOpen = false;//bool откр_лог

    aStart = false;//bool начин_запись
    aOutTimer = new QTimer;//новый таймер отсчета
    timer = new QTimer;
    timer->setSingleShot (true);

    tt = 1.0;

    connect(aOutTimer,SIGNAL(timeout()),this,SLOT(slot_requestTimeout())); //притягиваем к слоту первый запрос
                                                                           //для обновления счетчика
}
    //param init
void httpstress::init(const QString &strUrl, const int &outTime, const int &intervalTime,
                    const QString &title,const QString &type_info2,const QString &pos_text)//const QString &startTimer, const QString &endTimer,
{
    astrUrl = strUrl;
    aIntervalTime = intervalTime*1000;
    aOutTime = outTime*1000;
    aTitle = title;
    atype_info2 =  type_info2;

    apos_text =  pos_text;
}
//////////////////////////////////////////////////////////////////////
//1
void httpstress::sendRequest()//реквест

{
        t.start();//отсчт задержки

        QNetworkRequest netRequest;
        netRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

        netRequest.setUrl(QUrl(astrUrl)); //URL

        if(astrUrl.toLower().startsWith("https"))
        {
            QSslConfiguration sslConfig;
            sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
            sslConfig.setProtocol(QSsl::TlsV1_1);
            netRequest.setSslConfiguration(sslConfig);
        }
        aNetworkReply = aNetworkManager->get(netRequest);
        if(aNetworkReply->isFinished() != true)
        {
        if (aNetworkReply->error() == QNetworkReply::NoError)
         {
           err3=1;
           //connect(aNetworkReply,SIGNAL(finished()),this,SLOT(slot_requestFinished())); //нормальный и ошибочный нетсоеиднения
         }
        }
        aOutTimer->start(aOutTime);
        qDebug("sendTime elapsed: %d ms", t.elapsed());

        aIntervalTimer = new QTimer;
        connect(aIntervalTimer,SIGNAL(timeout()),this,SLOT(slot_intervalTimeRequest()));
        aIntervalTimer->start(aIntervalTime);

}
////////////////////////////////////////////////////////////////
void httpstress::closeIntervalTimer()//Таймер закрытия интервала
{qDebug()<< "closeIntervalTimer";
    if(!aIntervalTimer)
    {
        aIntervalTimer = new QTimer;
        connect(aIntervalTimer,SIGNAL(timeout()),this,SLOT(slot_intervalTimeRequest()));
    }
    aRequestState = "Не_отправляется";
    aIntervalTimer->stop();
    aOutTimer->stop();
    timer->stop();

    qDebug()<< "+= sssscloseIntervalTimer";
}

void httpstress::startIntervalTimer()//Таймер интервала запуска

{qDebug()<< "startIntervalTimer";
    if(!aIntervalTimer)
    {
        aIntervalTimer = new QTimer;
        connect(aIntervalTimer,SIGNAL(timeout()),this,SLOT(slot_intervalTimeRequest()));
    }
    aRequestState = "request_in_process";
    aIntervalTimer->start(aIntervalTime);
}
////////////////////////////////////////////////////////////////
void httpstress::log(const QString &log_str)
{
    if(!aLogOpen)
        return;

    QDateTime time = QDateTime::currentDateTime();
    QString currTime = time.toString("yyyy-MM-dd hh:mm:ss");
    //формат лога
    QString log_result = "URL:" + astrUrl + " ; " + "date:" + currTime +" ; " + "result:" + log_str;

    QDir *TEST = new QDir;//папка_лога

    bool exist = TEST->exists("log");
    if(!exist)
        bool ok = TEST->mkdir("log");
    //имя
    QString fileName = "log/" + aTitle +"_" + time.toString("yyyy-MM-dd") + ".txt";

    aLogFileName = fileName;
    QString str = log_result;
    QFile file(fileName);

    QFileInfo fileinfo(fileName);

    if (fileinfo.size() > 1024*1024)
    {
        QTime currTime = QTime::currentTime();
        int currTimeStamp = currTime.hour()*3600 + currTime.minute()*60 + currTime.second();

        QString newfileName = "log" + aTitle +"_" + time.toString("yyyy-MM-dd")+
                "_"+QString::number(currTimeStamp) + ".txt";
        file.rename(newfileName);
    }
    if(!file.open(QIODevice::WriteOnly  | QIODevice::Text|QIODevice::Append))
    { qDebug()<<"can't open";}
    QTextStream in(&file);
    in<<str<<"\n";
    file.close();
}
void httpstress::log2d(const QString &log_str2,const QString &log_str3)
{
    if(!aLogOpen)
        return;

    QDateTime time = QDateTime::currentDateTime();

    QString currTime = time.toString("hh:mm");
    //формат лога
    QString log_result2 = "URL:" + astrUrl + " ; " + "delay:" + log_str2 + " ; " + "meandelay_10:" + log_str3 + " ; "+ currTime + " ; ";

    QDir *TEST = new QDir;//папка_лога

    bool exist = TEST->exists("delaylog");
    if(!exist)
        bool ok = TEST->mkdir("delaylog");
    QString fileName2 = "delaylog/" + aTitle +"_" + time.toString("yyyy-MM-dd") + ".txt";

    aLogFileName2 = fileName2;
    QString str2 = log_result2;
    QFile file2(fileName2);

    QFileInfo fileinfo(fileName2);

    if (fileinfo.size() > 1024*1024)
    {
        QTime currTime = QTime::currentTime();
        int currTimeStamp = currTime.hour()*3600 + currTime.minute()*60;

        QString newfileName2 = "log/" + aTitle +"_" + time.toString("yyyy-MM-dd")+
                "_"+QString::number(currTimeStamp) + ".txt";
        file2.rename(newfileName2);
    }
    if(!file2.open(QIODevice::WriteOnly  | QIODevice::Text|QIODevice::Append))
    {
        qDebug()<<"can't open";
    }
    QTextStream in(&file2);
    in<<str2<<"\n";
    file2.close();
}
////////////////////////////////////
QString httpstress::getLogFileName()
{
    return aLogFileName;
}

QString httpstress::getLogFileName2()
{
    return aLogFileName2;
}
QString httpstress::getRequestTimes()
{
    return QString::number(aRequestTimes);//колво_запросов
}
QString httpstress::getdelay()
{
    return QString::number(delay1);//
}
QString httpstress::getmean()
{
    return QString::number(mean3);//
}
QString httpstress::getreqdata()
{
    return strResult;//
}
QString httpstress::getRequestUrl()
{
    return astrUrl;
}
QString httpstress::getRequestTitle()
{
    return aTitle;
}
int httpstress::getIntervalTime()
{
    return aIntervalTime/1000;
}
int httpstress::getTimeOut()
{
    return aOutTime/1000;
}
void httpstress::setLogSwitch(const bool &on_off)
{
    aLogOpen = on_off;
}
QString httpstress::getpos_text()/////////////////
{
    return apos_text;
}
QString httpstress::getype_info2()
{
    return atype_info2;
}
////////////////////////////////////////////////////////////////
void httpstress::slot_requestFinished()
//Конец запроса
{
    qDebug()<< "slot_requestFinished(";
    tt = (t.elapsed())/1000.0;
    tt2 = aOutTime/1000.0;
    qDebug() << tt << "s";
    qDebug() <<  tt2<< "s";
    qDebug()<< err3 ;

   if (err3 !=0 && tt2>tt)
   {
    delay1 = tt;
    err2 = 0;
    err3 = 0;
    tt = (t.elapsed())/1000.0;
    tt2 = aOutTime;
    mean1 = mean1 + 1;
    mean2 = mean2 + tt;
    mean3 = mean2/mean1;
    //результат на get post

    int nHttpCode = aNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();//http код возврата
    QList<QByteArray> listHeaders;
    listHeaders = aNetworkReply->rawHeaderList();
    QByteArray header;

    QByteArray resultContent = aNetworkReply->readAll();//
    QTextCodec* pCodec = QTextCodec::codecForName("UTF-8");

    strResult = aNetworkReply->attribute(QNetworkRequest::HttpReasonPhraseAttribute).toString() + "\n";
    strResult += aNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() + "\n";
    foreach(header, listHeaders) {
             strResult += header + "_:_" + aNetworkReply->rawHeader(header) +"\n";
             qDebug("%s: %s", header.data(), aNetworkReply->rawHeader(header).data());
          }
    strResult += aNetworkReply->errorString().toInt() + "\n\n\n\n\n\n";

    strResult += pCodec->toUnicode(resultContent);//ответ
    qDebug() << (strResult) ;
    qDebug("%s: %s", header.data(), aNetworkReply->rawHeader("Set-Cookie").data());
    cook = aNetworkReply->rawHeader("Set-Cookie");

    if(nHttpCode == 200)//Успешно

    {
        log("Zapro_ok_http_200");
         log2d(QString::number(delay1),QString::number(mean3));
        //Успешный запрос
        emit signal_requestFinished(true,strResult);//bSuccess
    }
    else
    {
        //Запрос не выполнен
        log("http_get_false_not200");
        //strResult=aNetworkReply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        log2d(QString::number(delay1),QString::number(mean3));
            err = err+1;
             qDebug()<< "timer_or_error";
        emit signal_requestFinished(false,strResult);//bSuccess
    }

    if (aIntervalTime>=aOutTime)
    {
     //slot_requestTimeout();
     qDebug("Time elapsed: %d s", aOutTime);
     qDebug("Time elapsed: %d s", aIntervalTime);
     tt3 = t.elapsed();
     aOutTimer->stop();//сброс
     timer->stop();
     //err3 = 0;
         }
   aNetworkReply->deleteLater();
     }
        }
void httpstress::slot_requestTimeout()
{
       tt3 = t.elapsed();
       aOutTimer->stop();//сброс
       timer->stop();
    log("signal_timeout");
    emit signal_requestFinished(false,"timeout");//bSuccess
    err3 = 0;

    disconnect(timer,SIGNAL(timeout()),this,SLOT(slot_requestTimeout()));
    (aNetworkReply)->abort();
    (aNetworkReply)->blockSignals(true);
    aNetworkReply->deleteLater();
}
////////////////////////////////////////////////////////////////
//2
void httpstress::slot_intervalTimeRequest()//обработка временного интервала
{                                          //если принудительно
    qDebug()<< "slot_intervalTimeRequest(";
        QNetworkRequest netRequest;
        //инфо о данных
        netRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
        //netRequest.setHeader(QNetworkRequest::ContentTypeHeader,"text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
        netRequest.setUrl(QUrl(astrUrl)); //Информация об адресе
        netRequest.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::UserVerifiedRedirectPolicy);
        netRequest.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
        netRequest.setAttribute(QNetworkRequest::HttpPipeliningAllowedAttribute, true);
        netRequest.setAttribute(QNetworkRequest::RedirectPolicyAttribute, QNetworkRequest::NoLessSafeRedirectPolicy);

        if(astrUrl.toLower().startsWith("https"))
        {
            QSslConfiguration sslConfig;
              sslConfig.setPeerVerifyMode(QSslSocket::VerifyNone);
              sslConfig.setProtocol(QSsl::TlsV1_2);
              sslConfig.setProtocol(QSsl::AnyProtocol);
        }
        t.start();
////////////////////////////////
        QNetworkCookieJar *cookieJar = new QNetworkCookieJar(aNetworkManager);
        aNetworkManager->setCookieJar(cookieJar);
        netRequest.setRawHeader(QByteArray("Cookie"),  QByteArray(cook));
        QByteArray reqdata;
        reqdata.append(apos_text);
        // // // // //// // // //
        if (atype_info2 == "get")
        {
            aNetworkReply = aNetworkManager->get(netRequest);
        }
        if (atype_info2 == "post")
        {
            aNetworkReply = aNetworkManager->post(netRequest,reqdata);
        }
/////////////////////////////////
        timer->start (aOutTime);
        err3 = 1;
        disconnect(timer,SIGNAL(timeout()),this,SLOT(slot_requestTimeout()));  //незабыть перенести в начало
        (aNetworkReply)->blockSignals(false);
         connect(aNetworkReply,SIGNAL(finished()),this,SLOT(slot_requestFinished()));//

        aOutTimer->start(aOutTime);
        aRequestTimes += 1;
}
