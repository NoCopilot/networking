#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "QListWidgetItem"
#include "qfiledialog.h"
#include "qpalette.h"
#include "qmessagebox.h"
#include "qstringlist.h"

#include "Networking.hpp"
#include <fstream>
#include <iostream>

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

    void on_dijkstraBtn_clicked();

    void on_fromList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_toList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

    void on_goBackBtn_clicked();

private:
    Ui::MainWindow *ui;
    std::vector<Subnet> subnets;
    std::vector<Subnet> routers;
    std::string d_from, d_to;
};
#endif // MAINWINDOW_H
