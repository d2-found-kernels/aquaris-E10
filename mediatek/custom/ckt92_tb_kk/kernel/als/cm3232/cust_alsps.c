#include <linux/types.h>
#include <mach/mt_pm_ldo.h>
#include <cust_alsps.h>
#include <mach/upmu_common.h>

static struct alsps_hw cust_alsps_hw = {
    .i2c_num    = 0,
	.polling_mode_ps =0,
	.polling_mode_als =1,
    .power_id   = MT65XX_POWER_NONE,    /*LDO is not used*/
    .power_vol  = VOL_DEFAULT,          /*LDO is not used*/
    //.i2c_addr   = {0x0C, 0x48, 0x78, 0x00},
    //.als_level  = { 0,  1,  1,   7,  15,  15,  100, 1000, 2000,  3000,  6000, 10000, 14000, 18000, 20000},
    //.als_value  = {40, 40, 90,  90, 160, 160,  225,  320,  640,  1280,  1280,  2600,  2600, 2600,  10240, 10240},
    /* MTK: modified to support AAL */
    .als_level  = { 0, 45, 45, 45, 303, 303, 303, 1305, 1305, 2699, 2699, 7622, 38416, 46881, 65535},
    .als_value  = { 0, 80, 80, 80, 107, 107, 107,  650,  650, 1160, 1160, 3100,  5000,  8500, 10050, 10240},
    //.als_level  = {0,3, 9, 22,  40, 70, 140,280,   560,650,1900,5800, 10240,10241},
    //.als_value  = {0,18,60,100, 220,230,250,300,   500,560,1200,2600, 10240,10240},
    .ps_threshold_high = 31,
    .ps_threshold_low = 21,
};
struct alsps_hw *get_cust_alsps_hw(void) {
    return &cust_alsps_hw;
}


