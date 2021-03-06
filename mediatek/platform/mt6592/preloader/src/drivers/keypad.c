/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein is
 * confidential and proprietary to MediaTek Inc. and/or its licensors. Without
 * the prior written permission of MediaTek inc. and/or its licensors, any
 * reproduction, modification, use or disclosure of MediaTek Software, and
 * information contained herein, in whole or in part, shall be strictly
 * prohibited.
 * 
 * MediaTek Inc. (C) 2010. All rights reserved.
 * 
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER
 * ON AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL
 * WARRANTIES, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR
 * NONINFRINGEMENT. NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH
 * RESPECT TO THE SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY,
 * INCORPORATED IN, OR SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES
 * TO LOOK ONLY TO SUCH THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO.
 * RECEIVER EXPRESSLY ACKNOWLEDGES THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO
 * OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES CONTAINED IN MEDIATEK
 * SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK SOFTWARE
 * RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S
 * ENTIRE AND CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE
 * RELEASED HEREUNDER WILL BE, AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE
 * MEDIATEK SOFTWARE AT ISSUE, OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE
 * CHARGE PAID BY RECEIVER TO MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek
 * Software") have been modified by MediaTek Inc. All revisions are subject to
 * any receiver's applicable license agreements with MediaTek Inc.
 */

#include "typedefs.h"
#include "platform.h"
#include "keypad.h"
#include "pmic.h"
#include <gpio.h>

extern pmic_detect_powerkey(void);
void mtk_kpd_gpio_set(void);
extern U32 pmic_read_interface (U32 RegNum, U32 *val, U32 MASK, U32 SHIFT);
extern U32 get_pmic6320_chip_version (void);

void mtk_kpd_gpios_get(unsigned int ROW_REG[], unsigned int COL_REG[])
{
	int i;
	for(i = 0; i< 8; i++)
	{
		ROW_REG[i] = 0;
		COL_REG[i] = 0;
	}
	#ifdef GPIO_KPD_KROW0_PIN
		ROW_REG[0] = GPIO_KPD_KROW0_PIN;
	#endif

	#ifdef GPIO_KPD_KROW1_PIN
		ROW_REG[1] = GPIO_KPD_KROW1_PIN;
	#endif

	#ifdef GPIO_KPD_KROW2_PIN
		ROW_REG[2] = GPIO_KPD_KROW2_PIN;
	#endif

	#ifdef GPIO_KPD_KROW3_PIN
		ROW_REG[3] = GPIO_KPD_KROW3_PIN;
	#endif

	#ifdef GPIO_KPD_KROW4_PIN
		ROW_REG[4] = GPIO_KPD_KROW4_PIN;
	#endif

	#ifdef GPIO_KPD_KROW5_PIN
		ROW_REG[5] = GPIO_KPD_KROW5_PIN;
	#endif

	#ifdef GPIO_KPD_KROW6_PIN
		ROW_REG[6] = GPIO_KPD_KROW6_PIN;
	#endif

	#ifdef GPIO_KPD_KROW7_PIN
		ROW_REG[7] = GPIO_KPD_KROW7_PIN;
	#endif


	#ifdef GPIO_KPD_KCOL0_PIN
		COL_REG[0] = GPIO_KPD_KCOL0_PIN;
	#endif

	#ifdef GPIO_KPD_KCOL1_PIN
		COL_REG[1] = GPIO_KPD_KCOL1_PIN;
	#endif

	#ifdef GPIO_KPD_KCOL2_PIN
		COL_REG[2] = GPIO_KPD_KCOL2_PIN;
	#endif

	#ifdef GPIO_KPD_KCOL3_PIN
		COL_REG[3] = GPIO_KPD_KCOL3_PIN;
	#endif

	#ifdef GPIO_KPD_KCOL4_PIN
		COL_REG[4] = GPIO_KPD_KCOL4_PIN;
	#endif

	#ifdef GPIO_KPD_KCOL5_PIN
		COL_REG[5] = GPIO_KPD_KCOL5_PIN;
	#endif

	#ifdef GPIO_KPD_KCOL6_PIN
		COL_REG[6] = GPIO_KPD_KCOL6_PIN;
	#endif

	#ifdef GPIO_KPD_KCOL7_PIN
		COL_REG[7] = GPIO_KPD_KCOL7_PIN;
	#endif
}

void mtk_kpd_gpio_set(void)
{
	unsigned int ROW_REG[8];
	unsigned int COL_REG[8];
	int i;

	print("Enter mtk_kpd_gpio_set! \n");
	mtk_kpd_gpios_get(ROW_REG, COL_REG);
	
	for(i = 0; i < 8; i++)
	{
		if (COL_REG[i] != 0)
		{
			/* KCOL: GPIO INPUT + PULL ENABLE + PULL UP */
			mt_set_gpio_mode(COL_REG[i], 1);
			mt_set_gpio_dir(COL_REG[i], 0);
			mt_set_gpio_pull_enable(COL_REG[i], 1);
			mt_set_gpio_pull_select(COL_REG[i], 1);
		}
		
		if(ROW_REG[i] != 0)
		{
			/* KROW: GPIO output + pull disable + pull down */
			mt_set_gpio_mode(ROW_REG[i], 1);
			mt_set_gpio_dir(ROW_REG[i], 1);
			mt_set_gpio_pull_enable(ROW_REG[i], 0);	
			mt_set_gpio_pull_select(ROW_REG[i], 0);
		}
	}

	mdelay(33);
}

