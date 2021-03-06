#ifndef __DBG_FLASH_PARAM_H__
#define __DBG_FLASH_PARAM_H__

// flash Debug Tag
enum
{
	// flash
	FL_T_VERSION=0,
	FL_T_SCENE_MODE,
	FL_T_IS_FLASH_ON,
	FL_T_ISO,
	FL_T_AFE_GAIN,
	FL_T_ISP_GAIN,
	FL_T_EXP_TIME,
	FL_T_DUTY,
	FL_T_STEP,
	FL_T_ERR1,
	FL_T_ERR2,
	FL_T_ERR3,
	FL_T_ERR1_TIME,
	FL_T_ERR2_TIME,
	FL_T_ERR3_TIME,
	FL_T_VBAT,
	FL_T_ISO_INC_MODE,
	FL_T_ISO_INC_VALUE,
	FL_T_PF_I,
	FL_T_MF_I_MIN,
	FL_T_MF_I_MAX,
	FL_T_PMF_I_PEAK,
	FL_T_TORCH_I_PEAK,
	FL_T_TORCH_I,
	FL_T_PF_START_COOLING_TIME,
	FL_T_PF_START_TIME,
	FL_T_PF_END_TIME,
	FL_T_PRE_FIRE_ST_TIME,
	FL_T_PRE_FIRE_ED_TIME,
	FL_T_COOLING_TIME,
	FL_T_EST_PFMF_TIME,
	FL_T_DELAY_TIME,
	FL_T_NUM,
};

#define FLASH_DEBUG_TAG_VERSION (1)
#define FLASH_TAG_NUM (FL_T_NUM + 578/*125*/)

typedef struct
{
    AAA_DEBUG_TAG_T Tag[FLASH_TAG_NUM];
} FLASH_DEBUG_INFO_T;

#endif // __DBG_FLASH_PARAM_H__

