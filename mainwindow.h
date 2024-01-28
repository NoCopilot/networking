#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qfiledialog.h"
#include "qpalette.h"
#include "qmessagebox.h"
#include "Ip.hpp"
#include <fstream>
#include <string>

std::string readFile(std::wstring);

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_loadFromFileBtn_clicked();
    void loadFromFile(QString);
    void on_closeWindowBtn_clicked();

    void on_calculateBtn_clicked();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
