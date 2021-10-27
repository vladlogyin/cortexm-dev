#include <main.h>

int main()
{
  __asm(".global __use_no_semihosting\n\t");
  clocksetup();
  gpiosetup();
  while(true)
  {
    gpio_set(GPIOB, GPIO12);
    delay_ms(1000);
    gpio_clear(GPIOB, GPIO12);
    delay_ms(1000);
  }
  //i2csetup();
  //usartsetup();
  //spisetup();
  //printf("Hello World!\n\r");

  return 0;
}

void clocksetup()
{
  rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);

  delay_setup();

  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_GPIOC);
  
  rcc_periph_clock_enable(RCC_AFIO);
  rcc_periph_clock_enable(RCC_SPI1);
  rcc_periph_clock_enable(RCC_I2C2);
  rcc_periph_clock_enable(RCC_USART1);

  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO5 | GPIO7);
  gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO6);
}
void i2csetup()
{
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
		      GPIO_I2C2_SCL | GPIO_I2C2_SDA);
  i2c_peripheral_disable(I2C2);

	/* APB1 is running at 36MHz. */
  i2c_set_clock_frequency(I2C2, 36);
  /* 400KHz - I2C Fast Mode */
  i2c_set_fast_mode(I2C2);
  /*
	 * fclock for I2C is 36MHz APB2 -> cycle time 28ns, low time at 400kHz
	 * incl trise -> Thigh = 1600ns; CCR = tlow/tcycle = 0x1C,9;
	 * Datasheet suggests 0x1e.
	 */
	i2c_set_ccr(I2C2, 0x1e);
	/*
	 * fclock for I2C is 36MHz -> cycle time 28ns, rise time for
	 * 400kHz => 300ns and 100kHz => 1000ns; 300ns/28ns = 10;
	 * Incremented by 1 -> 11.
	 */
	i2c_set_trise(I2C2, 0x0b);

	/*
	 * This is our slave address - needed only if we want to receive from
	 * other masters.
	 */
	i2c_set_own_7bit_slave_address(I2C2, 0x32);

	/* If everything is configured -> enable the peripheral. */
	i2c_peripheral_enable(I2C2);
}
void gpiosetup()
{
  
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO8);
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO9);
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
}
void spisetup()
{
  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO5 | GPIO7);
  gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO6);
}
void rgb(uint8_t r,uint8_t g,uint8_t b)
{
	if(r) gpio_set(GPIOC, GPIO13);
	else gpio_clear(GPIOC, GPIO13);
	if(g) gpio_set(GPIOB, GPIO9);
	else gpio_clear(GPIOB, GPIO9);
	if(b) gpio_set(GPIOB, GPIO8);
	else gpio_clear(GPIOB, GPIO8);
}
void usartsetup(void)
{
	/* Setup GPIO pin GPIO_USART1_RE_TX on GPIO port B for transmit. */
	gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO_USART1_TX);

	/* Setup UART parameters. */
	usart_set_baudrate(USART1, 115200);
	usart_set_databits(USART1, 8);
	usart_set_stopbits(USART1, USART_STOPBITS_1);
	usart_set_parity(USART1, USART_PARITY_NONE);
	usart_set_flow_control(USART1, USART_FLOWCONTROL_NONE);
	usart_set_mode(USART1, USART_MODE_TX);

	/* Finally enable the USART. */
	usart_enable(USART1);
}
int print(const char* str)
{
  int i;
  for (i=0;str[i]!='\0';i++)
    usart_send_blocking(USART1, str[i]);
  return i;
}
int _write(int file, char *ptr, int len)
{
	int i;
	rgb(0,1,1);
	while(1);
	if (file == 1) {
		for (i = 0; i < len; i++)
			usart_send_blocking(USART1, ptr[i]);
		return i;
	}

	errno = EIO;
	return -1;
}
void _putchar( char c)
{	
	//__asm__("BKPT");
	usart_send_blocking(USART1, c);
}
