import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import display, spi
from esphome.const import (
    CONF_ID,
    CONF_LAMBDA,
    CONF_PAGES,
)

CODEOWNERS = ["@yourusername"]
DEPENDENCIES = ["spi"]

gc9a01a_ns = cg.esphome_ns.namespace("gc9a01a_display")
GC9A01A = gc9a01a_ns.class_("GC9A01ADisplay", cg.PollingComponent, spi.SPIDevice, display.DisplayBuffer)

CONF_DC_PIN = "dc_pin"
CONF_RESET_PIN = "reset_pin"
CONF_BACKLIGHT_PIN = "backlight_pin"

GC9A01A_MODEL = "GC9A01A"

MODELS = {
    GC9A01A_MODEL: GC9A01A,
}

CONFIG_SCHEMA = cv.All(
    display.FULL_DISPLAY_SCHEMA.extend(
        {
            cv.GenerateID(): cv.declare_id(GC9A01A),
            cv.Required(CONF_DC_PIN): cv.All(cv.require_positive_int),
            cv.Optional(CONF_RESET_PIN): cv.All(cv.require_positive_int),
            cv.Optional(CONF_BACKLIGHT_PIN): cv.All(cv.require_positive_int),
        }
    )
    .extend(cv.polling_component_schema("5s"))
    .extend(spi.spi_device_schema(cs_pin_required=False)),
    cv.has_at_most_one_key(CONF_PAGES, CONF_LAMBDA),
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    await spi.register_spi_device(var, config)
    await display.register_display(var, config)

    dc = await cg.gpio_pin_expression(config[CONF_DC_PIN])
    cg.add(var.set_dc_pin(dc))

    if CONF_RESET_PIN in config:
        reset = await cg.gpio_pin_expression(config[CONF_RESET_PIN])
        cg.add(var.set_reset_pin(reset))

    if CONF_BACKLIGHT_PIN in config:
        backlight = await cg.gpio_pin_expression(config[CONF_BACKLIGHT_PIN])
        cg.add(var.set_backlight_pin(backlight))