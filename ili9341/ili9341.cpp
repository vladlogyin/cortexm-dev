#include <ili9341/ili9341.h>

const uint8_t ili9341::uppercaseFont[26][5]={{0b00111111,0b01000100,0b01000100,0b01000100,0b00111111}, // A
                                       {0b01111111,0b01001001,0b01001001,0b01001001,0b00110110}, // B
                                       {0b00111110,0b01000001,0b01000001,0b01000001,0b00100010}, // C
                                       {0b01111111,0b01000001,0b01000001,0b00100010,0b00011100}, // D
                                       {0b01111111,0b01001001,0b01001001,0b01001001,0b01000001}, // E
                                       {0b01111111,0b01001000,0b01001000,0b01001000,0b01000000}, // F
                                       {0b00111110,0b01000001,0b01001001,0b01001001,0b00101110}, // G
                                       {0b01111111,0b00001000,0b00001000,0b00001000,0b01111111}, // H
                                       {0b00000000,0b01000001,0b01111111,0b01000001,0b00000000}, // I
                                       {0b00000010,0b00000001,0b01000001,0b01111110,0b01000000}, // J
                                       {0b01111111,0b00001000,0b00010100,0b00100010,0b01000001}, // K
                                       {0b01111111,0b00000001,0b00000001,0b00000001,0b00000001}, // L
                                       {0b01111111,0b00100000,0b00011000,0b00100000,0b01111111}, // M
                                       {0b01111111,0b00010000,0b00001000,0b00000100,0b01111111}, // N
                                       {0b00111110,0b01000001,0b01000001,0b01000001,0b00111110}, // O
                                       {0b01111111,0b01001000,0b01001000,0b01001000,0b00110000}, // P
                                       {0b00111110,0b01000001,0b01000101,0b01000010,0b00100101}, // Q
                                       {0b01111111,0b01001000,0b01001100,0b01001010,0b00110001}, // R
                                       {0b00110001,0b01001001,0b01001001,0b01001001,0b01000110}, // S
                                       {0b01000000,0b01000000,0b01111111,0b01000000,0b01000000}, // T
                                       {0b01111110,0b00000001,0b00000001,0b00000001,0b01111110}, // U
                                       {0b01111100,0b00000010,0b00000001,0b00000010,0b01111100}, // V
                                       {0b01111110,0b00000001,0b00001110,0b00000001,0b01111110}, // W
                                       {0b01100011,0b00010100,0b00001000,0b00010100,0b01100011}, // X
                                       {0b01110000,0b00001000,0b00000111,0b00001000,0b01110000}, // Y
                                       {0b01000011,0b01000101,0b01001001,0b01010001,0b01100001}  // Z
                                      };

void ili9341::drawString(char *str, uint16_t x, uint16_t y, uint8_t s, uint16_t c)
{
  uint16_t x1=0;
  while(*str){
    if(*str=='\n')
    {
      x1=0;
      y+=8*s;
    }
    else
    {
      drawFont(*str,x+x1,y,s,c);
      x1+=6*s;
    }
    str++;
  }
}

void ili9341::drawFont(uint8_t ch, uint16_t x, uint16_t y, uint8_t s, uint16_t c)
{
  for(int x1=0;x1<5;x1++)
  {
    uint8_t y1=0;
    uint8_t b=uppercaseFont[ch-'A'][x1];
    while(b){
    if(b&0x1){fillRectangle(x+x1*s, y-y1-s, x+x1*s+s-1, y-y1-1,c);}
    b=b>>1;
    y1+=s;
    }
  }
}

void ili9341::drawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t c)
{ 
  if(x1 == x2 || y1 == y2)
  {
    // Sort points
    uint16_t minx = std::min(x1,x2), miny = std::min(y1,y2);
    uint16_t maxx = std::max(x1,x2), maxy = std::max(y1,y2);
    fillRectangle(minx,miny,maxx,maxy,c);
    return;
  }
  //TODO optimize by looking for dx<dy and using vertical lines
  int16_t dx=x2-x1, dy=y2-y1;
  if(dx>dy)
  {
    for(uint16_t x=x1;x<=x2;x++)
    {
    	writePixel(x,y1+dy*(x-x1)/dx,c);
    }
  }
  else
  {
    for(uint16_t y=y1;y<=y2;y++)
    {
    	writePixel(x1+dx*(y-y1)/dy,y,c);
    }
  }
  
}

