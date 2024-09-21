#ifndef I2C_LCD_H
#define I2C_LCD_H

#include <Arduino.h>
#include <LiquidCrystal_I2C.h>

class I2C_LCD {
private:
  LiquidCrystal_I2C* lcd;
  uint8_t _cols;
  uint8_t _rows;
  uint8_t _custom_char_fill[8] = { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F };
  uint8_t _custom_char_empty[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  uint8_t _custom_char_left_arrow[8] = { 0x03, 0x07, 0x0f, 0x1f, 0x1f, 0x0f, 0x07, 0x03 };
  uint8_t _custom_char_right_arrow[8] = { 0x18, 0x1c, 0x1e, 0x1f, 0x1f, 0x1e, 0x1c, 0x18 };
  uint8_t _custom_char_up_arrow[8] = { 0x04, 0x0e, 0x1f, 0x1f, 0x00, 0x00, 0x00, 0x00 };
  uint8_t _custom_char_down_arrow[8] = { 0x00, 0x00, 0x00, 0x00, 0x1f, 0x1f, 0x0e, 0x04 };


public:
  I2C_LCD(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows);
  ~I2C_LCD();

  void begin();
  void show_text(const String& message, uint8_t row_number);
  void show_text_center(const String& message, uint8_t row_number);
  void test_display();
  void show_text_with_arrow(const String& message, uint8_t row_number);
  void clear_line(uint8_t row_number);
};

#endif