#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    changeEnabled();

    //socket_.readyRead();
    QObject::connect(&socket_, &QAbstractSocket::connected, this, &Widget::doConnected);
    QObject::connect(&socket_, &QAbstractSocket::disconnected, this, &Widget::doDisconnected);
    QObject::connect(&socket_, &QAbstractSocket::readyRead, this, &Widget::doReadyRead);
}

void Widget::changeEnabled(){
    ui->pbConnect->setEnabled(socket_.state()==QAbstractSocket::UnconnectedState);
    ui->cbSsl->setEnabled(socket_.state()==QAbstractSocket::UnconnectedState);
    ui->lPort->setEnabled(socket_.state()==QAbstractSocket::UnconnectedState);
    ui->pbDisconnect->setEnabled(socket_.state()==QAbstractSocket::ConnectedState);
    ui->pbSend->setEnabled(socket_.state()==QAbstractSocket::ConnectedState);
}

void Widget::doConnected(){
    ui->pteMessage->insertPlainText("Connected \r\n");
    changeEnabled();
}

void Widget::doDisconnected(){
    ui->pteMessage->insertPlainText("Disconnected \r\n");
    changeEnabled();
}

void Widget::doReadyRead(){
    ui->pteMessage->insertPlainText(socket_.readAll());
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pbConnect_clicked()
{
    if(ui->cbSsl->isChecked()){
        socket_.connectToHostEncrypted(ui->leHost->text(),ui->lPort->text().toUShort());
    }
    else {
        socket_.connectToHost(ui->leHost->text(),ui->lPort->text().toUShort());
    }

}

void Widget::on_pbDisconnect_clicked()
{
    socket_.close();
}


void Widget::on_pbSend_clicked()
{
    socket_.write(ui->pteSend->toPlainText().toUtf8());
}


void Widget::on_pbClear_clicked()
{
    ui->pteMessage->clear();
}


void Widget::on_cbSsl_stateChanged(int arg1)
{
    if(ui->cbSsl->isChecked()){
        ui->lPort->setText("443");
    }
    else{
        ui->lPort->setText("80");
    }
}

