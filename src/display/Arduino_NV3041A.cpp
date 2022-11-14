#include "Arduino_NV3041A.h"
#include "SPI.h"

Arduino_NV3041A::Arduino_NV3041A(
    Arduino_DataBus *bus, int8_t rst, uint8_t r,
    bool ips, int16_t w, int16_t h,
    uint8_t col_offset1, uint8_t row_offset1, uint8_t col_offset2, uint8_t row_offset2)
    : Arduino_TFT(bus, rst, r, ips, w, h, col_offset1, row_offset1, col_offset2, row_offset2)
{
}

void Arduino_NV3041A::begin(int32_t speed)
{
  Arduino_TFT::begin(speed);
}

/**************************************************************************/
/*!
    @brief   Set origin of (0,0) and orientation of TFT display
    @param   m  The index for rotation, from 0-3 inclusive
*/
/**************************************************************************/
void Arduino_NV3041A::setRotation(uint8_t r)
{
  Arduino_TFT::setRotation(r);
  switch (_rotation)
  {
  case 1:
    r = NV3041A_MADCTL_MX | NV3041A_MADCTL_MV | NV3041A_MADCTL_RGB;
    break;
  case 2:
    r = NV3041A_MADCTL_MX | NV3041A_MADCTL_MY | NV3041A_MADCTL_RGB;
    break;
  case 3:
    r = NV3041A_MADCTL_MY | NV3041A_MADCTL_MV | NV3041A_MADCTL_RGB;
    break;
  default: // case 0:
    r = NV3041A_MADCTL_RGB;
    break;
  }
  _bus->beginWrite();
  _bus->writeCommand(NV3041A_MADCTL);
  _bus->write(r);
  _bus->endWrite();
}

void Arduino_NV3041A::writeAddrWindow(int16_t x, int16_t y, uint16_t w, uint16_t h)
{
  if ((x != _currentX) || (w != _currentW))
  {
    _currentX = x;
    _currentW = w;
    x += _xStart;
    _bus->writeC8D16D16Split(NV3041A_CASET, x, x + w - 1);
  }

  if ((y != _currentY) || (h != _currentH))
  {
    _currentY = y;
    _currentH = h;
    y += _yStart;
    _bus->writeC8D16D16Split(NV3041A_RASET, y, y + h - 1);
  }

  _bus->writeCommand(NV3041A_RAMWR); // write to RAM
}

void Arduino_NV3041A::invertDisplay(bool i)
{
  // _bus->sendCommand(_ips ? (i ? NV3041A_INVOFF : NV3041A_INVON) : (i ? NV3041A_INVON : NV3041A_INVOFF));
}

void Arduino_NV3041A::displayOn(void)
{
  _bus->sendCommand(NV3041A_SLPOUT);
  delay(NV3041A_SLPOUT_DELAY);
}

void Arduino_NV3041A::displayOff(void)
{
  _bus->sendCommand(NV3041A_SLPIN);
  delay(NV3041A_SLPIN_DELAY);
}

