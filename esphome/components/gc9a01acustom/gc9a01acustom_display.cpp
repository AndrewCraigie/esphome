#include "gc9a01acustom_display.h"
#include "LCD_1in28.h"
#include "esphome/core/log.h"
#include "esp_heap_caps.h"
#include "GUI_Paint.h"

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

  // LCD_1IN28_Init(HORIZONTAL);
  // Paint_NewImage(reinterpret_cast<UBYTE *>(this->framebuffer_), LCD_1IN28.WIDTH, LCD_1IN28.HEIGHT, 0, WHITE);
  // Paint_SetScale(65);
  // Paint_SetRotate(ROTATE_0);
  // Paint_Clear(WHITE);

  // LCD_1IN28_Clear(this->framebuffer_, 0x07E0);  // GREEN
}

void GC9A01ACUSTOMDisplay::update() {
  static uint32_t update_counter = 0;

  if (this->framebuffer_ == nullptr) {
    if (update_counter % 30 == 0) {  // Log only every 30th update (~30 seconds if 1 Hz)
      ESP_LOGW(TAG, "Display update skipped: framebuffer not allocated");
    }
    update_counter++;
    return;
  }

  if (update_counter % 30 == 0) {
    ESP_LOGD(TAG, "Display update running: framebuffer OK at %p", this->framebuffer_);
  }
  update_counter++;
}

void GC9A01ACUSTOMDisplay::dump_config() {
  ESP_LOGCONFIG(TAG, "GC9A01ACUSTOM display configuration:");
}

void GC9A01ACUSTOMDisplay::draw_absolute_pixel_internal(int x, int y, Color color) {
  // Stub implementation - replace with real drawing logic
}


}  // namespace gc9a01acustom
}  // namespace esphome