void ili9341::setRotation(uint8_t rotation) 
{
writeCommand(0x36);
robust_delay(1);
	
switch(rotation) 
	{
		case SCREEN_VERTICAL_1:
			writeData(0x40|0x08);
			width = 240;
			height = 320;
			break;
		case SCREEN_HORIZONTAL_1:
			writeData(0x20|0x08);
			width  = 320;
			height = 240;
			break;
		case SCREEN_VERTICAL_2:
			writeData(0x80|0x08);
			width  = 240;
			height = 320;
			break;
		case SCREEN_HORIZONTAL_2:
			writeData(0x40|0x80|0x20|0x08);
			width  = 320;
			height = 240;
			break;
		default:
			break;
	}
}

ili9341::ili9341()
{

}

void ili9341::init()
{

  // Enable SPI1 peripheral and GPIO clocks
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_AFIO);
  rcc_periph_clock_enable(RCC_SPI1);
  // Configure GPIOs
  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO5 | GPIO7);
  gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO3 | GPIO2 | GPIO1); //register/data selection, reset pin, CS pin
  gpio_set_mode(GPIOA, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, GPIO6);
  
  // Reset SPI, SPI_CR1 register cleared, SPI is disabled
  spi_reset(SPI1);
  
  spi_init_master(SPI1, SPI_CR1_BAUDRATE_FPCLK_DIV_2, SPI_CR1_CPOL_CLK_TO_0_WHEN_IDLE,
                  SPI_CR1_CPHA_CLK_TRANSITION_1, SPI_CR1_DFF_8BIT, SPI_CR1_MSBFIRST);
  //spi_set_standard_mode(SPI1,0);
  spi_enable_software_slave_management(SPI1);
  spi_set_nss_high(SPI1);
  
  // Enable SPI1 peripheral
  spi_enable(SPI1);
  
  // Hardware reset
  gpio_set(GPIOA, GPIO2);
  robust_delay(100);
  gpio_clear(GPIOA, GPIO2);
  robust_delay(100);
  gpio_set(GPIOA, GPIO2);
  //// Software reset
  //gpio_clear(GPIOA, GPIO3);
  //spi_xfer(SPI1,ILI9341_SWRESET);
  robust_delay(100);
  
  for(int i=0;initcmd[i];)
  {
    uint8_t cmd=initcmd[i++];
    uint8_t x = initcmd[i++];
    uint8_t numArgs = x & 0x7F;
    sendCommand(cmd,initcmd+i,numArgs);
    i+=numArgs;
    if(x & 0x80) { robust_delay(150); }
  }
  setRotation(SCREEN_VERTICAL_1);
}
void ili9341::fillScreen(uint16_t c)
{
  fillRectangle(0,0,width-1,height-1,c);
}
void ili9341::fillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t c) //x2 > x1 and y2 >y1
{
  
  if ((x1<=x2) && (y1<=y2) && (x1 >= 0) && (x2 < width) && (y1 >= 0) && (y2 < height))
  {
    setAddressWindow(x1, y1, x2, y2);
    gpio_set(GPIOA, GPIO3); // DC data high
    //robust_us(1);
    gpio_clear(GPIOA, GPIO1); // CS active low
    //robust_us(1);
    for(int i=(x2-x1+1)*(y2-y1+1);i--;)
    {
      spi_send(SPI1, c>>8); spi_send(SPI1, c); //SPI_WRITE16(c);
    }
    gpio_set(GPIOA, GPIO1); // CS inactive high
   // robust_us(1);
  }
}
void ili9341::writePixel(uint16_t x, uint16_t y, uint16_t c)
{
  if ((x >= 0) && (x < width) && (y >= 0) && (y < height)) {
    setAddressWindow(x, y, x + 1, y + 1);
    
    gpio_set(GPIOA, GPIO3); // DC data high
    robust_us(1);
    gpio_clear(GPIOA, GPIO1); // CS active low
    robust_us(1);
    spi_xfer(SPI1, c>>8); spi_xfer(SPI1, c); //SPI_WRITE16(c);
    gpio_set(GPIOA, GPIO1); // CS inactive high
    robust_us(1);
  }

}

void ili9341::setAddressWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
  writeCommand(ILI9341_CASET); // Column address set
  writeData(x1>>8); writeData(x1); //SPI_WRITE16(x1);
  writeData(x2>>8); writeData(x2); //SPI_WRITE16(x2);
  
  writeCommand(ILI9341_PASET); // Row address set
  writeData(y1>>8); writeData(y1); //SPI_WRITE16(y1);
  writeData(y2>>8); writeData(y2); //SPI_WRITE16(y2);
  
  writeCommand(ILI9341_RAMWR); // Write to RAM
}