// Companion code to the above tables.  Reads and issues
// a series of LCD commands stored in PROGMEM byte array.
void Arduino_NV3041A::tftInit()
{
  if (_rst != GFX_NOT_DEFINED)
  {
    pinMode(_rst, OUTPUT);
    digitalWrite(_rst, HIGH);
    delay(100);
    digitalWrite(_rst, LOW);
    delay(NV3041A_RST_DELAY);
    digitalWrite(_rst, HIGH);
    delay(NV3041A_RST_DELAY);
  }
  else
  {
    // Software Rest
    _bus->sendCommand(NV3041A_SWRESET);
    delay(NV3041A_RST_DELAY);
  }

  _bus->sendCommand(0xff);
  _bus->sendData(0xa5);

  _bus->sendCommand(0xE7); // TE_output_en
  _bus->sendData(0x10);

  _bus->sendCommand(0x35); // TE_ interface_en
  _bus->sendData(0x00);    // 01

  _bus->sendCommand(0x36); //
  _bus->sendData(0xc0);

  _bus->sendCommand(0x3A); //
  _bus->sendData(0x01);    // 01---565，00---666

  _bus->sendCommand(0x40);
  _bus->sendData(0x01); // 01 IPS, 00 TN

  _bus->sendCommand(0x41);
  _bus->sendData(0x03); // 01--8bit, 03-16bit

  _bus->sendCommand(0x44); // VBP  ?????
  _bus->sendData(0x15);    // 21

  _bus->sendCommand(0x45); // VFP  ?????
  _bus->sendData(0x15);    // 21

  _bus->sendCommand(0x7d); // vdds_trim[2:0]
  _bus->sendData(0x03);

  _bus->sendCommand(0xc1); // avdd_clp_en avdd_clp[1:0] avcl_clp_en avcl_clp[1:0]
  _bus->sendData(0xbb);    // 0xbb	 88		  a2

  _bus->sendCommand(0xc2); // vgl_clp_en vgl_clp[2:0]
  _bus->sendData(0x05);

  _bus->sendCommand(0xc3); // vgl_clp_en vgl_clp[2:0]
  _bus->sendData(0x10);

  _bus->sendCommand(0xc6); // avdd_ratio_sel avcl_ratio_sel vgh_ratio_sel[1:0] vgl_ratio_sel[1:0]
  _bus->sendData(0x3e);    // 35

  _bus->sendCommand(0xc7); // mv_clk_sel[1:0] avdd_clk_sel[1:0] avcl_clk_sel[1:0]
  _bus->sendData(0x25);    // 2e

  _bus->sendCommand(0xc8); //	VGL_CLK_sel
  _bus->sendData(0x11);    //

  _bus->sendCommand(0x7a); //	user_vgsp
  _bus->sendData(0x5f);    // 4f:0.8V		3f:1.04V	5f

  _bus->sendCommand(0x6f); //	user_gvdd
  _bus->sendData(0x44);    // 1C:5.61	  5f	 53		   2a	    3a

  _bus->sendCommand(0x78); //	user_gvcl
  _bus->sendData(0x70);    // 50:-3.22	  75			58	     	66

  _bus->sendCommand(0xc9); //
  _bus->sendData(0x00);

  _bus->sendCommand(0x67); //
  _bus->sendData(0x21);

  // gate_ed

  _bus->sendCommand(0x51); // gate_st_o[7:0]
  _bus->sendData(0x0a);

  _bus->sendCommand(0x52); // gate_ed_o[7:0]
  _bus->sendData(0x76);    // 76

  _bus->sendCommand(0x53); // gate_st_e[7:0]
  _bus->sendData(0x0a);    // 76

  _bus->sendCommand(0x54); // gate_ed_e[7:0]
  _bus->sendData(0x76);
  ////sorce
  _bus->sendCommand(0x46); // fsm_hbp_o[5:0]
  _bus->sendData(0x0a);

  _bus->sendCommand(0x47); // fsm_hfp_o[5:0]
  _bus->sendData(0x2a);

  _bus->sendCommand(0x48); // fsm_hbp_e[5:0]
  _bus->sendData(0x0a);

  _bus->sendCommand(0x49); // fsm_hfp_e[5:0]
  _bus->sendData(0x1a);

  _bus->sendCommand(0x56); // src_ld_wd[1:0] src_ld_st[5:0]
  _bus->sendData(0x43);

  _bus->sendCommand(0x57); // pn_cs_en src_cs_st[5:0]
  _bus->sendData(0x42);

  _bus->sendCommand(0x58); // src_cs_p_wd[6:0]
  _bus->sendData(0x3c);

  _bus->sendCommand(0x59); // src_cs_n_wd[6:0]
  _bus->sendData(0x64);

  _bus->sendCommand(0x5a); // src_pchg_st_o[6:0]
  _bus->sendData(0x41);    // 41

  _bus->sendCommand(0x5b); // src_pchg_wd_o[6:0]
  _bus->sendData(0x3c);

  _bus->sendCommand(0x5c); // src_pchg_st_e[6:0]
  _bus->sendData(0x02);    // 02

  _bus->sendCommand(0x5d); // src_pchg_wd_e[6:0]
  _bus->sendData(0x3c);    // 3c

  _bus->sendCommand(0x5e); // src_pol_sw[7:0]
  _bus->sendData(0x1f);

  _bus->sendCommand(0x60); // src_op_st_o[7:0]
  _bus->sendData(0x80);

  _bus->sendCommand(0x61); // src_op_st_e[7:0]
  _bus->sendData(0x3f);

  _bus->sendCommand(0x62); // src_op_ed_o[9:8] src_op_ed_e[9:8]
  _bus->sendData(0x21);

  _bus->sendCommand(0x63); // src_op_ed_o[7:0]
  _bus->sendData(0x07);

  _bus->sendCommand(0x64); // src_op_ed_e[7:0]
  _bus->sendData(0xe0);

  _bus->sendCommand(0x65); // chopper
  _bus->sendData(0x02);

  _bus->sendCommand(0xca); // avdd_mux_st_o[7:0]
  _bus->sendData(0x20);

  _bus->sendCommand(0xcb); // avdd_mux_ed_o[7:0]
  _bus->sendData(0x52);    // 52

  _bus->sendCommand(0xcc); // avdd_mux_st_e[7:0]
  _bus->sendData(0x10);

  _bus->sendCommand(0xcD); // avdd_mux_ed_e[7:0]
  _bus->sendData(0x42);

  _bus->sendCommand(0xD0); // avcl_mux_st_o[7:0]
  _bus->sendData(0x20);

  _bus->sendCommand(0xD1); // avcl_mux_ed_o[7:0]
  _bus->sendData(0x52);

  _bus->sendCommand(0xD2); // avcl_mux_st_e[7:0]
  _bus->sendData(0x10);

  _bus->sendCommand(0xD3); // avcl_mux_ed_e[7:0]
  _bus->sendData(0x42);

  _bus->sendCommand(0xD4); // vgh_mux_st[7:0]
  _bus->sendData(0x0a);

  _bus->sendCommand(0xD5); // vgh_mux_ed[7:0]
  _bus->sendData(0x32);

  /// test  mode
  _bus->sendCommand(0xf8); //
  _bus->sendData(0x03);    // 16????    10-15??????

  _bus->sendCommand(0xf9); //	  ??????
  _bus->sendData(0x20);

  // 2-1
  ////gammma  weihuan pianguangpian 0913
  _bus->sendCommand(0x80); // gam_vrp0	0					6bit
  _bus->sendData(0x00);
  _bus->sendCommand(0xA0); // gam_VRN0		 0-
  _bus->sendData(0x00);

  _bus->sendCommand(0x81); // gam_vrp1	1				   6bit
  _bus->sendData(0x07);
  _bus->sendCommand(0xA1); // gam_VRN1		 1-
  _bus->sendData(0x06);

  _bus->sendCommand(0x82); // gam_vrp2	 2					6bit
  _bus->sendData(0x02);
  _bus->sendCommand(0xA2); // gam_VRN2		 2-
  _bus->sendData(0x01);

  _bus->sendCommand(0x86); // gam_prp0	 7bit	8			7bit
  _bus->sendData(0x11);    // 33
  _bus->sendCommand(0xA6); // gam_PRN0	 	8-
  _bus->sendData(0x10);    // 2a

  _bus->sendCommand(0x87); // gam_prp1	 7bit	 40			 7bit
  _bus->sendData(0x27);    // 2d
  _bus->sendCommand(0xA7); // gam_PRN1	 	40-
  _bus->sendData(0x27);    // 2d

  _bus->sendCommand(0x83); // gam_vrp3	 61				 6bit
  _bus->sendData(0x37);
  _bus->sendCommand(0xA3); // gam_VRN3		61-
  _bus->sendData(0x37);

  _bus->sendCommand(0x84); // gam_vrp4	  62			 6bit
  _bus->sendData(0x35);
  _bus->sendCommand(0xA4); // gam_VRN4		62-
  _bus->sendData(0x35);

  _bus->sendCommand(0x85); // gam_vrp5	  63			 6bit
  _bus->sendData(0x3f);
  _bus->sendCommand(0xA5); // gam_VRN5		63-
  _bus->sendData(0x3f);
  //

  _bus->sendCommand(0x88); // gam_pkp0	  	 4			   5bit
  _bus->sendData(0x0b);    // 0b
  _bus->sendCommand(0xA8); // gam_PKN0		4-
  _bus->sendData(0x0b);    // 0b

  _bus->sendCommand(0x89); // gam_pkp1	  5					5bit
  _bus->sendData(0x14);    // 14
  _bus->sendCommand(0xA9); // gam_PKN1		5-
  _bus->sendData(0x14);    // 14

  _bus->sendCommand(0x8a); // gam_pkp2	  7					 5bit
  _bus->sendData(0x1a);    // 1a
  _bus->sendCommand(0xAa); // gam_PKN2		7-
  _bus->sendData(0x1a);    // 1a

  _bus->sendCommand(0x8b); // gam_PKP3	  10				 5bit
  _bus->sendData(0x0a);
  _bus->sendCommand(0xAb); // gam_PKN3		10-
  _bus->sendData(0x0a);

  _bus->sendCommand(0x8c); // gam_PKP4	   16				 5bit
  _bus->sendData(0x14);
  _bus->sendCommand(0xAc); // gam_PKN4		16-
  _bus->sendData(0x08);

  _bus->sendCommand(0x8d); // gam_PKP5		22				 5bit
  _bus->sendData(0x17);
  _bus->sendCommand(0xAd); // gam_PKN5		22-
  _bus->sendData(0x07);

  _bus->sendCommand(0x8e); // gam_PKP6		28				 5bit
  _bus->sendData(0x16);    // 16 change
  _bus->sendCommand(0xAe); // gam_PKN6		28-
  _bus->sendData(0x06);    // 13change

  _bus->sendCommand(0x8f); // gam_PKP7		34				  5bit
  _bus->sendData(0x1B);
  _bus->sendCommand(0xAf); // gam_PKN7		34-
  _bus->sendData(0x07);

  _bus->sendCommand(0x90); // gam_PKP8		 46				   5bit
  _bus->sendData(0x04);
  _bus->sendCommand(0xB0); // gam_PKN8		46-
  _bus->sendData(0x04);

  _bus->sendCommand(0x91); // gam_PKP9		 52					5bit
  _bus->sendData(0x0A);
  _bus->sendCommand(0xB1); // gam_PKN9		52-
  _bus->sendData(0x0A);

  _bus->sendCommand(0x92); // gam_PKP10		58					5bit
  _bus->sendData(0x16);
  _bus->sendCommand(0xB2); // gam_PKN10		58-
  _bus->sendData(0x15);

  _bus->sendCommand(0xff);
  _bus->sendData(0x00);

  _bus->sendCommand(0x11);
  _bus->sendData(0x00);

  delay(700);

  _bus->sendCommand(0x29);
  _bus->sendData(0x00);

  delay(100);

  if (_ips)
  {
    // _bus->sendCommand(NV3041A_INVON);
  }
}
