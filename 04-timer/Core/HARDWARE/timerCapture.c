//
// Created by 86156 on 2024/6/2.
//

#include "timerCapture.h"

TIM_HandleTypeDef tim4HandleTypeDef;




/**
 * ��ʱ�� 4 ���벶������
 * @param arr �Զ�װ��ֵ
 * @param psc ʱ��Ԥ��Ƶ��
 */
void Tim4_Init(u16 arr, u16 psc)
{
    TIM_IC_InitTypeDef timIcInitTypeDef;

    // ��ʼ���������ʱ������
    tim4HandleTypeDef.Instance = TIM4;
    tim4HandleTypeDef.Init.Prescaler = psc;
    tim4HandleTypeDef.Init.CounterMode = TIM_COUNTERMODE_UP;
    tim4HandleTypeDef.Init.Period = arr;
    tim4HandleTypeDef.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_IC_Init(&tim4HandleTypeDef);


    timIcInitTypeDef.ICPolarity = TIM_ICPOLARITY_RISING; // �����ز���
    timIcInitTypeDef.ICFilter = 0; // ��������˲��������˲�
    timIcInitTypeDef.ICPrescaler = TIM_ICPSC_DIV1; // ���������Ƶ������Ƶ�� ÿ�����ڶ�����
    timIcInitTypeDef.ICSelection = TIM_ICSELECTION_DIRECTTI; // ӳ�䵽 TI1 ��
    // ���� TIM4 ͨ��1
    HAL_TIM_IC_ConfigChannel(&tim4HandleTypeDef, &timIcInitTypeDef, TIM_CHANNEL_1);
    // ���������ж�
    HAL_TIM_IC_Start_IT(&tim4HandleTypeDef,TIM_CHANNEL_1);   //����TIM4�Ĳ���ͨ��1�����ҿ��������ж�
    __HAL_TIM_ENABLE_IT(&tim4HandleTypeDef, TIM_IT_UPDATE);

}

/**
 * ��ʱ��4�ײ�������ʱ��ʹ�ܣ��������ã��ж�����
 * @param htim ��ʱ�����
 */
void HAL_TIM_IC_MspInit(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4)
    {
        GPIO_InitTypeDef  gpioInitTypeDef;
        __HAL_RCC_GPIOD_CLK_ENABLE(); // ʹ�� GPIOD ʱ��
        __HAL_RCC_TIM4_CLK_ENABLE(); // ʹ�� TIM4 ʱ��

        gpioInitTypeDef.Pin = GPIO_PIN_12; // PD12
        gpioInitTypeDef.Mode = GPIO_MODE_AF_PP; // �����������
        gpioInitTypeDef.Pull = GPIO_PULLDOWN; // ����
        gpioInitTypeDef.Speed = GPIO_SPEED_HIGH; // ����
        gpioInitTypeDef.Alternate = GPIO_AF2_TIM4; // PD12 ���� TIM4 ͨ��1

        HAL_GPIO_Init(GPIOD, &gpioInitTypeDef);

        HAL_NVIC_SetPriority(TIM4_IRQn, 2 ,0); // �����ж����ȼ�����ռ���ȼ�2�� ��Ӧ���ȼ�0
        HAL_NVIC_EnableIRQ(TIM4_IRQn); // ���� TIM4 �ж�ͨ��

    }
}

/**
 * ��ʱ��4�жϺ���
 */
void TIM4_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&tim4HandleTypeDef);
}


//����״̬
//[7]:0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
//[6]:0,��û���񵽵͵�ƽ;1,�Ѿ����񵽵͵�ƽ��.
//[5:0]:����͵�ƽ������Ĵ���(����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
u8  TIM4CH1_CAPTURE_STA=0;	//���벶��״̬
u32	TIM4CH1_CAPTURE_VAL;	//���벶��ֵ(TIM4��32λ)


/**
 * ��ʱ�������жϣ���������жϻص��������ú����� HAL_TIM_IRQHandler �е���
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4)
    {
        if ((TIM4CH1_CAPTURE_STA&0x80) == 0) // ��δ�ɹ�����
        {
            if ((TIM4CH1_CAPTURE_STA&0x40)) // ���񵽸ߵ�ƽ
            {
                if ((TIM4CH1_CAPTURE_STA&0x3F) == 0X3F) // �ߵ�ƽʱ��̫��
                {
                    TIM4CH1_CAPTURE_STA|=0x80; // ��ǳɹ�����һ��
                    TIM4CH1_CAPTURE_VAL = 0XFFFF;
                } else TIM4CH1_CAPTURE_STA++;
            }
        }
    } else if (htim->Instance == TIM3)
    {
        HAL_GPIO_WritePin(GPIOF, GPIO_PIN_10, GPIO_PIN_SET);
        //LED1=!LED1; // LED1 ��ת
    }
}


/**
 * ��ʱ�����벶���жϴ���ص��������ú�����HAL_TIM_IRQHandler �лᱻ����
 * @param htim ��ʱ�����
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim) // �����жϷ���ʱִ��
{
    if (htim->Instance == TIM4)
    {
        if ((TIM4CH1_CAPTURE_STA&0x80) == 0) // ��δ�ɹ�����
        {
            if ((TIM4CH1_CAPTURE_STA&0x40)) // ����һ���½���
            {
                TIM4CH1_CAPTURE_STA|=0x80; // ��ǳɹ�����һ�θߵ�ƽ����
                // ��ȡ��ʱ��4ͨ��1�Ĳ���ֵ
                TIM4CH1_CAPTURE_VAL= HAL_TIM_ReadCapturedValue(&tim4HandleTypeDef,TIM_CHANNEL_1);
                // ���ԭ��������
                TIM_RESET_CAPTUREPOLARITY(&tim4HandleTypeDef,TIM_CHANNEL_1);
                // ����TIM4ͨ��1�����Ҳ���
                TIM_SET_CAPTUREPOLARITY(&tim4HandleTypeDef, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
            } else // ��δ��ʼ��ʱ�򣬵�һ�β���������
            {
                TIM4CH1_CAPTURE_STA = 0;
                TIM4CH1_CAPTURE_VAL = 0;
                TIM4CH1_CAPTURE_STA |= 0x40; // �༭�Ѿ�����������
                // �رն�ʱ��4
                __HAL_TIM_DISABLE(&tim4HandleTypeDef);
                __HAL_TIM_SET_COUNTER(&tim4HandleTypeDef, 0);

                // ���ԭ��������
                TIM_RESET_CAPTUREPOLARITY(&tim4HandleTypeDef,TIM_CHANNEL_1);
                // ����Ϊ�½���
                TIM_SET_CAPTUREPOLARITY(&tim4HandleTypeDef,TIM_CHANNEL_1, TIM_ICPOLARITY_FALLING);
                // ʹ�ܶ�ʱ��4
                __HAL_TIM_ENABLE(&tim4HandleTypeDef);

            }
        }
    }
}