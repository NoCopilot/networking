#include "mainwindow.h"
#include "./ui_mainwindow.h"

void readTextBox(std::string str, std::vector<Subnet>& subnets, bool mode)
{
    int temp_n, temp_n1;
    std::string temp;
    Subnet subnet;

    for(size_t i = 0; i < str.size(); i++)
    {
        //getting the line
        temp_n = str.find(10, i);
        if(temp_n == -1) temp_n = str.size();
        temp = str.substr(i, temp_n - i);

        //getting name
        temp_n1 = temp.find(' ');
        if(temp_n1 == -1 || temp_n1 == 0)
        {
            i = temp_n + 1;
            continue;
        }

        //setting name and host numbers then add to the list
        if(!mode)
        {
            subnet.name = temp.substr(0, temp_n1);
            subnet.hosts = toInt(temp.substr(temp_n1+1));
            subnets.push_back(subnet);
        }
        else
        {
            subnet.name = temp.substr(temp_n1+1);
            temp = temp.substr(0, temp_n1);
            for(Subnet& k : subnets)
            {
                if(k.name == subnet.name)
                {
                    k.gateways.push_back(temp);
                    break;
                }
            }
        }

        //ready for next line
        i = temp_n;
    }
}

std::string fileRead(std::wstring path)
{
    std::ifstream file(std::filesystem::path(path), std::ios::binary);
    if(!file.is_open()) return "";

    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    return content;
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->ipInput->setText("1.1.1.1/24");
    ui->subnetBox->setText("s1 10\ns2 20\ns3 30");
    ui->routerBox->setText("r1 s1\nr1 s2\nr3 s3\n");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadFromFileBtn_clicked()
{
    loadFromFile(QFileDialog::getOpenFileName());
}

void MainWindow::loadFromFile(QString path)
{
    std::string text = fileRead(path.toStdWString());
}

void MainWindow::on_closeWindowBtn_clicked()
{
    this->close();
}


void MainWindow::on_calculateBtn_clicked()
{
    Ip ip;
    if(!ip.loadIp(ui->ipInput->text().toStdString()))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("title");
        msgBox.setText("ip not valid");
        msgBox.exec();
        return;
    }
    if(ip.getMask().size() == 0 && !ip.loadMask(ui->maskInput->text().toStdString()))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("title");
        msgBox.setText("mask not valid");
        msgBox.exec();
        return;
    }

    std::vector<Subnet> subnets;

    std::vector<std::string> host_names, router_names;
    std::vector<int> host_n;
    std::vector<std::vector<std::string>> subnet_links, router_links;

    //first textbox
    readTextBox(ui->subnetBox->toPlainText().toStdString(), subnets, 0);
    //eliminate name repetition
    for(size_t i = subnets.size()-1; i >= 1; i--)
    {
        for(size_t j = 0; j < i; j++)
        {
            if(subnets[i].name == subnets[j].name)
            {
                subnets.erase(subnets.begin() + i);
                break;
            }
        }
    }

    readTextBox(ui->routerBox->toPlainText().toStdString(), subnets, 1);
    for(size_t i = 0; i < subnets.size(); i++)
    {
        for(size_t j = subnets[i].gateways.size(); j >= 1; j--)
        {
            for(size_t k = 0; k < j; k++)
            {
                if(subnets[i].gateways[j] == subnets[i].gateways[k])
                {
                    subnets[i].gateways.erase(subnets[i].gateways.begin() + j);
                    break;
                }
            }
        }
    }
    for(Subnet& i : subnets)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("title");
        msgBox.setText(QString::fromStdString(toString(i.gateways.size())));
        msgBox.exec();
    }

    if(!ui->routerIpInput->text().isEmpty())
    {

    }
}

//192.168.1.0/24
