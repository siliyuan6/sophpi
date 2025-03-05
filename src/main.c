#include <stdlib.h>

#include "log.h"
#include "sys.h"

/**
 * @brief 设置sensor引脚pinmux
 *
 * 配置IIC3、RESET、MCLK引脚的pinmux寄存器
 *
 * @return 返回 0 表示成功
 */
int sensor_pin_set(void)
{
	// IIC3
	system("devmem 0x03001018 32 0x0");
	system("devmem 0x03001014 32 0x0");
	// RESET
	system("devmem 0x03001008 32 0x3");
	// MCLK
	system("devmem 0x0300100C 32 0x0");

	return 0;
}



int main(int argc, char *argv[])
{
    int ret = 0;

    sensor_pin_set();

	sys_ver_pri();
	
    ret = sys_init();
    if (ret != 0)
    {
        log_err("sys_init failed. ret:%d.");
        return ret;
    }

    ret = sys_deinit();
    if (ret != 0)
    {
        log_err("sys_init failed. ret:%d.");
        return ret;
    }

	return 0;
}
