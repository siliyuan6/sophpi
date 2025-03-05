#include "cvi_vb.h"
#include "cvi_sys.h"
#include "cvi_vi.h"

#include "log.h"

static VB_CONFIG_S s_stVbConf = 
{
    .u32MaxPoolCnt = 1,
    .astCommPool[0] = 
    {
        .u32BlkSize = 2304*1344*3/2,
        .u32BlkCnt = 5,
        .enRemapMode = VB_REMAP_MODE_CACHED,
    },
};

static VI_VPSS_MODE_S s_stViVpssMode = 
{
    .aenMode[0] = VI_OFFLINE_VPSS_OFFLINE,
};

static VPSS_MODE_S s_stVpssMode = 
{
    .enMode = VPSS_MODE_DUAL,
    .aenInput[0] = VPSS_INPUT_MEM,
    .aenInput[1] = VPSS_INPUT_ISP,
    .ViPipe[0] = 0,
    .ViPipe[1] = 0,
};

/**
 * @brief 打印SDK版本
 * 
 * @return 返回 0 表示成功
 */
int sys_ver_pri(void)
{
	MMF_VERSION_S stVersion;
	CVI_SYS_GetVersion(&stVersion);
	log_info("MMF Version:%s.", stVersion.version);
}

/**
 * @brief 初始化vb内存以及vpss配置
 * 
 * @return 返回 0 表示成功
 */
int sys_init(void)
{
    int ret = 0;

    log_info("sys_init start.");

    CVI_SYS_Exit();
    CVI_VB_Exit();
    ret = CVI_VB_SetConfig(&s_stVbConf);
    if (ret != 0)
    {
        log_err("COMM_SYS_Init failed. ret:%d.");
        return ret;
    }

    CVI_VB_Init();
    CVI_SYS_Init();

    ret = CVI_SYS_SetVIVPSSMode(&s_stViVpssMode);
    if (ret != 0)
    {
        log_err("CVI_SYS_SetVIVPSSMode failed. ret:%d.");
        return ret;
    }

    ret = CVI_SYS_SetVPSSModeEx(&s_stVpssMode);
    if (ret != 0)
    {
        log_err("CVI_SYS_SetVPSSModeEx failed. ret:%d.");
        return ret;
    }
    log_info("sys_init done.");

    return 0;
}

/**
 * @brief 初始化vb内存以及vpss配置
 * 
 * @return 返回 0 表示成功
 */
int sys_deinit(void)
{
    log_info("sys_deinit start.");

    CVI_SYS_Exit();
    CVI_VB_Exit();

    log_info("sys_deinit done.");

    return 0;
}