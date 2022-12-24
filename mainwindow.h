#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPlainTextEdit>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionabout_triggered();

    void on_actionfind_triggered();

    void on_actionreplace_triggered();

    void on_actionnew_triggered();

    void on_actionopen_triggered();

    void on_actionsave_triggered();

    void on_actionsaveas_triggered();

    void on_textEdit_textChanged();

    void on_actioncut_triggered();

    void on_actioncopy_triggered();

    void on_actionpaste_triggered();

    //void on_actionrecovery_triggered();

    void on_textEdit_redoAvailable(bool b);

    void on_textEdit_undoAvailable(bool b);

    void on_textEdit_copyAvailable(bool b);

    void on_actionredo_triggered();

    void on_actionundo_triggered();

    void on_actionfontcolor_triggered();

    void on_actionbackgroundcolor_triggered();

    void on_actionfontbackgroundcolor_triggered();

    void on_actionfont_triggered();

    void on_actionLineWrap_triggered();

    void on_actionShowToolBar_triggered();

    void on_actionShowStatusBar_triggered();

    void on_actionSelectAll_triggered();

    void on_actionExit_triggered();

private:
    Ui::MainWindow *ui;

    QLabel statusCursorLabel;
    QLabel statusLabel;

    QString filePath;
    bool textChanged;

    bool userEditConfirmed();
};

#endif // MAINWINDOW_H
