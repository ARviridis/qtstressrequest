#ifndef ewidjet_H
#define ewidjet_H

#include <QWidget>
#include "httpstress.h"
#include "widget.h"

namespace Ui
    {
class ewidjet;
    }
class widget;
class ewidjet : public QWidget
    {
    Q_OBJECT
//------------------------------------------------
private:
    Ui::ewidjet *ui;                      //указатели
    httpstress* zadh;
    widget * zadw ;

private slots:                             //слоты на клики
    void on_sureBtn_clicked();

    void on_cancleBtn_clicked();
//------------------------------------------------
public:
    explicit ewidjet(QWidget *parent = 0); //нулевой указатель родителя
    ewidjet(widget  *widget, httpstress *http);
    ~ewidjet();
    void showHttpValue();

    };
#endif                                     // ewidjet_H
