#ifndef PUSHANDRECEIVEWIDGET_H
#define PUSHANDRECEIVEWIDGET_H

#include <QWidget>
#include <QProcess>
#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QIcon>
#include <QPixmap>
#include <QListWidget>
#include <QListWidgetItem>


#include "listitemswidget.h"
#include "botitemswidget.h"
#include "useritemswidget.h"


namespace Ui {
class PushAndReceiveWidget;
}

class PushAndReceiveWidget : public QWidget
{
    Q_OBJECT

    typedef void (QProcess::*FinishedFunc)(int);

private:
    const int _Width    =450;
    const int _Height   =600;

    // 设置Python解释器和脚本路径
    QString pythonInterpreter = "python";
    QString pythonScript ="E:/openpy/qtopenai.py";

    bool information_coming=false;

public:
    explicit PushAndReceiveWidget(QWidget *parent = nullptr);
    ~PushAndReceiveWidget();


    void initItemsStyle();

private slots:

    void handle_user_information();

    void handle_bot_information();

    void on_pushButton_send_clicked();

private:
    Ui::PushAndReceiveWidget *ui;
};

#endif // PUSHANDRECEIVEWIDGET_H
