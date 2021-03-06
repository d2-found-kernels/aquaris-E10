#include "platform.h"
#include "i2c.h"
#include "pmic_mt6333.h"

#ifdef MT6333_I2C_CHANNEL_CHANGED_0
    #define MT6333_I2C_ID	I2C0
#else
    #ifdef MT6333_I2C_CHANNEL_CHANGED_2
        #define MT6333_I2C_ID	I2C2
    #else
        #define MT6333_I2C_ID	I2C1
    #endif
#endif        

#define mt6333_SLAVE_ADDR_WRITE   0xD6
#define mt6333_SLAVE_ADDR_Read    0xD7

static struct mt_i2c_t mt6333_i2c;

/**********************************************************
  *
  *   [I2C Function For Read/Write mt6333] 
  *
  *********************************************************/
kal_uint32 mt6333_write_byte(kal_uint8 addr, kal_uint8 value)
{
        kal_uint32 ret_code = I2C_OK;
        kal_uint8 write_data[2];
        kal_uint16 len;

        write_data[0]= addr;
        write_data[1] = value;

        mt6333_i2c.id = MT6333_I2C_ID;
		/* Since i2c will left shift 1 bit, we need to set MT6333 I2C address to >>1 */
        mt6333_i2c.addr = (mt6333_SLAVE_ADDR_WRITE >> 1);
        mt6333_i2c.mode = ST_MODE;
        mt6333_i2c.speed = 100;
        len = 2;

        ret_code = i2c_write(&mt6333_i2c, write_data, len);
        printf("%s: i2c_write: ret_code: %d\n", __func__, ret_code);

        return ret_code;
}

kal_uint32 mt6333_read_byte (kal_uint8 addr, kal_uint8 *dataBuffer) {
        kal_uint32 ret_code = I2C_OK;
        kal_uint16 len;
		*dataBuffer = addr;

        mt6333_i2c.id = MT6333_I2C_ID;
		/* Since i2c will left shift 1 bit, we need to set MT6333 I2C address to >>1 */
        mt6333_i2c.addr = (mt6333_SLAVE_ADDR_WRITE >> 1);
        mt6333_i2c.mode = ST_MODE;
        mt6333_i2c.speed = 100;
        len = 1;

        ret_code = i2c_write_read(&mt6333_i2c, dataBuffer, len, len);
        printf("%s: i2c_read: ret_code: %d\n", __func__, ret_code);

        return ret_code;
}

/**********************************************************
  *
  *   [Read / Write Function] 
  *
  *********************************************************/
kal_uint32 mt6333_read_interface (kal_uint8 RegNum, kal_uint8 *val, kal_uint8 MASK, kal_uint8 SHIFT)
{
    kal_uint8 mt6333_reg = 0;
    int ret = 0;

#if 1
    //printf("--------------------------------------------------PL\n");

    ret = mt6333_read_byte(RegNum, &mt6333_reg);
    //printf("[mt6333_read_interface] Reg[%x]=0x%x\n", RegNum, mt6333_reg);
    
    mt6333_reg &= (MASK << SHIFT);
    *val = (mt6333_reg >> SHIFT);    
    //printf("[mt6333_read_interface] val=0x%x\n", *val);
#endif    

    return ret;
}

kal_uint32 mt6333_config_interface (kal_uint8 RegNum, kal_uint8 val, kal_uint8 MASK, kal_uint8 SHIFT)
{
    kal_uint8 mt6333_reg = 0;
    int ret = 0;

#if 1
    //printf("--------------------------------------------------PL\n");

    ret = mt6333_read_byte(RegNum, &mt6333_reg);
    //printf("[mt6333_config_interface] Reg[%x]=0x%x\n", RegNum, mt6333_reg);
    
    mt6333_reg &= ~(MASK << SHIFT);
    mt6333_reg |= (val << SHIFT);

    ret = mt6333_write_byte(RegNum, mt6333_reg);
    //printf("[mt6333_config_interface] write Reg[%x]=0x%x\n", RegNum, mt6333_reg);

    // Check
    //mt6333_read_byte(RegNum, &mt6333_reg);
    //printf("[mt6333_config_interface] Check Reg[%x]=0x%x\n", RegNum, mt6333_reg);
#endif

    return ret;
}  

