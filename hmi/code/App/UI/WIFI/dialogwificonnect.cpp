#include "dialogwificonnect.h"
#include "ui_dialogwificonnect.h"
#include <QKeyEvent>
#include "dialogconnectmask.h"
#include "../../../Base/base.h"
#include "../../WIFI/wifi.h"
#include <QTimer>


DialogWIFIConnect::DialogWIFIConnect(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogWIFIConnect)
{
    ui->setupUi(this);
    ui->WIFIPwd->installEventFilter(this);
    setWIFIPwdStrLen(32);
    ui->WIFIName->setText(QString::fromStdString(WIFI_Single::instance()->getConnectingWIFI()));
    //链接wifi线程
    connectTimer = new QTimer(this);
    connectTimer->setInterval(0);
    connectTimer->setSingleShot(true);//触发一次 true
    connect(connectTimer,SIGNAL(timeout()),this,SLOT(slot_connectWifi()));
    //wifi密码校验
    mPskVerifyTimer = new QTimer(this);
    mPskVerifyTimer->setInterval(2000);
    mPskVerifyTimer->setSingleShot(false);//触发一次 false
    connect(mPskVerifyTimer,SIGNAL(timeout()),this,SLOT(slot_pskVerify()));
}



DialogWIFIConnect::~DialogWIFIConnect()
{
    delete connectTimer;
    delete ui;
}

void DialogWIFIConnect::setWIFIPwdStrLen(int vel)
{
    WIFIPwdStrLen = vel;
}

void DialogWIFIConnect::on_btn_shift_clicked()
{
    isSHIFT = !isSHIFT;
    if(isSHIFT){
        ui->btn_shift->setText("SHIFT");
    }else{
        ui->btn_shift->setText("shift");
    }
}