void ili9341::sendCommand(uint8_t command,const uint8_t *data, uint8_t n)
{
  //assume 8 bit bus width
  
  gpio_clear(GPIOA, GPIO1); // CS active low
  robust_us(1);
  gpio_clear(GPIOA, GPIO3); // DC command low
  robust_us(1);
  spi_xfer(SPI1, command);
  
  gpio_set(GPIOA, GPIO3); // DC data high
  robust_us(1);
  for(int i=0;i<n;i++) {
    spi_xfer(SPI1, data[i]);
  }
  
  gpio_set(GPIOA, GPIO1); // CS inactive high
  robust_us(1);
}

uint8_t ili9341::readCommand(uint8_t command, uint8_t ind) //readCommand8
{
  uint8_t res=0;
  gpio_clear(GPIOA, GPIO3); // DC command low
  robust_us(1);
  gpio_clear(GPIOA, GPIO1); // CS active low
  robust_us(1);
  spi_xfer(SPI1, command);
  gpio_set(GPIOA, GPIO3); // DC data high
  robust_us(1);
  do {
    res=spi_read(SPI1);
  } while (ind--); // Discard bytes up to indth byte
  gpio_set(GPIOA, GPIO1); // CS inactive high
  robust_us(1);
  return res;
}
void ili9341::writeCommand(uint8_t command)
{
  gpio_clear(GPIOA, GPIO3); // DC command low
  robust_us(1);
  gpio_clear(GPIOA, GPIO1); // CS active low
  robust_us(1);
  spi_xfer(SPI1, command);
  gpio_set(GPIOA, GPIO1); // CS inactive high
  robust_us(1);
}
void ili9341::writeData(uint8_t data)
{
  gpio_set(GPIOA, GPIO3); // DC data high
  robust_us(1);
  gpio_clear(GPIOA, GPIO1); // CS active low
  robust_us(1);
  spi_xfer(SPI1, data);
  gpio_set(GPIOA, GPIO1); // CS inactive high
  robust_us(1);
}



const uint8_t ili9341::initcmd[] = {
  0xEF, 3, 0x03, 0x80, 0x02,
  0xCF, 3, 0x00, 0xC1, 0x30,
  0xED, 4, 0x64, 0x03, 0x12, 0x81,
  0xE8, 3, 0x85, 0x00, 0x78,
  0xCB, 5, 0x39, 0x2C, 0x00, 0x34, 0x02,
  0xF7, 1, 0x20,
  0xEA, 2, 0x00, 0x00,
  ILI9341_PWCTR1  , 1, 0x23,             // Power control VRH[5:0]
  ILI9341_PWCTR2  , 1, 0x10,             // Power control SAP[2:0];BT[3:0]
  ILI9341_VMCTR1  , 2, 0x3e, 0x28,       // VCM control
  ILI9341_VMCTR2  , 1, 0x86,             // VCM control2
  ILI9341_MADCTL  , 1, 0x48,             // Memory Access Control
  ILI9341_VSCRSADD, 1, 0x00,             // Vertical scroll zero
  ILI9341_PIXFMT  , 1, 0x55,
  ILI9341_FRMCTR1 , 2, 0x00, 0x18,
  ILI9341_DFUNCTR , 3, 0x08, 0x82, 0x27, // Display Function Control
  0xF2, 1, 0x00,                         // 3Gamma Function Disable
  ILI9341_GAMMASET , 1, 0x01,             // Gamma curve selected
  ILI9341_GMCTRP1 , 15, 0x0F, 0x31, 0x2B, 0x0C, 0x0E, 0x08, // Set Gamma
    0x4E, 0xF1, 0x37, 0x07, 0x10, 0x03, 0x0E, 0x09, 0x00,
  ILI9341_GMCTRN1 , 15, 0x00, 0x0E, 0x14, 0x03, 0x11, 0x07, // Set Gamma
    0x31, 0xC1, 0x48, 0x08, 0x0F, 0x0C, 0x31, 0x36, 0x0F,
  ILI9341_SLPOUT  , 0x80,                // Exit Sleep
  ILI9341_DISPON  , 0x80,                // Display on
  0x00                                   // End of list
};

uint16_t ili9341::rgb565(uint32_t rgb)
{
  return ((rgb & 0x00F80000) >> 8) | ((rgb & 0x0000FC00) >> 5) | ((rgb & 0x0000F8)>>3);
}
uint16_t ili9341::rgb565(uint8_t r, uint8_t g, uint8_t b)
{
  return ((r & 0xF8) <<8) | ((g & 0xFC) <<3) | (b >>3);
}
