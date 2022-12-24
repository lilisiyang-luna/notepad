#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include "searchdialog.h"
#include "replacedialog.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QColorDialog>
#include <QFontDialog>
#include <QPlainTextEdit>
#include <qtextedit.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->statusBar;
    textChanged = false;
    on_actionnew_triggered();

    statusLabel.setMaximumWidth(150);
    statusLabel.setText("Length: " + QString::number(0) + "  lines: " + QString::number(1));
    ui->statusBar->addPermanentWidget(&statusLabel);

    statusCursorLabel.setMaximumWidth(150);
    statusCursorLabel.setText("Ln: " + QString::number(0) + "  Col: " + QString::number(1));
    ui->statusBar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel(ui->statusBar);
    author->setText("袁智霖");
    ui->statusBar->addPermanentWidget(author);

    ui->actionundo->setEnabled(false);
    ui->actionredo->setEnabled(false);
    ui->actioncopy->setEnabled(false);
    ui->actioncut->setEnabled(false);
    ui->actionpaste->setEnabled(false);

        QPlainTextEdit::LineWrapMode mode = ui->textEdit->lineWrapMode();

        if (mode ==QTextEdit::NoWrap){
            ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
            ui->actionLineWrap->setChecked(false);
       }
        else{
            ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
            ui->actionLineWrap->setChecked(true);
        }
     ui->actionShowStatusBar->setChecked(true);
     ui->actionShowToolBar->setChecked(true);
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionabout_triggered()
{
    AboutDialog dlg;
    dlg.exec();
}

void MainWindow::on_actionfind_triggered()
{
    SearchDialog dlg(this,ui->textEdit);
    dlg.exec();
}

void MainWindow::on_actionreplace_triggered()
{
    ReplaceDialog dlg(this,ui->textEdit);
    dlg.exec();
}

void MainWindow::on_actionnew_triggered()
{
    if (!userEditConfirmed())
        return;
    filePath ="";
    ui->textEdit->clear();
    this->setWindowTitle(tr("新建文本文件-编辑器"));

    textChanged = false;
}

void MainWindow::on_actionopen_triggered()
{
    if (!userEditConfirmed())
        return;

    QString filename =QFileDialog::getOpenFileName(this,"打开文件",".",tr("Text files(*.txt);;All (*.*)"));
    QFile file (filename);
    if (!file.open(QFile::ReadOnly|QFile ::Text)){
        QMessageBox::warning(this,"..","打开文件失败");
        return;
    }

    filePath=filename;
    QTextStream in(&file);
    QString text =in.readAll();
    ui->textEdit->insertPlainText(text);
    file.close();

    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());

    textChanged = false;
}

void MainWindow::on_actionsave_triggered()
{
    if (filePath == ""){
        QString filename =QFileDialog::getSaveFileName(this,"保存文件",".",tr("Text files(*.txt)"));

        QFile file(filename);
        if (!file.open(QFile::WriteOnly|QFile ::Text)){
           QMessageBox::warning(this,"..","打开保存文件失败");
           return;
        }
        file.close();
        filePath=filename;
    }

    QFile file(filePath);

    if (!file.open(QFile::WriteOnly|QFile ::Text)){
       QMessageBox::warning(this,"..","打开保存文件失败");
       return;
   }

    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();

    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
    textChanged=false;
}

void MainWindow::on_actionsaveas_triggered()
{
    QString filename =QFileDialog::getSaveFileName(this,"保存文件",".",tr("Text files(*.txt)"));

    QFile file(filename);
    if (!file.open(QFile::WriteOnly|QFile ::Text)){
       QMessageBox::warning(this,"..","打开保存文件失败");
       return;
    }
    filePath=filename;
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();

    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
}

void MainWindow::on_textEdit_textChanged()
{
    if(!textChanged){
        this->setWindowTitle("*" + this->windowTitle());
        textChanged =true;

    }
}

bool MainWindow::userEditConfirmed()
{
    QString path =(filePath !="") ? filePath:"无标题.txt";
    if (textChanged){
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle("...");

        msg.setText(QString("是否将gen更改保存到\n") + "\"" + path + "\" ?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No |QMessageBox::Cancel);

        int r =msg.exec();
        switch (r){
        case QMessageBox::Yes:
            on_actionsave_triggered();
            break;
         case QMessageBox::No:
            textChanged=false;
            break;
         case QMessageBox::Cancel:
            return false;
        }
    }
    return true;
}



void MainWindow::on_actioncut_triggered()
{
    ui->textEdit->cut();
    ui->actionpaste->setEnabled(true);
}

void MainWindow::on_actioncopy_triggered()
{
    ui->textEdit->copy();
    ui->actionpaste->setEnabled(true);

}

void MainWindow::on_actionpaste_triggered()
{
    ui->textEdit->paste();
}

void MainWindow::on_actionundo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_textEdit_redoAvailable(bool b)
{
    ui->actionredo->setEnabled(b);
}

void MainWindow::on_textEdit_undoAvailable(bool b)
{
    ui->actionundo->setEnabled(b);

}

void MainWindow::on_textEdit_copyAvailable(bool b)
{
    ui->actioncopy->setEnabled(b);
    ui->actioncut->setEnabled(b);

}

void MainWindow::on_actionredo_triggered()
{
    ui->textEdit->redo();
}



void MainWindow::on_actionfontcolor_triggered()
{
    QColor color =QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit {color: %1}").arg(color.name()));
    }
}

void MainWindow::on_actionbackgroundcolor_triggered()
{
    QColor color =QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit {background-color: %1}").arg(color.name()));
    }
}


void MainWindow::on_actionfontbackgroundcolor_triggered()
{
    QColor color =QColorDialog::getColor(Qt::black,this,"选择颜色");
    if(color.isValid()){
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit {fontbackground-color: %1}").arg(color.name()));
    }
}

void MainWindow::on_actionfont_triggered()
{
    bool ok = false;
    QFont font =QFontDialog::getFont(&ok,this);
    if (ok)
        ui->textEdit->setFont(font);
}

void MainWindow::on_actionLineWrap_triggered()
{
    QPlainTextEdit::LineWrapMode mode = ui->textEdit->lineWrapMode();

    if (mode ==QPlainTextEdit::NoWrap){
        ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->actionLineWrap->setChecked(true);
    }
    else{
        ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionLineWrap->setChecked(false);
    }
}

void MainWindow::on_actionShowToolBar_triggered()
{
    bool visible =ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->actionShowToolBar->setChecked(!visible);
}

void MainWindow::on_actionShowStatusBar_triggered()
{
    bool visible =ui->statusBar->isVisible();
    ui->statusBar->setVisible(!visible);
    ui->actionShowStatusBar->setChecked(!visible);
}

void MainWindow::on_actionSelectAll_triggered()
{
    ui->textEdit->selectAll();
}

void MainWindow::on_actionExit_triggered()
{
    if (userEditConfirmed())
        exit(0);
}
