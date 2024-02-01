#include "mainwindow.h"
#include "./ui_mainwindow.h"

struct Association
{
    std::string a, b;
};

std::vector<Association> getAssociations(std::string str)
{
    std::vector<Association> res;
    std::vector<std::string> rows = split(str, '\n'), temp_v;

    for(std::string& i : rows)
    {
        temp_v = split(i, ' ');
        for(size_t j = 0; j < temp_v.size(); j++)
            if(temp_v[j].size() == 0)
                temp_v.erase(temp_v.begin() + j--);

        if(temp_v.size() != 2) continue;
        res.push_back({temp_v[0], temp_v[1]});
    }
    return res;
}

std::string readFile(std::wstring path)
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
    ui->routerBox->setText("r1 s1\nr1 s2\nr3 s3");
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_loadFromFileBtn_clicked()
{
    loadFromFile(QFileDialog::getOpenFileName(this, "", QDir::currentPath() + "/saves"));
}

void MainWindow::loadFromFile(QString path)
{
    std::string text = readFile(path.toStdWString());
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
        msgBox.setText("Ip not valid");
        msgBox.exec();
        return;
    }
    if(ip.getMask().size() == 0 && !ip.loadMask(ui->maskInput->text().toStdString()))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("title");
        msgBox.setText("Mask not valid");
        msgBox.exec();
        return;
    }

    std::vector<Subnet> subnets;

    //subnet textbox
    std::vector<Association> associations = getAssociations(ui->subnetBox->toPlainText().toStdString());
    bool check;
    for(const Association& i : associations)
    {
        //checking if already exist
        check = true;
        for(size_t j = 0; j < subnets.size(); j++)
        {
            if(subnets[j].name == i.a)
            {
                check = false;
                break;
            }
        }
        if(!check) continue;
        //adding the subnet
        subnets.push_back({
                i.a,
                toInt(i.b),
                {}
        });
    }
    //got the subnets names and host numbers

    //roters textbox
    associations = getAssociations(ui->routerBox->toPlainText().toStdString());
    for(const Association& i : associations)
    {
        for(size_t j = 0; j < subnets.size(); j++)
        {
            if(subnets[j].name == i.b)
            {
                check = true;
                for(size_t k = 0; k < subnets[j].gateways.size(); k++)
                {
                    //checking if router is already linked
                    if(subnets[j].gateways[k] == i.a)
                    {
                        check = false;
                        break;
                    }
                }
                //adding router to the list
                if(check)subnets[j].gateways.push_back(i.a);
                break;
            }
        }
    }
    //got the links between subnets and routers

    //subnet
    std::vector<Network> subnet_res = ip.subnet(subnets, ui->checkBox->isChecked());
    if(subnets.size() != subnet_res.size())
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("title");
        msgBox.setText(QString::fromStdString("Error!\nPossible causes: ip, subnets, router"));
        msgBox.exec();
        return;
    }
    //got the results

    //link text box
    subnets.clear();
    associations = getAssociations(ui->linkBox->toPlainText().toStdString());
    for(const Association& i : associations)
    {
        check = true;
        for(size_t j = 0; j < subnets.size(); j++)
        {
            if(subnets[j].name == i.a || subnets[j].name == i.b)
            {
                std::string to_find = (i.a == subnets[j].name ? i.b : i.a);
                bool check1 = true;
                check = false;
                for(size_t k = 0; k < subnets[j].gateways.size(); k++)
                {
                    if(to_find == subnets[j].gateways[k])
                    {
                        check1 = false;
                        break;
                    }
                }
                if(check1) subnets[j].gateways.push_back(to_find);
                break;
            }
        }
        if(check) subnets.push_back({i.a, 1, {i.b}});
    }
    //got the links between routers

    if(ui->routerIpInput->text().isEmpty())
    {
        Ip ip_l;

        if(!ip_l.loadIp(ui->routerIpInput->text().toStdString()))
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("title");
            msgBox.setText("Ip not valid");
            msgBox.exec();
            return;
        }

    }

    std::string temp_res, temp;
    for(size_t i = 0; i < subnet_res.size(); i++)
    {
        temp = "/" + toString(subnet_res[i].cdir);

        temp_res += subnet_res[i].name + "\n";

        temp_res += "network-address: ";
        temp_res += subnet_res[i].networkAddress + temp + "\n";

        temp_res += "broadcast-address: ";
        temp_res += subnet_res[i].broadcastAddress + temp + "\n";

        temp_res += "gateways: ";
        for(size_t j = 0; j < subnet_res[i].gatewayAdresses.size(); j++)
        {
            temp_res += "  " + subnet_res[i].gatewayNames[j] + ": ";
            temp_res += subnet_res[i].gatewayAdresses[j] + temp + "\n";;
        }

        temp_res += "\n";
    }
    ui->resBox->setText(QString::fromStdString(temp_res));
}

//192.168.1.0/24

/*
    QMessageBox msgBox;
    msgBox.setWindowTitle("title");
    msgBox.setText(QString::fromStdString());
    msgBox.exec();
*/
