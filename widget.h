#ifndef widget_H
#define widget_H

#include <QDialog>
#include <QQueue>
#include <QDateTime>
#include <QLabel>
#include <QTimer>
#include <QScrollArea>
#include <QGridLayout>
#include <QWidget>
#include <QProgressBar>
#include <QFileDialog>
#include <QLineEdit>

#include "downloader.h"
#include "httpstress.h"
#include "Ewidjet.h"

namespace Ui
{
class widget;
}
const QString data_file_name = "data/request_data.txt";
const QString data_file_name2 = "data/delay_data.txt";

class widget : public QDialog                //widget/гл
{
    Q_OBJECT

public:
    explicit widget(QWidget *parent = 0);
    ~widget();
    void updateRequestInfo();               //refresh//
    int flag1;
    int flag2;
private:
                                            //Добавить во все списки
    void addItemTowidget(const QString &strUrl,const QString &title,const int &outTime,
                         const int &intervalTime,const QString & type_info2);//,const QString& start,const QString& end
    void deleteItem  (const int &row);      //dell req
    void showLog     (const int &row);      //seejurnal

    void showLog2     (const int &row);      //seelogdelay

    void startRequest(int row,int col);     //pleaseStart
    void editRequest (int row,int col);     //editreq
    void stopRequest (int row,int col);     //stopreq
    void delRequest  (int row,int col);     //delreq
    void loadData();                        //loadData

protected:

    void closeEvent(QCloseEvent *e);

private slots:
    void slot_requestBtnClicked();          //клик на кнопку старт гет
    void slot_reqbtnpost();          //клик на кнопку старт пост
    void slot_reqbtnfile();          //клик на кнопку старт file

    void slot_requestFinished(bool bSuccess, const QString& strResult);//httpendreq

    //void slot_requvesterr(const QString& strResult);//httpendreq

    void slot_clickItem(const int& row,const int& col);//clickitem
    void slot_flushWidget();                //timerefreshupdate
    void on_allStopPbtn_clicked();          //stopall
    void on_allStartPbtn_clicked();         //atartall
    void on_savepbtn_clicked();

    void on_saveplog2();


    void on_checkBox_clicked(bool checked);

    //down
    // Слот для старта загрузки
    void onDownloadButtonClicked();

    // Слот для выбора каталога для скачивания
    void onSelectTargetFolderButtonClicked();

    // Слот для отмены загрузки
    void onCancelButtonClicked();

    // Слот для обновления прогресса загрузки
    void onUpdateProgress(qint64 bytesReceived, qint64 bytesTotal);

private:
    Ui::widget *ui;
    Downloader m_downloader;//пересылку
    QQueue<Downloader*> dowl;

    QQueue<httpstress*> https;
    QLabel *logLabel;
    QWidget *wgt;
    QWidget *wgt2;
    QScrollArea *scrollArea;
    QGridLayout* gridLayout;

    QTimer *aftimer;

    QLineEdit *targetFolderLineEdit;
    QLineEdit *urlLineEdit;
    QPushButton *selectTargetFolderPushButton;
    QPushButton *downloadPushButton;
    QProgressBar  *downloadProgressBar;
    QPushButton *cancelPushButton;
};
#endif                                      // widget_H
