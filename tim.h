//#ifndef __TIM_H
//#define __TIM_H		
//#include "air001xx.h"
//#include "air001xx_hal.h"
//#include "air001xx_hal_tim.h"
//#include "air001xx_it.h"
//#define HAL_TIM_MODULE_ENABLED

////void tim_init(void);
///* USER CODE BEGIN Private defines */

///* USER CODE END Private defines */
//extern TIM_HandleTypeDef htim3;
//void MX_TIM2_Init(void);

//void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

//#endif


#ifndef __TIM_H__
#define __TIM_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern TIM_HandleTypeDef htim3;

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

void MX_TIM3_Init(void);

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __TIM_H__ */

