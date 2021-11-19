#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/cm3/nvic.h>

//#include <lib/systemutils.cpp>

void clock_setup();
void pwm_setup();
void adc_setup();

int main()
{
  clock_setup();
  //delay_setup();
  pwm_setup();
  adc_setup();



  while(true)
  {
    __asm("nop");
    //delay_ms(100);
  }

  return 0;
}

void clock_setup()
{
  rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);
}

void pwm_setup()
{
  rcc_periph_clock_enable(RCC_GPIOA); // PA1
  rcc_periph_clock_enable(RCC_AFIO);
  rcc_periph_clock_enable(RCC_TIM2);
  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL,GPIO_TIM2_CH1_ETR);


  timer_set_mode(TIM2, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);

  timer_set_prescaler(TIM2, rcc_apb1_frequency/10000-1);

  timer_set_period(TIM2, 0xFFFF);
  timer_continuous_mode(TIM2);
  timer_enable_counter(TIM2);

  timer_enable_oc_preload(TIM2, TIM_OC1);

  timer_set_oc_value(TIM2,TIM_OC1,0x7FFF);

  timer_set_oc_mode(TIM2, TIM_OC1, TIM_OCM_PWM1);

  timer_enable_oc_output(TIM2, TIM_OC1);
}

void adc_setup()
{

}
