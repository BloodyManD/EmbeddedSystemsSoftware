/**********************************************************************
 *
 * Function:    sevenseg_set_hex()
 *
 * Description: Decode hex number to format for 7-seg display.
 *              Also: sends data to the 7-seg display PIO
 *
 * Notes:       
 *
 * Returns:     The 7-seg-decoded values of a hex digit input.
 *
 **********************************************************************/
static void sevenseg_set_hex(int hex)
{
  static alt_u8 segments[16] = {
    0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90, /* 0-9 */
    0x88, 0x83, 0xC6, 0xA1, 0x86, 0x8E };                       /* a-f */

  unsigned int data = segments[hex & 15] | (segments[(hex >> 4) & 15] << 8);

  IOWR_ALTERA_AVALON_PIO_DATA(SEVEN_SEG_PIO_BASE, data);
}
