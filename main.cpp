#include <main.h>
//#define printf(x) (void)(x)
//BMP180 myBMP(BMP180_ULTRAHIGHRES);//BMP180 
//HTU21D myHTU21D(HTU21D_RES_RH12_TEMP14);
ds3231 ds;
int main()
{
__asm(".global __use_no_semihosting\n\t");
  //init_printf(NULL,tpfputc);
  clocksetup();
  gpiosetup();
  i2csetup();
  usartsetup();
  rgb(0,1,0);  
  printf("p1\n");
  /*while (!myBMP.begin());
  {
  delay_ms(50);
  }*/
  /*while (myHTU21D.begin() != true)
  {
    delay_ms(50);
  }*/
  printf("p1.5\n");
  //lm75_write_config(I2C2, 0x48);
  printf("p2\n");
  rgb(0,1,0);
  
  while(1)
  {
  printf("p3\n");
  //printf("temp1: %.3f\n",(lm75_read_temperature(I2C2, LM75_SENSOR0)>>5)/8.0);
  //printf("temp2: %.3f\n",(lm75temp()>>5)/8.0);
  //printf("temp2: %.3f\n",myBMP.getTemperature());
  printf("time1: %d\n",ds.getSec());
  delay_ms(500);
  }
  
  return 0;
}

void clocksetup()
{
  rcc_clock_setup_in_hse_8mhz_out_72mhz();
}
void i2csetup()
{
  rcc_periph_clock_enable(RCC_I2C2);
  rcc_periph_clock_enable(RCC_AFIO);
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_50_MHZ,
		      GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN,
		      GPIO_I2C2_SCL | GPIO_I2C2_SDA);
  i2c_peripheral_disable(I2C2);

	/* APB1 is running at 36MHz. */
  i2c_set_clock_frequency(I2C2, I2C_CR2_FREQ_36MHZ);
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
  
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_GPIOC);
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO8);
  gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO9);
  gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_10_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);
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
	rcc_periph_clock_enable(RCC_GPIOA);
	rcc_periph_clock_enable(RCC_AFIO);
	rcc_periph_clock_enable(RCC_USART1);
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