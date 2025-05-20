#include "gc9a01acustom_display.h"
#include "LCD_1in28.h"
#include "esphome/core/log.h"

namespace esphome {
namespace gc9a01acustom {

void GC9A01ACUSTOMDisplay::setup() {
  // Initialization logic for the display
  ESP_LOGI("custom_1in28_display", "Initializing display...");
  LCD_1IN28_Init(HORIZONTAL);
  LCD_1IN28_Clear(0x07E0);  // GREEN in RGB565
}

void GC9A01ACUSTOMDisplay::update() {
  // Fill the entire display with green
  // Nothing dynamic yet
}

void GC9A01ACUSTOMDisplay::dump_config() {
  ESP_LOGCONFIG("custom_1in28_display", "Custom 1.28 inch display");
}

}  // namespace gc9a01acustom
}  // namespace esphome