kal_uint32 mt6333_get_reg_value(kal_uint32 reg)
{
    kal_uint32 ret=0;
    kal_uint8 reg_val=0;

    ret=mt6333_read_interface( (kal_uint8) reg, &reg_val, 0xFF, 0x0);
    
    return reg_val;
}

//==============================================================================
// PMIC6323 Init Code
//==============================================================================
void mt6333_init (void)
{
    kal_uint32 ret=0;
    kal_uint8 reg_val=0;
    int polling_timeout_value = 10;
    int polling_time = 0;
    
    printf("[mt6333_init] Reg[%x]=0x%x\n", MT6333_CID0, mt6333_get_reg_value(MT6333_CID0));
    printf("[mt6333_init] Reg[%x]=0x%x\n", MT6333_CID1, mt6333_get_reg_value(MT6333_CID1));
    printf("[mt6333_init] Reg[%x]=0x%x\n", 0x2C, mt6333_get_reg_value(0x2C)); //E3:0x10

    ret = mt6333_config_interface(0xA0, 0, 0x1, 0x2);  // [2:2]: RG_VCORE_VOSEL_SPM_SET; set VCORE to hw ctrl
    ret = mt6333_config_interface(0x9F, 0, 0x3, 0x0);  // [0:1]: VRF18_EN, RG_VRF18_MODESET; Disable VRF18 and set to auto mode
    ret = mt6333_config_interface(0x16, 0, 0x1, 0x0);  // [0:0]: Disable boost
    ret = mt6333_config_interface(0x31, 0, 0x1, 0x0);  // [0:0]: RG_FLASH_EN; Disable flashlight driver

    #ifdef DUMMY_AP
    ret = mt6333_config_interface(0x9F, 0x6A, 0xFF, 0);
    printf("[mt6333_init for DUMMY_AP] Reg[%x]=0x%x\n", 0x9F, mt6333_get_reg_value(0x9F));
    #endif
    
    #if 0
    //enable charger when charger boot
    ret=mt6333_read_interface(0x1B, &reg_val, 0x1, 2);
    if(reg_val == 1)
    {
        ret=mt6333_config_interface(0x2D, 0x1, 0x1, 1); //mt6333_set_rg_usbdl_mode_b(1);
        
        ret=mt6333_read_interface(0x24, (&reg_val), 0x1, 0); //mt6333_get_rgs_power_on_ready()
        while(reg_val != 1)
        {
            if(polling_time++ >= polling_timeout_value)
            {
                printf("check rgs_power_on_ready fail\n");
                break;
            }
            ret=mt6333_read_interface(0x24, (&reg_val), 0x1, 0); //mt6333_get_rgs_power_on_ready()
        }
        printf("polling_time=%d of rgs_power_on_ready\n", polling_time);

        ret=mt6333_config_interface(0x2B, 0x4, 0xF, 0); // RG_ICH_SEL[3:0]=0x4, 500mA
        ret=mt6333_config_interface(0x2A, 0x3, 0xF, 4); // RG_CV_SEL[3:0]=0x4, 4.2V
        ret=mt6333_config_interface(0x04, 0x95, 0xFF, 0x0); // CHR_EN=1

        printf("[mt6333_init] Reg[%x]=0x%x\n", 0x04, mt6333_get_reg_value(0x04));
        printf("[mt6333_init] Reg[%x]=0x%x\n", 0x1B, mt6333_get_reg_value(0x1B));        
        printf("[mt6333_init] Reg[%x]=0x%x\n", 0x24, mt6333_get_reg_value(0x24));
        printf("[mt6333_init] Reg[%x]=0x%x\n", 0x2A, mt6333_get_reg_value(0x2A));
        printf("[mt6333_init] Reg[%x]=0x%x\n", 0x2B, mt6333_get_reg_value(0x2B));        
        printf("[mt6333_init] Reg[%x]=0x%x\n", 0x2D, mt6333_get_reg_value(0x2D));
    }
    #endif
}

