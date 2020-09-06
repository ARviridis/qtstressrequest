#include "widget.h"
#include <QApplication>

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);//удаляет помощь
    widget w;
    w.setBackgroundRole(QPalette::Shadow);
    w.setStyleSheet(
                "QWidget {border: 0.1px solid DarkCyan ;text-align: center;"
                "color:rgba(255,255,250,255);"
                "border-radius: 2px;"
                "border-width: 2px;"
                "border-image: 9,2,5,2; "
                "background-color: qlineargradient(spread:pad, x1:0, y1:0, x2:1, y2:1, stop:0 rgba(70, 70, 70, 255), stop:1 rgba(10, 0, 0, 200));"
                "}"
                "QHeaderView::section:horizontal {color: #fff;border-style: solid;background-color: qlineargradient( x1: 0, y1: 0, x2: 0, y2: 1,stop: 0 #20B2AA, stop: 1 #356ccc);}"

                "QTableView {border: 2px solid #3873d9;border-top-color: #808000;border-radius: 4px;background-color: #696969;gridline-color: #777;selection-background-color: #808000;color:#000000;font-size:12px;}"
    );
    w.show();
    return a.exec();
}