void DialogWIFIConnect::on_btn_space_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    WIFIPwdStr += " ";
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_Baskspace_clicked()
{
    if(WIFIPwdStr.length() == 0){
        return;
    }
    WIFIPwdStr.chop(1);
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_lang_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "~";
    }else{
        WIFIPwdStr += "`";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_1_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "!";
    }else{
        WIFIPwdStr += "1";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_2_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "@";
    }else{
        WIFIPwdStr += "2";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_3_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "#";
    }else{
        WIFIPwdStr += "3";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_4_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "$";
    }else{
        WIFIPwdStr += "4";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_5_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "%";
    }else{
        WIFIPwdStr += "5";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_6_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "^";
    }else{
        WIFIPwdStr += "6";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_7_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "&";
    }else{
        WIFIPwdStr += "7";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_8_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "*";
    }else{
        WIFIPwdStr += "8";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_9_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "(";
    }else{
        WIFIPwdStr += "9";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_0_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += ")";
    }else{
        WIFIPwdStr += "0";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_jian_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "_";
    }else{
        WIFIPwdStr += "-";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_deng_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "+";
    }else{
        WIFIPwdStr += "=";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_q_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "Q";
    }else{
        WIFIPwdStr += "q";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_w_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "W";
    }else{
        WIFIPwdStr += "w";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_e_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "E";
    }else{
        WIFIPwdStr += "e";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_r_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "R";
    }else{
        WIFIPwdStr += "r";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_t_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "T";
    }else{
        WIFIPwdStr += "t";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_y_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "Y";
    }else{
        WIFIPwdStr += "y";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_u_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "U";
    }else{
        WIFIPwdStr += "u";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_i_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "I";
    }else{
        WIFIPwdStr += "i";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_o_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "O";
    }else{
        WIFIPwdStr += "o";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_p_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "P";
    }else{
        WIFIPwdStr += "p";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_zuokuang_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "{";
    }else{
        WIFIPwdStr += "[";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_youkuang_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "}";
    }else{
        WIFIPwdStr += "]";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_fanxie_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "|";
    }else{
        WIFIPwdStr += "\\";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_a_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "A";
    }else{
        WIFIPwdStr += "a";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_s_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "S";
    }else{
        WIFIPwdStr += "s";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_d_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "D";
    }else{
        WIFIPwdStr += "d";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_f_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "F";
    }else{
        WIFIPwdStr += "f";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_g_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "G";
    }else{
        WIFIPwdStr += "g";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_h_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "H";
    }else{
        WIFIPwdStr += "h";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_j_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "J";
    }else{
        WIFIPwdStr += "j";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_k_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "K";
    }else{
        WIFIPwdStr += "k";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_l_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "L";
    }else{
        WIFIPwdStr += "l";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_fenhao_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += ":";
    }else{
        WIFIPwdStr += ";";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_danyin_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "\"";
    }else{
        WIFIPwdStr += "'";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_z_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "Z";
    }else{
        WIFIPwdStr += "z";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_x_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "X";
    }else{
        WIFIPwdStr += "x";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_c_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "C";
    }else{
        WIFIPwdStr += "c";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_v_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "V";
    }else{
        WIFIPwdStr += "v";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_b_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "B";
    }else{
        WIFIPwdStr += "b";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_n_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "N";
    }else{
        WIFIPwdStr += "n";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_m_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "M";
    }else{
        WIFIPwdStr += "m";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_douhao_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "<";
    }else{
        WIFIPwdStr += ",";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_jvhao_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += ">";
    }else{
        WIFIPwdStr += ".";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btn_xiegang_clicked()
{
    if(WIFIPwdStr.length() > WIFIPwdStrLen){
        return;
    }
    if(isSHIFT){
        WIFIPwdStr += "?";
    }else{
        WIFIPwdStr += "/";
    }
    ui->WIFIPwd->setText(WIFIPwdStr);
}

void DialogWIFIConnect::on_btnConnect_clicked()
{
    connectTimer->start();
    DialogConnectMask diaConnect(this);
    connectUi = &diaConnect;
    diaConnect.exec();
    connectUi = nullptr;
}
void DialogWIFIConnect::slot_connectWifi()
{
    std::map<int,std::string>& configWifi = WIFI_Single::instance()->getWifiConfig();
    QString command;
    //wifi已保存在conf文件中，策略：将其删除从新链接
    std::map<int,std::string>::iterator key;
    bool found = false;
    for(auto &map : configWifi){
        if(map.second == WIFI_Single::instance()->getConnectingWIFI()){
            //将存档中的旧帐号密码删除
            command = "wpa_cli -i wlan0 remove_network " + QString::number(map.first);
            system(qStringToC(command));
            DEBUG_I("%s",qStringToC(command));
            key = configWifi.find(map.first);
            found = true;
        }
    }
    //在循环中删除元素 会导致后面元素向前，使for循环崩溃
    if(found && key != configWifi.end()){
        configWifi.erase(key);
    }
    //配置文件中没有存档，则添加链接。并保存进文件。没有校验密码是否正确，以后需要处理
    connectNewAccount();
}
void DialogWIFIConnect::slot_pskVerify()
{
    DEBUG_I("slot_pskVerify ssid is %s ",mSsid.c_str());
    static int count = 0; // 设计验证三次 两秒一次
    count++;
    FILE *fp = nullptr;
    char buf[256];
    std::string data;
    bool connected = false;
    if((fp = popen("wpa_cli -i wlan0 list_networks","r")) != nullptr){
        while(fgets(buf, 256, fp) != nullptr){
            data = buf;
            if(data.find(mSsid) != std::string::npos && data.find("[CURRENT]") != std::string::npos ){
                connected = true;
                DEBUG_D("%s",data.c_str());
                break;
            }
            memset(buf,'0',256);
        }
        pclose(fp);
        fp = nullptr;
    }
    if(connected){
        count = 0;
        connectUi->setConnectStatus("链接成功");
        mySleep(1000);
        saveNowAccount();
        connectUi->close();
        mPskVerifyTimer->stop();

    }else{
        if(count > 3){
            count = 0;
            connectUi->setConnectStatus("链接失败，请检查密码");
            mPskVerifyTimer->stop();
            removeNowAccount();
            mySleep(1000);
            connectUi->close();
        }
    }


}

void DialogWIFIConnect::connectNewAccount()
{
    //下部命令后要空格
    QString command = "";
    FILE *fp = nullptr;
    char buf[15];
    std::string data("");
    //1.添加wpa网络
    if((fp = popen("wpa_cli -i wlan0 add_network","r")) != nullptr){
        while(fgets(buf, 15, fp) != nullptr){
            data = buf;
            if(data.find("FAIL") != std::string::npos  ){
                return;
            }else{
                mNetId = buf;
                mNetId = trimStr(mNetId);
            }
            memset(buf,'0',15);
        }
        pclose(fp);
        fp = nullptr;
    }
    //2.帐号
    command = "wpa_cli -i wlan0 set_network " + QString::fromStdString(mNetId)  + " ssid " + "'" +"\"" + \
    QString::fromStdString(WIFI_Single::instance()->getConnectingWIFI()) +"\"" +"'";
    system(qStringToC(command));
    //3.密码
    command = "wpa_cli -i wlan0 set_network " + QString::fromStdString(mNetId) + " psk " + "'" +"\"" + \
    WIFIPwdStr +"\"" + "'";
    system(qStringToC(command));
    //4.使能网卡
    command = "wpa_cli -i wlan0 enable_network " + QString::fromStdString(mNetId);
    system(qStringToC(command));
    //5.选择新加的网卡
    command = "wpa_cli -i wlan0 select_network " + QString::fromStdString(mNetId);
    system(qStringToC(command));
    setVerifySsidPsk(QString::fromStdString(WIFI_Single::instance()->getConnectingWIFI()),\
    WIFIPwdStr,atoi(mNetId.c_str()));

    mySleep(2000);
    //开启校验timer
    mPskVerifyTimer->start();
}

void DialogWIFIConnect::setVerifySsidPsk(QString ssid,QString psk,int id)
{
    mSsid = ssid.toStdString();
    mPsk = psk.toStdString();
    mNetId = to_string(id);
    DEBUG_I("ssid : %s ; psk : %s ; id : %s \n",ssid.toStdString().c_str(),psk.toStdString().c_str(),mNetId.c_str());
}

void DialogWIFIConnect::saveNowAccount()
{
    DEBUG_I("saveNowAccount  num : %s ssid : %s",mNetId.c_str(),mSsid.c_str());
    if (WIFI_Single::instance()->WIFI::getWifiConfig().find(atoi(mNetId.c_str())) != WIFI_Single::instance()->WIFI::getWifiConfig().end()){
        WIFI_Single::instance()->WIFI::getWifiConfig().erase(atoi(mNetId.c_str()));
    }
    WIFI_Single::instance()->WIFI::getWifiConfig().insert(std::make_pair(atoi(mNetId.c_str()),mSsid));
    system("wpa_cli -i wlan0 save_config");
    mSsid = "";
    mPsk = "";
    mNetId = "-1";
}

void DialogWIFIConnect::removeNowAccount()
{
    QString command;
    command = "wpa_cli -i wlan0 remove_network " + QString::fromStdString(mNetId);
    DEBUG_I("%s",qStringToC(command));
    system(qStringToC(command));
    system("wpa_cli -i wlan0 save_config");
    mSsid = "";
    mPsk = "";
    mNetId = "-1";
}
