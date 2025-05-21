#include "gc9a01acustom_display.h"
#include "LCD_1in28.h"
#include "esphome/core/log.h"
#include "esp_heap_caps.h"
#include "GUI_Paint.h"

extern SPIClass *vspi;

namespace esphome {
namespace gc9a01acustom {

static const char *const TAG = "gc9a01acustom";

void GC9A01ACUSTOMDisplay::setup() {
  ESP_LOGCONFIG(TAG, "Setting up GC9A01ACUSTOM display...");

  this->image_size_ = LCD_1IN28_HEIGHT * LCD_1IN28_WIDTH * 2;
  this->framebuffer_ = static_cast<uint16_t *>(heap_caps_malloc(this->image_size_, MALLOC_CAP_DMA));
  if (this->framebuffer_ == nullptr) {
    ESP_LOGE(TAG, "Failed to allocate framebuffer");
    this->mark_failed();
    return;
  }

  if (DEV_Module_Init() != 0) {
    ESP_LOGE(TAG, "DEV_Module_Init failed");
    this->mark_failed();
    return;
  }
  ESP_LOGD(TAG, "DEV_Module_Init succeeded");

  LCD_1IN28_Init(HORIZONTAL);
  // Paint_NewImage(reinterpret_cast<UBYTE *>(this->framebuffer_), LCD_1IN28.WIDTH, LCD_1IN28.HEIGHT, 0, WHITE);
  // Paint_SetScale(65);
  // Paint_SetRotate(ROTATE_0);
  // Paint_Clear(WHITE);

  // LCD_1IN28_Clear(this->framebuffer_, 0x07E0);  // GREEN
}

void GC9A01ACUSTOMDisplay::update() {
  static uint32_t update_counter = 0;

  if (this->framebuffer_ == nullptr) {
    if (update_counter % 30 == 0) {  // Log every 30th update (~30s if 1 Hz)
      ESP_LOGW(TAG, "Display update skipped: framebuffer not allocated");
    }
    update_counter++;
    return;
  }

  if (update_counter % 30 == 0) {
    ESP_LOGD(TAG, "Display update running: framebuffer OK at %p", this->framebuffer_);

    if (vspi == nullptr) {
      ESP_LOGW(TAG, "SPI not initialized (vspi is nullptr)");
    } else {
      ESP_LOGD(TAG, "SPI bus active, vspi is valid at %p", vspi);

      // Optional: test a SPI transfer
      uint8_t test_byte = 0xAA;
      uint8_t result = vspi->transfer(test_byte);
      ESP_LOGD(TAG, "SPI test transfer: wrote 0x%02X, read back 0x%02X", test_byte, result);
    }
  }

  update_counter++;
}

void GC9A01ACUSTOMDisplay::dump_config() { ESP_LOGCONFIG(TAG, "GC9A01ACUSTOM display configuration:"); }

void GC9A01ACUSTOMDisplay::draw_absolute_pixel_internal(int x, int y, Color color) {
  // Stub implementation - replace with real drawing logic
}

}  // namespace gc9a01acustom
}  // namespace esphome
