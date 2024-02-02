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
    std::string ip_input = ui->ipInput->text().toStdString(), mask_input = ui->maskInput->text().toStdString();
    int n = ip_input.rfind('/');
    if(n != -1)
    {
        if(n+1 >= ip_input.size())
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("title");
            msgBox.setText("Ip not valid");
            msgBox.exec();
            return;
        }
        else
        {
            mask_input = ip_input.substr(n+1);
            ip_input = ip_input.substr(0, n);
        }
    }
    Ip ip(ip_input);
    if(ip.getIp() == "")
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("title");
        msgBox.setText("Ip not valid");
        msgBox.exec();
        return;
    }

    Mask* mask = nullptr;
    if(mask_input.size() > 0 && mask_input.size() < 3)
    {
        mask = new Mask(stringToInt(mask_input));
        if(mask->getMask().size() == 0)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("title");
            msgBox.setText("Cdir not valid");
            msgBox.exec();
            return;
        }
    }
    else
    {
        mask = new Mask(mask_input);
        if(mask->getMask().size() == 0)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle("title");
            msgBox.setText("Mask not valid");
            msgBox.exec();
            return;
        }
    }
    ip.applyMask(*mask);

    std::vector<SubnetInfo> _subnets;

    //subnet textbox
    std::vector<Association> associations = getAssociations(ui->subnetBox->toPlainText().toStdString());
    bool check;
    for(const Association& i : associations)
    {
        //checking if already exist
        check = true;
        for(size_t j = 0; j < _subnets.size(); j++)
        {
            if(_subnets[j].name == i.a)
            {
                check = false;
                break;
            }
        }
        if(!check) continue;
        //adding the subnet
        _subnets.push_back({
                i.a,
                stringToInt(i.b),
                {}
        });
    }
    //got the _subnets names and host numbers

    //roters textbox
    associations = getAssociations(ui->routerBox->toPlainText().toStdString());
    for(const Association& i : associations)
    {
        for(size_t j = 0; j < _subnets.size(); j++)
        {
            if(_subnets[j].name == i.b)
            {
                check = true;
                for(size_t k = 0; k < _subnets[j].gateways.size(); k++)
                {
                    //checking if router is already linked
                    if(_subnets[j].gateways[k] == i.a)
                    {
                        check = false;
                        break;
                    }
                }
                //adding router to the list
                if(check)_subnets[j].gateways.push_back(i.a);
                break;
            }
        }
    }
    //got the links between subnets and routers

    //subnet
    subnets = subnet(ip, mask->getCdir(), _subnets, ui->checkBox->isChecked());
    if(_subnets.size() != subnets.size())
    {
        subnets.clear();

        QMessageBox msgBox;
        msgBox.setWindowTitle("title");
        msgBox.setText(QString::fromStdString("Error!\nPossible causes: ip, subnets, router"));
        msgBox.exec();
        return;
    }
    //got the results

    //link text box
    _subnets.clear();
    associations = getAssociations(ui->linkBox->toPlainText().toStdString());
    for(const Association& i : associations)
    {
        check = true;
        if(i.a == i.b) continue;
        for(size_t j = 0; j < _subnets.size(); j++)
        {
            if(_subnets[j].name == i.a || _subnets[j].name == i.b)
            {
                std::string to_find = (i.a == _subnets[j].name ? i.b : i.a);
                bool check1 = true;
                check = false;
                for(size_t k = 0; k < _subnets[j].gateways.size(); k++)
                {
                    if(to_find == _subnets[j].gateways[k])
                    {
                        check1 = false;
                        break;
                    }
                }
                if(check1) _subnets[j].gateways.push_back(to_find);
                break;
            }
        }
        if(check) _subnets.push_back({i.a, 1, {i.b}});
    }
    //got the links between routers

    //print the subnet results
    std::string temp_res, temp;
    for(size_t i = 0; i < subnets.size(); i++)
    {
        temp = "/" + intToString(subnets[i].cdir);

        temp_res += subnets[i].name + "\n";

        temp_res += "network-address: ";
        temp_res += subnets[i].networkAddress + temp + "\n";

        temp_res += "broadcast-address: ";
        temp_res += subnets[i].broadcastAddress + temp + "\n";

        temp_res += "gateways: ";
        for(size_t j = 0; j < subnets[i].gatewayAdresses.size(); j++)
        {
            temp_res += "  " + subnets[i].gatewayNames[j] + ": ";
            temp_res += subnets[i].gatewayAdresses[j] + temp + "\n";;
        }

        temp_res += "\n";
    }
    ui->resBox->setText(QString::fromStdString(temp_res + "\n"));
    //end

    //print router results
    routers = subnet(ip, mask->getCdir(), _subnets, true);
    if(routers.size() != _subnets.size())
    {
        routers.clear();

        QMessageBox msgBox;
        msgBox.setWindowTitle("title");
        msgBox.setText(QString::fromStdString("Error!\nPossible causes: router links"));
        msgBox.exec();
        return;
    }
    for(size_t i = 0; i < routers.size(); i++)
    {
        temp = "/" + intToString(routers[i].cdir);

        temp_res += routers[i].name + "\n";

        temp_res += "network-address: ";
        temp_res += routers[i].networkAddress + temp + "\n";

        temp_res += "broadcast-address: ";
        temp_res += routers[i].broadcastAddress + temp + "\n";

        temp_res += "gateways: ";
        for(size_t j = 0; j < routers[i].gatewayAdresses.size(); j++)
        {
            temp_res += "  " + routers[i].gatewayNames[j] + ": ";
            temp_res += routers[i].gatewayAdresses[j] + temp + "\n";;
        }

        temp_res += "\n";
    }
    ui->resBox->setText(QString::fromStdString(temp_res));
    //end
}

void MainWindow::on_dijkstraBtn_clicked()
{
    on_calculateBtn_clicked();
    d_from = "";
    d_to = "";

    if(routers.size() == 0)
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle("title");
        msgBox.setText(QString::fromStdString("Please set first the routers"));
        msgBox.exec();
        return;
    }
    ui->stackedWidget->setCurrentIndex(1);

    QStringList wordList;
    std::string str;
    bool check;
    for(Subnet& s : routers)
    {
        str = s.name;
        check = true;
        for(size_t i = 0; i < wordList.size(); i++)
        {
            if(wordList[i].toStdString() == str)
            {
                check = false;
                break;
            }
        }
        if(check) wordList << QString::fromStdString(str);

        for(size_t i = 0; i < s.gatewayNames.size(); i++)
        {
            check = true;
            for(size_t j = 0; j < wordList.size(); j++)
            {
                if(wordList[j].toStdString() == s.gatewayNames[i])
                {
                    check = false;
                    break;
                }
            }
            if(check) wordList << QString::fromStdString(s.gatewayNames[i]);
        }
    }

    ui->fromList->blockSignals(true);
    ui->toList->blockSignals(true);

    ui->fromList->clear();
    ui->toList->clear();

    ui->fromList->blockSignals(false);
    ui->toList->blockSignals(false);

    ui->fromList->addItems(wordList);
    ui->toList->addItems(wordList);
}


void MainWindow::on_fromList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    d_from = current->text().toStdString();
    std::cout << "changed from\n";
}


void MainWindow::on_toList_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    d_to = current->text().toStdString();
    std::cout << "changed to\n";
}


void MainWindow::on_goBackBtn_clicked()
{
    ui->stackedWidget->setCurrentIndex(0);
}

