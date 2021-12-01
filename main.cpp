#include <main.h>

/**
 * Interrupt-based blinky with extra serial hello world goodness
 */
int main()
{
  clocksetup();
  usartsetup();
  ledsetup();

  char str[] = "Hello World!\n\r";
  usart_print(str);

  while(true);

  return 0;
}

// This isr gets called everytime TIM3 overflows
void tim6_dacunder_isr()
{
  timer_clear_flag(TIM6, TIM_SR_UIF);
  // Toggle PB3
  gpio_toggle(GPIOB, GPIO3);
  // Clear interrupt flag
}

void clocksetup()
{
  // Set up the PLL to generate 72MHz from the external oscillator
  // Switch from 4MHz to 80MHz
  rcc_clock_setup_pll(&rcc_hsi16_configs[RCC_CLOCK_VRANGE1_80MHZ]);
  // Enable the alternate function clock

}

void ledsetup()
{
  // RCC_GPIOB needs to be enabled in order to do anything
  rcc_periph_clock_enable(RCC_GPIOB);
  // Configure PB3 as a push-pull output
  gpio_mode_setup(GPIOB, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO3);

  // Enable TIM3 clock
  rcc_periph_clock_enable(RCC_TIM6);
  // Set prescaler to 2kHz
  timer_set_prescaler(TIM6, 40000);
  // Set timer period to 1s
  timer_set_period(TIM6, 2000);
  timer_continuous_mode(TIM6);
  // Configure TIM6 to call an interrupt
  timer_enable_irq(TIM6, TIM_DIER_UIE);
  // Enable the interrupt source and start the timer
  nvic_enable_irq(NVIC_TIM6_DACUNDER_IRQ);
  timer_enable_counter(TIM6);
}

void usartsetup(void)
{
  // Enable USART1 clock
  rcc_periph_clock_enable(RCC_USART2);

  // Setup GPIO pin GPIO_USART1_TX on GPIO port A for transmit.
  rcc_periph_clock_enable(RCC_GPIOA);
  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO15);
  gpio_set_af(GPIOA, GPIO_AF7, GPIO2 | GPIO15);


  // Setup UART parameters.
  usart_set_baudrate(USART2, 115200);
  usart_set_databits(USART2, 8);
  usart_set_stopbits(USART2, USART_STOPBITS_1);
  usart_set_parity(USART2, USART_PARITY_NONE);
  usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);
  usart_set_mode(USART2, USART_MODE_TX);

  // Finally enable the USART
  usart_enable(USART2);
}

int usart_print(const char str[])
{
  int i;
  for(i = 0; str[i]!=0;i++)
  {
    usart_send_blocking(USART2, str[i]);
  }

  return i;
}
