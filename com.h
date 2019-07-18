#ifndef COM_H
#define COM_H

#define FW_Date  0x20160421

#define ConfigAddr  0x800FC00

#define ConfigValid 0x006fb67c
#define ConfigSave 0x00F6B6C7

#define DefaultI2CAddr 0x4B

#define MaxDuty 1000
#define MaxFre  10000
#define AngleDiv 180

enum PARAM_REG
{
	CONFIG_VALID = 0,
	CONFIG_TB_PWM_FREQ = CONFIG_VALID + 4,

	I2C_ADDRESS = CONFIG_TB_PWM_FREQ + 4,

	TB_1A_MODE,
	TB_1A_DIR,
	TB_1A_DUTY,
	TB_1A_SPM_SPEED = TB_1A_DUTY + 2,
	TB_1A_SPM_STEP = TB_1A_SPM_SPEED + 4,

	TB_1B_MODE = TB_1A_SPM_STEP + 4,
	TB_1B_DIR,
	TB_1B_DUTY,
	TB_1B_SPM_SPEED = TB_1B_DUTY + 2,
	TB_1B_SPM_STEP = TB_1B_SPM_SPEED + 4,

	TB_2A_MODE = TB_1B_SPM_STEP + 4,
	TB_2A_DIR,
	TB_2A_DUTY,
	TB_2A_SPM_SPEED = TB_2A_DUTY + 2,
	TB_2A_SPM_STEP = TB_2A_SPM_SPEED + 4,

	TB_2B_MODE = TB_2A_SPM_STEP + 4,
	TB_2B_DIR,
	TB_2B_DUTY,
	TB_2B_SPM_SPEED = TB_2B_DUTY + 2,
	TB_2B_SPM_STEP = TB_2B_SPM_SPEED + 4,

	SVM1_STATE = TB_2B_SPM_STEP + 4,
	SVM1_FREQ,
	SVM1_ANGLE = SVM1_FREQ + 2,

	SVM2_STATE = SVM1_ANGLE + 2,
	SVM2_FREQ,
	SVM2_ANGLE = SVM2_FREQ + 2,

	SVM3_STATE = SVM2_ANGLE + 2,
	SVM3_FREQ,
	SVM3_ANGLE = SVM3_FREQ + 2,

	SVM4_STATE = SVM3_ANGLE + 2,
	SVM4_FREQ,
	SVM4_ANGLE = SVM4_FREQ + 2,

	SVM5_STATE = SVM4_ANGLE + 2,
	SVM5_FREQ,
	SVM5_ANGLE = SVM5_FREQ + 2,

	SVM6_STATE = SVM5_ANGLE + 2,
	SVM6_FREQ,
	SVM6_ANGLE = SVM6_FREQ + 2,

	IO1_STATE = SVM6_ANGLE + 2,
	IO1_MODE,
	IO1_PUPD,
	IO1_PPOD,

	IO2_STATE,
	IO2_MODE,
	IO2_PUPD,
	IO2_PPOD,

	IO3_STATE,
	IO3_MODE,
	IO3_PUPD,
	IO3_PPOD,

	IO4_STATE,
	IO4_MODE,
	IO4_PUPD,
	IO4_PPOD,

	IO5_STATE,
	IO5_MODE,
	IO5_PUPD,
	IO5_PPOD,

	IO6_STATE,
	IO6_MODE,
	IO6_PUPD,
	IO6_PPOD,

	FW_DATE,

	PARAM_REG_NUM = FW_DATE + 4
};

enum TB_PORT
{
	TB_1A = 0,
	TB_1B,
	TB_2A,
	TB_2B,
	TB_PORT_NUM
};

enum TB_WORKMODE
{
	TB_SHORT_BREAK = 0,
	TB_CW,
	TB_CCW,
	TB_STOP,
	TB_WORKMODE_NUM
};

enum TB_PORTMODE
{
	TB_DCM = 0,
	TB_SPM,
	TB_PORTMODE_NUM
};

enum SVM_PORT
{
	SVM1 = 0,
	SVM2,
	SVM3,
	SVM4,
	SVM5,
	SVM6,
	SVM_PORT_NUM
};

enum SVM_STATE
{
	SVM_DISABLE = 0,
	SVM_ENABLE,
	SVM_STATE_NUM
};

enum IO_MODE
{
	IO_IN = 0,
	IO_OUT,
	IO_MODE_NUM
};

enum IO_PUPD
{
	IO_PU = 0,
	IO_PD,
	IO_NP,
	IO_PUPD_NUM
};

enum IO_PPOD
{
	IO_PP = 0,
	IO_OD,
	IO_PPOD_NUM
};

enum IO_STATE
{
	IO_LOW = 0,
	IO_HIGH,
	IO_STATE_NUM
};

enum IO_PORT
{
	IO1 = 0,
	IO2,
	IO3,
	IO4,
	IO5,
	IO6,
	IO_NUM
};

#pragma pack(1)
typedef struct 
{
	enum TB_PORTMODE TB_PortMode;
	enum TB_WORKMODE Direction;
	uint16_t Duty;  //in spm mode,it is used to adapt different motor,in svm mode,it is use to set different duty , in dcm mode .it is used to set speed;
	uint32_t SPM_Speed;  //only for spm mode;
	uint32_t SPM_Step;
}TB_PortProperty_TypeDef;

typedef struct
{
	enum IO_STATE State;
	enum IO_MODE Mode;
	enum IO_PUPD PuPd;
	enum IO_PPOD PpOd;
}IO_Property_TypeDef;

typedef struct
{
	enum SVM_STATE State;
	uint16_t Freq;
	uint16_t Angle;
}SVM_PortProperty_TypeDef;

typedef struct 
{
	uint32_t Valid;
	
  uint32_t TB_PWM_Freq;
	
	uint8_t I2C_Address;

	TB_PortProperty_TypeDef  TB_Port[TB_PORT_NUM];
	SVM_PortProperty_TypeDef SVM_Port[SVM_PORT_NUM];
	IO_Property_TypeDef      IO_Port[IO_NUM];
	
	uint32_t FWDate;

}Config_TypeDef;

#endif

