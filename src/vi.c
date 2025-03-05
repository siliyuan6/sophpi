#include "cvi_sns_ctrl.h"
#include "cvi_awb.h"
#include "linux/cvi_comm_video.h"
#include "cvi_vi.h"

#define SENSOR_I2C_ADDR 48 // 十进制
#define SENSOR_MODE WDR_MODE_NONE
#define SENSOR_WIDTH 2304
#define SENSOR_HEIGHT 1296

static RX_INIT_ATTR_S s_stRxInitAttr =
{
    .MipiDev = 0,
    .as16LaneId = {0, 1, 2, -1, -1},
    .as8PNSwap = {1, 1, 1, 0, 0},
    .stMclkAttr.bMclkEn = 1,
    .stMclkAttr.u8Mclk = 1,
};

static ISP_SNS_COMMBUS_U s_stIspSnsCommBus =
{
    .s8I2cDev = 3,
};

static ISP_CMOS_SENSOR_IMAGE_MODE_S s_stIspCmosSensorImageMode =
{
    .f32Fps = 30,
    .u16Width = SENSOR_WIDTH,
    .u16Height = SENSOR_HEIGHT,
};

static VI_PIPE s_ViPipe = 0;
static VI_DEV s_ViDev = 0;

static VI_DEV_ATTR_S s_stViDevAttr =
{
    .enBayerFormat = BAYER_FORMAT_BG,
    .stSize.u32Width = SENSOR_WIDTH,
    .stSize.u32Height = SENSOR_HEIGHT,
    .stWDRAttr.enWDRMode = SENSOR_MODE,
};

static VI_PIPE_ATTR_S s_stViPipeAttr =
{
    .u32MaxW = SENSOR_WIDTH,
    .u32MaxH = SENSOR_HEIGHT,
    .enCompressMode = COMPRESS_MODE_TILE,
};

int vi_init(void)
{
    int ret = 0;

    ret = CVI_SYS_VI_Open();
    if (ret != 0)
    {
        log_err("CVI_SYS_VI_Open failed. ret:%d.");
        return ret;
    }

    ISP_SNS_OBJ_S *pstIspSnsObj = &stSnsSC3336_Obj;
    if (pstIspSnsObj->pfnPatchRxAttr)
    {
        ret = pstIspSnsObj->pfnPatchRxAttr(&s_stRxInitAttr);
        if (ret != 0)
        {
            log_err("pfnPatchRxAttr failed. ret:%d.");
            return ret;
        }
    }

    if (pstIspSnsObj->pfnSetBusInfo)
    {
        
        ret = pstIspSnsObj->pfnSetBusInfo(s_ViPipe, s_stIspSnsCommBus);
        if (ret != 0)
        {
            log_err("pfnSetBusInfo failed. ret:%d.");
            return ret;
        }
    }

    if (pstIspSnsObj->pfnPatchI2cAddr)
    {
        pstIspSnsObj->pfnPatchI2cAddr(SENSOR_I2C_ADDR);
    }

    if (pstIspSnsObj->pfnRegisterCallback)
    {
        ALG_LIB_S ae_lib;
        ALG_LIB_S awb_lib;
        ae_lib.s32Id = s_ViPipe;
        awb_lib.s32Id = s_ViPipe;
        strcpy(ae_lib.acLibName, CVI_AE_LIB_NAME);
        strcpy(awb_lib.acLibName, CVI_AWB_LIB_NAME);
        ret = pstIspSnsObj->pfnRegisterCallback(s_ViPipe, &ae_lib, &awb_lib);
        if (ret != 0)
        {
            log_err("pfnRegisterCallback failed. ret:%d.");
            return ret;
        }
    }

    if (pstIspSnsObj->pfnExpSensorCb) {
        ISP_SENSOR_EXP_FUNC_S stIspSensorExpFunc;
        ret = pstIspSnsObj->pfnExpSensorCb(&stIspSensorExpFunc);
        if (ret != 0)
        {
            log_err("pfnExpSensorCb failed. ret:%d.");
            return ret;
        }

        ret = stIspSensorExpFunc.pfn_cmos_set_image_mode(s_ViPipe, &s_stIspCmosSensorImageMode);
        if (ret != 0)
        {
            log_err("pfn_cmos_set_image_mode failed. ret:%d.");
            return ret;
        }

        ret = stIspSensorExpFunc.pfn_cmos_set_wdr_mode(s_ViPipe, SENSOR_MODE);
        if (ret != 0)
        {
            log_err("pfn_cmos_set_wdr_mode failed. ret:%d.");
            return ret;
        }
    }

    if (pstIspSnsObj->pfnGetRxAttr)
    {
        SNS_COMBO_DEV_ATTR_S stSnsComboDevAttr;
        ret = pstIspSnsObj->pfnGetRxAttr(s_ViPipe, &stSnsComboDevAttr);
        if (ret != 0)
        {
            log_err("pfnGetRxAttr failed. ret:%d.");
            return ret;
        }

        ret = CVI_MIPI_SetMipiAttr(s_ViPipe, (CVI_VOID*)&stSnsComboDevAttr);
        if (ret != 0)
        {
            log_err("CVI_MIPI_SetMipiAttr failed. ret:%d.");
            return ret;
        }
    }

    ret = CVI_MIPI_SetSensorClock(s_ViPipe, 1);
    if (ret != 0)
    {
        log_err("CVI_MIPI_SetSensorClock failed. ret:%d.");
        return ret;
    }

    ret = CVI_MIPI_SetSensorReset(s_ViPipe, 0);
    if (ret != 0)
    {
        log_err("CVI_MIPI_SetSensorClock failed. ret:%d.");
        return ret;
    }

    if (pstIspSnsObj->pfnSnsProbe) {
        ret = pstIspSnsObj->pfnSnsProbe(s_ViPipe);
        if (ret != 0)
        {
            log_err("pfnSnsProbe failed. ret:%d.");
            return ret;
        }
    }

    ret = CVI_VI_SetDevAttr(s_ViDev, &s_stViDevAttr);
    if (ret != 0)
    {
        log_err("CVI_VI_SetDevAttr failed. ret:%d.");
        return ret;
    }

    ret = CVI_VI_EnableDev(s_ViDev);
    if (ret != 0)
    {
        log_err("CVI_VI_EnableDev failed. ret:%d.");
        return ret;
    }

    ret = CVI_VI_CreatePipe(s_ViPipe, &s_stViPipeAttr);
    if (ret != 0)
    {
        log_err("CVI_VI_CreatePipe failed. ret:%d.");
        return ret;
    }

    ret = CVI_VI_StartPipe(s_ViPipe);
    if (ret != 0)
    {
        log_err("CVI_VI_StartPipe failed. ret:%d.");
        return ret;
    }



    return 0;
}

int vi_deinit(void)
{
    CVI_SYS_VI_Close();
    return 0;
}