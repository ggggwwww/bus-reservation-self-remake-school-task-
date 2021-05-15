#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "DB.h"
#
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->textEdit->setVisible(false);
    QSqlDatabase DB;
    if(!DB.open()){
        db::CreateDB();
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::on_pushButton_clicked()
{
    QSqlDatabase db;
    db=QSqlDatabase::addDatabase("sans");
    db.setDatabaseName("C:\DeleteMe\Bus.db");

    QString numSel = ui->lineEdit->text();
    QTextEdit *board = ui->textEdit;

    board->setVisible(true);
}
