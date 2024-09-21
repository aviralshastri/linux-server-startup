#include "I2C_LCD.h"

I2C_LCD::I2C_LCD(uint8_t lcd_addr, uint8_t lcd_cols, uint8_t lcd_rows)
  : _cols(lcd_cols), _rows(lcd_rows) {
  lcd = new LiquidCrystal_I2C(lcd_addr, lcd_cols, lcd_rows);
}

I2C_LCD::~I2C_LCD() {
  delete lcd;
}

void I2C_LCD::begin() {
  lcd->init();
  lcd->backlight();
  lcd->createChar(0, _custom_char_left_arrow);
  lcd->createChar(1, _custom_char_right_arrow);
  lcd->createChar(2, _custom_char_up_arrow);
  lcd->createChar(3, _custom_char_down_arrow);
  lcd->createChar(4, _custom_char_empty);
  lcd->createChar(5, _custom_char_fill);
}

void I2C_LCD::show_text(const String& message, uint8_t row_number) {
  if (row_number >= _rows) return;
  lcd->setCursor(0, row_number);
  lcd->print(message.substring(0, _cols));
}

void I2C_LCD::show_text_center(const String& message, uint8_t row_number) {
  if (row_number >= _rows) return;
  int padding = (_cols - message.length()) / 2;
  if (padding < 0) padding = 0;
  lcd->setCursor(padding, row_number);
  lcd->print(message.substring(0, _cols - padding));
}

void I2C_LCD::test_display() {
  lcd->clear();
  for (uint8_t row = 0; row < _rows; row++) {
    for (uint8_t col = 0; col < _cols; col++) {
      for (uint8_t chars = 0; chars < 6; chars++) {
        lcd->setCursor(col, row);
        lcd->write(chars);
        delay(300);
      }
    }
  }
  delay(200);
  for (uint8_t row = 0; row < _rows; row++) {
    for (uint8_t col = 0; col < _cols; col++) {
      lcd->setCursor(col, row);
      lcd->write(4);
      delay(200);
    }
  }
}


void I2C_LCD::show_text_with_arrow(const String& message, uint8_t row_number) {
  if (row_number >= _rows) return;
  lcd->setCursor(0, row_number);
  lcd->print('<');
  lcd->setCursor(_cols - 1, row_number);
  lcd->print('>');

  int availableSpace = _cols - 2;
  String displayText = message;
  if (message.length() > availableSpace) {
    displayText = message.substring(0, availableSpace - 2) + "..";
  }

  int padding = (availableSpace - displayText.length()) / 2;
  lcd->setCursor(1 + padding, row_number);
  lcd->print(displayText);
}

void I2C_LCD::clear_line(uint8_t row_number) {
  if (row_number >= _rows) return;
  lcd->setCursor(0, row_number);
  for (uint8_t i = 0; i < _cols; i++) {
    lcd->print(' ');
  }
}