#ifndef httpstress_H
#define httpstress_H

#include <QObject>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QTimer>
#include <QString>
#include <QByteArray>
#include <QDateTime>
#include <QFile>
#include <QTextStream>
#include <QMessageBox>

#include <QSslConfiguration>                            //class содержит конфигурацию и состояние соединения SSL

class httpstress : public QObject  //httpreqclass
{
    Q_OBJECT
public:
    explicit httpstress(QObject *parent = 0);           //нулевой указатель родителя

    void init(const QString& strUrl,const int& outTime,const int& intervalTime,
              const QString& title,const QString &type_info2,const QString &pos_text);//const QString& startTimer,const QString& endTimer,
    void sendRequest();                                 //послать HTTP-запрос_на_URL
    void closeIntervalTimer();                          //закрытие_таймера_интервала
    void startIntervalTimer();                          //старт_таймера


    void log(const QString& log_str);                   //tolog
    void log2d(const QString& log_str2,const QString& log_str3);
    //void setRequestState(const QString &state);         //Установить колво
    void setLogSwitch(const bool &on_off);              //на переключатель ведения лога


    int getIntervalTime();                              //на получение интервала запроса /////////////
    int getTimeOut();                                   //на получение тайм-аута

    QString  getLogFileName();                          //на получение имени файла журнала
    QString  getLogFileName2();                          //на получение имени файла журнала
    //QString  getRequestState();                         //на получение статуса запроса
    QString  getRequestTimes();                         //на получение запроса

    QString  getdelay();                                   //на получение запроса
    QString  getmean();                                   //на получение запроса
    QString  getpos_text();                                   //на получение запроса
    QString  getype_info2();                                   //на получение запроса

    QString  getreqdata();                                   //на получение запроса

    QString  getRequestUrl();                           //на получение URL запроса
    QString  getRequestTitle();                         //на получение названия запроса

    //QString  getStartTime();                            //на получение времени начала
    //QString  getEndTime();                              //на получение времени окончания

signals:
    void signal_requestFinished(bool bSuccess,const QString &strResult);//на конец http запроса
public slots:
    void slot_requestFinished();                        //на конец http запроса
    void slot_requestTimeout();                         //на истечение времени запроса
    void slot_intervalTimeRequest();                    //Функция обработки запроса временного интервала

private:
    QNetworkAccessManager* aNetworkManager;           //на управление сетью//
    QNetworkReply* aNetworkReply;                     //на возврат информации
   // QNetworkCookieJar *cookieJar;
    QTimer* aOutTimer;                                //на интервальный таймер

    QTimer* timer;

    QTimer* aOutTimer2;
    QTimer* aIntervalTimer;                           //Интервалы

    QTime t;
    float tt;
    float tt2;
    float tt3;

    int aIntervalTime;                                //на время ожиданиезапроса
    int aOutTime;

    int err; //счтетчик ошибок для стопа
    int err2; //счтетчик ошибок для стопа
    int err3;

    int mean1;//счетчик для среднего
    float mean2;//счет
    float mean3;//среднее

    QString astrUrl;                                   //на текущий URL-адрес запроса
    QString aTitle;                                   //на запрос названия
    QString aLogFileName;                             //на имя файла журнала
    QString aLogFileName2;                             //на имя файла журнала
    QString aRequestState;                            //Статус доступа по времени
    QString strResult;
     QByteArray cook;

    QString apos_text;
    QString atype_info2;


    int64_t aRequestTimes;                            //колво выполненных запросов(просмотров)


    float delay1;                                 //задержка

    bool aStart;                                    //Начинается ли запись
    bool aLogOpen;                                  //сохр лога
};
#endif                                                  // httpstress_H

