#pragma once

#include "esphome/components/spi/spi.h"
#include "esphome/components/display/display_buffer.h"
#include "esphome/components/display/display_color_utils.h"
#include "esphome/core/component.h"
#include "esphome/core/gpio.h"

namespace esphome {
namespace gc9a01acustom {

  // Local color mode enum to mirror ESPHome behavior
enum GC9A01ACUSTOMColorMode {
  BITS_8 = 0x08,
  BITS_8_INDEXED = 0x09,
  BITS_16 = 0x10,
};

class GC9A01ACUSTOMDisplay : public display::DisplayBuffer {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;

  display::DisplayType get_display_type() override { return display::DisplayType::DISPLAY_TYPE_COLOR; }
  int get_width_internal() override { return 240; }
  int get_height_internal() override { return 240; }

  // No-op setters to satisfy ESPHome's expectations
  void set_spi_parent(spi::SPIComponent *parent) {}
  void set_cs_pin(GPIOPin *pin) {}
  void set_dc_pin(GPIOPin *pin) {}
  void set_reset_pin(GPIOPin *pin) {}
  void set_data_rate(uint32_t data_rate) {}
  void set_buffer_color_mode(GC9A01ACUSTOMColorMode color_mode) { this->buffer_color_mode_ = color_mode; }
  void invert_colors(bool invert) {}

  void draw_absolute_pixel_internal(int x, int y, Color color) override;


  protected:
  GC9A01ACUSTOMColorMode buffer_color_mode_{BITS_16};
  uint16_t *framebuffer_{nullptr};
  size_t image_size_{0};
};

}  // namespace gc9a01acustom
}  // namespace esphome
