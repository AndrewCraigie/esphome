#ifndef CUSTOM_1IN28_DISPLAY_H
#define CUSTOM_1IN28_DISPLAY_H

#include "esphome/components/display/display_buffer.h"
#include "esphome/core/component.h"

namespace esphome {
namespace gc9a01acustom {

class GC9A01ACUSTOMDisplay : public display::DisplayBuffer, public Component {
 public:
  void setup() override;
  void update() override;
  void dump_config() override;

  display::DisplayType get_display_type() override { return display::DisplayType::DISPLAY_TYPE_COLOR; }
  int get_width_internal() override { return 240; }
  int get_height_internal() override { return 240; }
};

class GC9A01ACustom240TFT : public GC9A01ACUSTOMDisplay {
 public:
  GC9A01ACustom240TFT() : GC9A01ACUSTOMDisplay() {}
};

}  // namespace gc9a01acustom
}  // namespace esphome

#endif  // CUSTOM_1IN28_DISPLAY_H
