#include "esphome.h"
using namespace esphome;

class ModbusRelayCommand : public Component {
 public:
  modbus::ModbusController *controller;

  void setup() override {
    if (controller == nullptr) {
      ESP_LOGE("modbus_relay", "Modbus controller not set.");
    }
  }

  void send_coil_command(uint8_t slave_address, uint16_t coil_address, bool on) {
    if (controller == nullptr) return;

    std::vector<uint8_t> frame;
    frame.push_back((coil_address >> 8) & 0xFF); // High byte
    frame.push_back(coil_address & 0xFF);        // Low byte
    frame.push_back(on ? 0xFF : 0x00);           // ON = 0xFF00, OFF = 0x0000
    frame.push_back(0x00);

    controller->queue_modbus_command(slave_address, 0x05, frame, 4);
    ESP_LOGI("modbus_relay", "Sent coil command to slave 0x%02X coil 0x%04X [%s]",
             slave_address, coil_address, on ? "ON" : "OFF");
  }
};