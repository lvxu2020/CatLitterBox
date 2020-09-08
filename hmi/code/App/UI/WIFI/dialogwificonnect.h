#ifndef DIALOGWIFICONNECT_H
#define DIALOGWIFICONNECT_H

#include <QDialog>
#include <string.h>
#include "dialogconnectmask.h"

namespace Ui {
class DialogWIFIConnect;
}

class DialogWIFIConnect : public QDialog
{
    Q_OBJECT

public:
    explicit DialogWIFIConnect(QWidget *parent = nullptr);
    ~DialogWIFIConnect();
    void setWIFIPwdStrLen(int);
    void connectNewAccount();
    void setVerifySsidPsk(QString ssid,QString psk,int);
    void saveNowAccount();
    void removeNowAccount();

private slots:
    void on_btn_shift_clicked();

    void on_btn_space_clicked();

    void on_btn_Baskspace_clicked();

    void on_btn_lang_clicked();

    void on_btn_1_clicked();

    void on_btn_2_clicked();

    void on_btn_3_clicked();

    void on_btn_4_clicked();

    void on_btn_5_clicked();

    void on_btn_6_clicked();

    void on_btn_7_clicked();

    void on_btn_8_clicked();

    void on_btn_9_clicked();

    void on_btn_0_clicked();

    void on_btn_jian_clicked();

    void on_btn_deng_clicked();

    void on_btn_q_clicked();

    void on_btn_w_clicked();

    void on_btn_e_clicked();

    void on_btn_r_clicked();

    void on_btn_t_clicked();

    void on_btn_y_clicked();

    void on_btn_u_clicked();

    void on_btn_i_clicked();

    void on_btn_o_clicked();

    void on_btn_p_clicked();

    void on_btn_zuokuang_clicked();

    void on_btn_youkuang_clicked();

    void on_btn_fanxie_clicked();

    void on_btn_a_clicked();

    void on_btn_s_clicked();

    void on_btn_d_clicked();

    void on_btn_f_clicked();

    void on_btn_g_clicked();

    void on_btn_h_clicked();

    void on_btn_j_clicked();

    void on_btn_k_clicked();

    void on_btn_l_clicked();

    void on_btn_fenhao_clicked();

    void on_btn_danyin_clicked();

    void on_btn_z_clicked();

    void on_btn_x_clicked();

    void on_btn_c_clicked();

    void on_btn_v_clicked();

    void on_btn_b_clicked();

    void on_btn_n_clicked();

    void on_btn_m_clicked();

    void on_btn_douhao_clicked();

    void on_btn_jvhao_clicked();

    void on_btn_xiegang_clicked();

    void on_btnConnect_clicked();

    void slot_connectWifi();

    void slot_pskVerify();
private:
    Ui::DialogWIFIConnect *ui;
    DialogConnectMask *connectUi = nullptr;
    //键盘部分
    bool isSHIFT = false;
    QString WIFIPwdStr = "";
    int WIFIPwdStrLen = 0;

    QTimer* connectTimer = nullptr;
    //密码校验
    QTimer* mPskVerifyTimer = nullptr;
    std::string mSsid;
    std::string mPsk;
    std::string mNetId = "-1";

};

#endif // DIALOGWIFICONNECT_H