void set_kpd_pmic_mode()
{
	unsigned int a,c;
	a = pwrap_read(0x0040,&c);
	if(a != 0)
 	print("kpd write fail, addr: 0x0040\n");
 	
	print("kpd read addr: 0x0040: data:0x%x\n", c);
	c=c&0xFFFE;
	a = pwrap_write(0x0040,c);
	if(a != 0)
 	print("kpd write fail, addr: 0x0040\n");

	mtk_kpd_gpio_set();

#if KPD_USE_EXTEND_TYPE		//double keypad
	*(volatile u16 *)(KP_SEL) |= 0x1;
#else			//single keypad
	*(volatile u16 *)(KP_SEL) &= (~0x1);

#endif	
	
	*(volatile u16 *)(KP_EN) = 0x1;
	print("after set KP enable: KP_SEL = 0x%x !\n", DRV_Reg16(KP_SEL));

#ifdef MTK_PMIC_RST_KEY
	print("MTK_PMIC_RST_KEY is used for this project!\n");
    	pmic_config_interface(STRUP_CON3, 0x01, PMIC_RG_FCHR_PU_EN_MASK, PMIC_RG_FCHR_PU_EN_SHIFT);//pull up homekey pin of PMIC for 82 project
#endif


	return;
}

void disable_PMIC_kpd_clock(void)
{
#if 0
	int rel = 0;
	//print("kpd disable_PMIC_kpd_clock register!\n");
	rel = pmic_config_interface(WRP_CKPDN,0x1, PMIC_RG_WRP_32K_PDN_MASK, PMIC_RG_WRP_32K_PDN_SHIFT);
	if(rel !=  0){
		print("kpd disable_PMIC_kpd_clock register fail!\n");
	}
#endif
}
void enable_PMIC_kpd_clock(void)
{
#if 0
	int rel = 0;
	//print("kpd enable_PMIC_kpd_clock register!\n");
	rel = pmic_config_interface(WRP_CKPDN,0x0, PMIC_RG_WRP_32K_PDN_MASK, PMIC_RG_WRP_32K_PDN_SHIFT);
	if(rel !=  0){
		print("kpd enable_PMIC_kpd_clock register fail!\n");
	}
#endif
}

bool mtk_detect_pmic_just_rst(void)
{
	kal_uint32 just_rst=0;

	printf("detecting pmic just reset\n");

		pmic_read_interface(0x04A, &just_rst, 0x01, 14);
		if(just_rst)
		{
			printf("Just recover form a reset\n");
			pmic_config_interface(0x04A, 0x01, 0x01, 4);
			return true;
		}
	return false;
}


bool mtk_detect_key(unsigned short key)  /* key: HW keycode */
{
    unsigned short idx, bit, din;
    U32 just_rst;

    if (key >= KPD_NUM_KEYS)
        return false;

    if (key % 9 == 8)
        key = 8;

	if (key == 8)
		{	/* Power key */
			#if 0 // for long press reboot, not boot up from a reset
			pmic_read_interface(0x04A, &just_rst, 0x01, 14);
			if(just_rst)
			{
				pmic_config_interface(0x04A, 0x01, 0x01, 4);
				print("Just recover from a reset\n"); 
				return false;
			}
			#endif
			if (1 == pmic_detect_powerkey())
			{
				print ("power key is pressed\n");
				return true;
			}
			return false;
		}

#ifdef MTK_PMIC_RST_KEY
	if(key == MTK_PMIC_RST_KEY)
	{
	print("mtk detect key function pmic_detect_homekey MT65XX_PMIC_RST_KEY = %d\n",MTK_PMIC_RST_KEY);
		if (1 == pmic_detect_homekey())
		{
			print("mtk detect key function pmic_detect_homekey pressed\n");
			return TRUE;
		}
		return FALSE;
	}
#endif

    idx = key / 16;
    bit = key % 16;

    din = DRV_Reg16(KP_MEM1 + (idx << 2)) & (1U << bit);
    if (!din) /* key is pressed */
    {
        print("key %d is pressed\n", key);
        return true;
    }
    return false;
}

bool mtk_detect_dl_keys(void)
{
#ifdef KPD_DL_KEY1
    if (!mtk_detect_key (KPD_DL_KEY1))
	return false;
#endif
#ifdef KPD_DL_KEY2
    if (!mtk_detect_key (KPD_DL_KEY2))
	return false;
#endif
#ifdef KPD_DL_KEY3    
    if (!mtk_detect_key (KPD_DL_KEY3)) 	 	
	return false;
#endif
#ifdef MTK_PMIC_RST_KEY
    if (!mtk_detect_key (MTK_PMIC_RST_KEY))
	return false;
#endif
    {
        print("download keys are pressed\n");
        return true;
    }
}

