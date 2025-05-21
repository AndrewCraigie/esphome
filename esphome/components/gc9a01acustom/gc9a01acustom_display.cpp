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
  LCD_1IN28_Clear(this->framebuffer_, WHITE);

  // Paint_NewImage(reinterpret_cast<UBYTE *>(this->framebuffer_), LCD_1IN28.WIDTH, LCD_1IN28.HEIGHT, 0, WHITE);
  // Paint_SetScale(65);
  // Paint_SetRotate(ROTATE_0);
  // Paint_Clear(WHITE);

  // LCD_1IN28_Clear(this->framebuffer_, 0x07E0);  // GREEN
}

// void GC9A01ACUSTOMDisplay::update() {
//   static uint32_t update_counter = 0;

//   if (this->framebuffer_ == nullptr) {
//     if (update_counter % 30 == 0) {
//       ESP_LOGW(TAG, "Display update skipped: framebuffer not allocated");
//     }
//     update_counter++;
//     return;
//   }

//   if (update_counter % 30 == 0) {
//     ESP_LOGD(TAG, "Display update running: framebuffer OK at %p", this->framebuffer_);

//     // SPI diagnostics
//     if (vspi != nullptr) {
//       ESP_LOGD(TAG, "SPI bus active, vspi is valid at %p", vspi);
//       uint8_t test_byte = 0xAA;
//       uint8_t response = vspi->transfer(test_byte);
//       ESP_LOGD(TAG, "SPI test transfer: wrote 0x%02X, read back 0x%02X", test_byte, response);
//     } else {
//       ESP_LOGW(TAG, "SPI bus is not initialized");
//     }

//     // I2C diagnostics
//     Wire.beginTransmission(0x00);  // Use an address unlikely to ACK
//     uint8_t error = Wire.endTransmission();
//     if (error == 0x00 || error == 0x02 || error == 0x03) {
//       ESP_LOGD(TAG, "I2C bus active: endTransmission returned %d", error);
//     } else {
//       ESP_LOGW(TAG, "I2C bus possibly uninitialized: endTransmission returned %d", error);
//     }
//   }

//   update_counter++;
// }

void GC9A01ACUSTOMDisplay::update() {
  static bool diag_run = false;
  static uint32_t update_counter = 0;

  if (this->framebuffer_ == nullptr) {
    if (update_counter % 30 == 0) {
      ESP_LOGW(TAG, "Display update skipped: framebuffer not allocated");
    }
    update_counter++;
    return;
  }

  if (update_counter % 30 == 0) {
    ESP_LOGD(TAG, "Display update running: framebuffer OK at %p", this->framebuffer_);
  }

  // SPI and I2C sanity checks
  if (vspi != nullptr) {
    ESP_LOGD(TAG, "SPI bus active, vspi is valid at %p", vspi);
    uint8_t test = vspi->transfer(0xAA);
    ESP_LOGD(TAG, "SPI test transfer: wrote 0xAA, read back 0x%02X", test);
  }

  Wire.beginTransmission(0x3C);  // Random typical I2C OLED addr
  uint8_t err = Wire.endTransmission();
  ESP_LOGD(TAG, "I2C bus active: endTransmission returned %d", err);

  if (!diag_run) {
    // Step 1: Fill framebuffer with WHITE (0xFFFF)
    for (int i = 0; i < LCD_1IN28_HEIGHT * LCD_1IN28_WIDTH; i++) {
      this->framebuffer_[i] = 0xFFFF;
    }
    ESP_LOGD(TAG, "Frame buffer manually filled with WHITE (0xFFFF)");

    // Step 2: Set display window
    LCD_1IN28_SetWindows(0, 0, LCD_1IN28_WIDTH, LCD_1IN28_HEIGHT);
    ESP_LOGD(TAG, "Set window to full screen");

    // Step 3: Set to data mode
    DEV_Digital_Write(LCD_DC_PIN, 1);
    ESP_LOGD(TAG, "LCD_DC set to data mode (1)");

    // Step 4: Push single scanline
    int y = 0;
    DEV_SPI_Write_nByte((uint8_t *)&this->framebuffer_[y * LCD_1IN28_WIDTH], LCD_1IN28_WIDTH * 2);
    ESP_LOGD(TAG, "Transmitted scanline %d", y);

    diag_run = true;
  }

  update_counter++;
}



void GC9A01ACUSTOMDisplay::dump_config() { ESP_LOGCONFIG(TAG, "GC9A01ACUSTOM display configuration:"); }

void GC9A01ACUSTOMDisplay::draw_absolute_pixel_internal(int x, int y, Color color) {
  // Stub implementation - replace with real drawing logic
}

}  // namespace gc9a01acustom
}  // namespace esphome
