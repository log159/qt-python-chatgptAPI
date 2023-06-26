#include "pushandreceivewidget.h"
#include "ui_pushandreceivewidget.h"

PushAndReceiveWidget::PushAndReceiveWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PushAndReceiveWidget)
{
    ui->setupUi(this);
    this->setFixedSize(_Width,_Height);
    initItemsStyle();
}

PushAndReceiveWidget::~PushAndReceiveWidget()
{
    delete ui;
}

void PushAndReceiveWidget::initItemsStyle()
{
    ui->textEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QPixmap sendbutton_img;
    sendbutton_img.load(":/res/PushImg.png");

    sendbutton_img = sendbutton_img.scaled(ui->pushButton_send->width(),ui->pushButton_send->height(), Qt::KeepAspectRatio);

    ui->pushButton_send->setIcon(QIcon(sendbutton_img));
    ui->pushButton_send->setIconSize(sendbutton_img.size()*0.7);
    ui->pushButton_send->setFixedSize(sendbutton_img.size()*0.7);
    ui->pushButton_send->setStyleSheet("background-color: transparent;");
    ui->pushButton_send->setCursor(Qt::PointingHandCursor);

    ui->listWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);// 禁止编辑
    ui->listWidget->setSelectionMode(QAbstractItemView::NoSelection); //禁止选中
    ui->listWidget->setFocusPolicy(Qt::NoFocus); //禁止获取焦点
    ui->listWidget->setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);//平滑效果
    //样式
    ui->listWidget->setStyleSheet(
                "QScrollBar:vertical {"
                "width: 10px; "
                "background-color: #F5F5F5; "
                "margin: 0px 0px 0px 0px;"
                "border-radius: 5px;"
                "} "
                "QScrollBar::handle:vertical {"
                "background-color: #CCCCCC; "
                "min-height: 20px;"
                 "border-radius: 5px;"
                "} "
                "QScrollBar::handle:vertical:hover {"
                "background-color: #BBBBBB;"
                "border-radius: 5px;"
                "} "
                "QScrollBar::add-line:vertical {"
                "height: 0px; "
                "subcontrol-position: bottom; "
                "subcontrol-origin: margin;"
                "} "
                "QScrollBar::sub-line:vertical "
                "{height: 0 px; "
                "subcontrol-position: top; "
                "subcontrol-origin: margin;"
                "} "
                "QScrollBar::add-page:vertical, QScrollBar::sub-page:vertical {"
                "background-color: #F5F5F5;"
                "border-radius: 5px;"
                "}");

    ui->textEdit->setFocus();


}

void PushAndReceiveWidget::handle_user_information()
{
    // listWidget添加user列表项
    QListWidgetItem *item = new QListWidgetItem();
    ListItemsWidget * widget=new UserItemsWidget;
    int lineCount=ui->textEdit->toPlainText().size()/15+1;
    widget->initItem(ui->textEdit->toPlainText(),ItemEnum::User,lineCount);
    ui->listWidget->addItem(item);
    item->setSizeHint(QSize(widget->width(),widget->height()+20));
    ui->listWidget->setItemWidget(item,widget);
    ui->listWidget->scrollToBottom();
}

void PushAndReceiveWidget::handle_bot_information()
{

    // 创建QProcess对象
    QProcess * process=new QProcess;

    QObject::connect(process,&QProcess::readyReadStandardOutput, [=]() {
        this->information_coming=false;
        QByteArray data = process->readAllStandardOutput();
        QString receivedData = QString::fromUtf8(data);
//            qDebug() << "接收内容为:" << receivedData;
        qDebug()<<"内容成功接收";

        // listWidget添加bot列表项
        QListWidgetItem *item = new QListWidgetItem();
        ListItemsWidget * widget=new BotItemsWidget;
        int lineCount=receivedData.size()/15+1;
        widget->initItem(receivedData,ItemEnum::Bot,lineCount);
        ui->listWidget->addItem(item);
        item->setSizeHint(QSize(widget->width(),widget->height()+20));
        ui->listWidget->setItemWidget(item,widget);

        ui->listWidget->scrollToBottom(); //将滚动条设置为最底部

    });

    FinishedFunc finished = &QProcess::finished;
    connect(process,finished,[=](){
        process->deleteLater();
        delete process;
        qDebug()<<"------------资源释放-----------";
    });
    QStringList arguments;
    arguments << pythonScript;
    qDebug()<<"------------发送请求-----------";
    process->start(pythonInterpreter, arguments);
    QString response = ui->textEdit->toPlainText();
    ui->textEdit->clear();
    ui->textEdit->setFocus();
    response.replace("\n", "");
    qDebug()<<"发送内容为："<<response;
    QByteArray responseData = response.toUtf8();
    process->write(responseData);
    process->closeWriteChannel();
}
void PushAndReceiveWidget::on_pushButton_send_clicked()
#define DEBUG
#ifndef DEBUG
{
    // listWidget添加user列表项
    {
        QListWidgetItem *item = new QListWidgetItem();
        ListItemsWidget * widget=new UserItemsWidget;
        int lineCount=ui->textEdit->toPlainText().size()/15+1;
        widget->initItem(ui->textEdit->toPlainText(),ItemEnum::User,lineCount);
        ui->listWidget->addItem(item);
        item->setSizeHint(QSize(widget->width(),widget->height()+20));
        ui->listWidget->setItemWidget(item,widget);

        ui->listWidget->scrollToBottom(); //将滚动条设置为最底部
    }


    // listWidget添加bot列表项
    QListWidgetItem *item = new QListWidgetItem();
    ListItemsWidget * widget=new BotItemsWidget;
//    int lineCount=receivedData.size()/15+1;
    widget->initItem("Hello! Can I help you ?",ItemEnum::Bot,1);
    ui->listWidget->addItem(item);
    item->setSizeHint(QSize(widget->width(),widget->height()+20));
    ui->listWidget->setItemWidget(item,widget);

    ui->listWidget->scrollToBottom(); //将滚动条设置为最底部
}
#else
{
    if(information_coming){return;}
    information_coming=true;

    handle_user_information();

    handle_bot_information();


}
#endif
