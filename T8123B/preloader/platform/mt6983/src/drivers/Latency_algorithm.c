#include <stdio.h>
#include <string.h>
#define mcSHOW_DBG_MSG(x) {printf x;}

typedef struct TX_path_config_parameter
{
    unsigned int data_rate;
    unsigned int TDQS2DQ[2]; //ps
    unsigned int TDQSS[2]; //dq ui
    unsigned int WL[2];  //tck
    unsigned int DBI;
    unsigned int ECC_EN;
    unsigned int DQ_P2S_RATIO;
    unsigned int CA_P2S_RATIO;
    unsigned int CKR;
} TX_path_latency_config ;

//MCK for Channel
//UI for Byte
//PI for Byte  --PHY
//Dline for bit--PHY
typedef struct TX_latency_control_rg
{
   unsigned char TX_DQS_MCK;
   unsigned char TX_DQS_MCK_OEN;
   unsigned char TX_DQS_UI[2][4];
   unsigned char TX_DQS_UI_OEN[2][4];

   unsigned char TX_DQ_MCK;
   unsigned char TX_DQ_MCK_OEN;
   unsigned char TX_DQ_UI[2][4];
   unsigned char TX_DQ_UI_OEN[2][4];

   unsigned char TX_DQ_PI[2][4];
   unsigned char TX_DQ_Dline[2][4][8];
   unsigned int  DQSOSCTHRD_INC[2];
   unsigned int  DQSOSCTHRD_DEC[2];
}TX_latency_control_rg;

unsigned int TX_DQSOSCTHRD_caculate(unsigned int MR23, unsigned int data_rate,unsigned int TDQS2DQ,unsigned char INC_EN)
{
    int DQSOSCTHRD;
    int inc_dec_magic_number;

    inc_dec_magic_number = (INC_EN == 1) ? 24 : 16;

    DQSOSCTHRD = MR23*inc_dec_magic_number*1000000/(40*data_rate*data_rate) * (1000000/(TDQS2DQ *TDQS2DQ)) ;

    mcSHOW_DBG_MSG("DQSOSCTHRD = %1d, %s, data_rate = %1d, MR23=%2d,TDQS2DQ=%1d", DQSOSCTHRD,INC_EN==1?"INCREASE":"DECREASE",data_rate,MR23,TDQS2DQ );

    return DQSOSCTHRD;
}


void TX_path_algorithm( TX_path_latency_config *in_p)
{
    int TX_dq_latency_ps[2];
    int TX_dqs_latency_ps[2];
    int DQ_UI_ps;
    int CA_UI_ps;
    int least_tracking_margin = 120; //ps
    int TX_DQ_PI_min;
    int TX_DQ_PI_max;
    int Rerseved_ps_after_DQUI_cut[2];
    int MR23 = 63;
    int TX_DQS_UI[2];
    int TX_DQ_UI[2];
    int TX_DQ_PI[2];
    int rank_id = 0;

    int TX_DQS_UI_temp[2];
    
    DQ_UI_ps = 1000000/(in_p->data_rate);

    CA_UI_ps = DQ_UI_ps * in_p->CKR;

    TX_DQ_PI_min = least_tracking_margin*32/DQ_UI_ps +1;//+1 for RoundUP
    TX_DQ_PI_max = 64-TX_DQ_PI_min;

    for(rank_id = 0; rank_id < 2; rank_id ++)
    {
        TX_dq_latency_ps[rank_id]           = in_p->WL[rank_id] * in_p->CKR * 2 * DQ_UI_ps -(in_p->DBI * in_p->DQ_P2S_RATIO)*DQ_UI_ps + in_p->TDQS2DQ[rank_id] + in_p->TDQSS[rank_id]*DQ_UI_ps + DQ_UI_ps/2;
        TX_DQ_UI[rank_id]                   = (TX_dq_latency_ps[rank_id]-least_tracking_margin) / DQ_UI_ps;
        Rerseved_ps_after_DQUI_cut[rank_id] = TX_dq_latency_ps[rank_id] - TX_DQ_UI[rank_id]*DQ_UI_ps;
        TX_DQ_PI[rank_id]                   = Rerseved_ps_after_DQUI_cut[rank_id]*32 / DQ_UI_ps;

        mcSHOW_DBG_MSG("[TX_path_calculate]rank_id = %1d,TX_dq_latency_ps=%1d,Rerseved_ps_after_DQUI_cut=%1d,TX_DQ_UI=%1d,DQ_UI_ps=%1d,TX_DQ_PI=%1d,TX_DQ_PI_min=%1d,TX_DQ_PI_max=%1d\n",
                    rank_id,TX_dq_latency_ps[rank_id],Rerseved_ps_after_DQUI_cut[rank_id],TX_DQ_UI[rank_id],DQ_UI_ps,TX_DQ_PI[rank_id],TX_DQ_PI_min,TX_DQ_PI_max);
        TX_DQS_UI[rank_id]                  = in_p->WL[rank_id] * in_p->CKR * 2 + 1;//+1 for TX CA central align --LP5 CA width only 1UI so for central align shoudl shift 1/2 MCKIO 

        TX_DQ_OE_UI[rank_id]  = TX_DQ_UI[rank_id]  - (in_p->DQ_P2S_RATIO==4)?3:3 + (in_p->DBI * in_p->DQ_P2S_RATIO);//notice here LP5 will -4?????  TODO
        TX_DQS_OE_UI[rank_id] = TX_DQS_UI[rank_id] - (in_p->DQ_P2S_RATIO==4)?3:4;

    }
}

int main()
{
    TX_path_latency_config in_p;

    in_p.TDQS2DQ[0]   = 400; //ps
    in_p.TDQS2DQ[1]   = 400; //ps
    in_p.TDQSS[0]     = 2; //dq ui
    in_p.TDQSS[1]     = 2; //dq ui
    in_p.ECC_EN       = 1;
    in_p.CA_P2S_RATIO = 4;
    in_p.CKR          = 1;
    
    in_p.data_rate    = 4266;
    in_p.WL[0]        = 18;  //tck
    in_p.WL[1]        = 18;  //tck
    in_p.DBI          = 1;
    in_p.DQ_P2S_RATIO = 8;
    mcSHOW_DBG_MSG("\n[TX_path_calculate]data_rate=%1d\n",in_p.data_rate);
    TX_path_algorithm(&in_p);

    in_p.data_rate    = 3733;
    in_p.WL[0]        = 16;  //tck
    in_p.WL[1]        = 16;  //tck
    in_p.DBI          = 1;
    in_p.DQ_P2S_RATIO = 8;
    mcSHOW_DBG_MSG("\n[TX_path_calculate]data_rate=%1d\n",in_p.data_rate);
    TX_path_algorithm(&in_p);

    in_p.data_rate    = 3200;
    in_p.WL[0]        = 14;  //tck
    in_p.WL[1]        = 14;  //tck
    in_p.DBI          = 1;
    in_p.DQ_P2S_RATIO = 8;
    mcSHOW_DBG_MSG("\n[TX_path_calculate]data_rate=%1d\n",in_p.data_rate);
    TX_path_algorithm(&in_p);

    in_p.data_rate    = 1600;
    in_p.WL[0]        = 8;  //tck
    in_p.WL[1]        = 8;  //tck
    in_p.DBI          = 0;
    in_p.DQ_P2S_RATIO = 8;
    mcSHOW_DBG_MSG("\n[TX_path_calculate]data_rate=%1d\n",in_p.data_rate);
    TX_path_algorithm(&in_p);

    in_p.data_rate    = 1600;
    in_p.WL[0]        = 8;  //tck
    in_p.WL[1]        = 8;  //tck
    in_p.DBI          = 0;
    in_p.DQ_P2S_RATIO = 4;
    mcSHOW_DBG_MSG("\n[TX_path_calculate]data_rate=%1d\n",in_p.data_rate);
    TX_path_algorithm(&in_p);

    in_p.data_rate    = 800;
    in_p.WL[0]        = 8;  //tck
    in_p.WL[1]        = 8;  //tck
    in_p.DBI          = 0;
    in_p.DQ_P2S_RATIO = 4;
    mcSHOW_DBG_MSG("\n[TX_path_calculate]data_rate=%1d\n",in_p.data_rate);
    TX_path_algorithm(&in_p);
}
