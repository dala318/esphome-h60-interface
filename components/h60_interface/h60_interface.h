#pragma once

#include "esphome/core/component.h"
#include "esphome/components/uart/uart.h"

#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"

// #include "HD_Globals.h"
// #include "HD_Utilities.h"
// #include "HD_H1comm.h"

#include <memory>
#include <string>
#include <vector>

namespace esphome {
namespace h60_interface {

class SensorPower : public sensor::Sensor, public PollingComponent {
public:
    // void setup() override { this->publish_state(50); }
    void update() override {
        // auto ip = wifi::global_wifi_component->wifi_sta_ip();
        // if (ip != this->last_ip_) {
        //   this->last_ip_ = ip;
        //   this->publish_state(ip.str());
        // }
        float value = 5050.8;
        if (value != this->last_value_) {
            this->last_value_ = value;
            this->publish_state(value);
        }
  }
//   std::string unique_id() override { return get_mac_address() + "-wifiinfo-ip"; }
//   void dump_config() override;

protected:
    float last_value_;
//   network::IPAddress last_ip_;
};

class SensorReturnTemp : public sensor::Sensor, public PollingComponent {
public:
    // void setup() override { this->publish_state(501.88); }
    void update() override {
        float value = -701.888;
        if (value != this->last_value_) {
            this->last_value_ = value;
            this->publish_state(value);
        }
    }
protected:
    float last_value_;
};



class H60InterfaceComponent : public Component {
public:
    void setup() override;
    void loop() override;
    void dump_config() override;
    void on_shutdown() override;

    void set_model(std::string model) {this->model_ = model; }
    void set_uart_parent(esphome::uart::UARTComponent *parent) { this->stream_ = parent; }
    void set_buffer_size(size_t size) { this->buf_size_ = size; }
    
    float get_setup_priority() const override { return esphome::setup_priority::AFTER_WIFI; }

    void register_binary_sensor(binary_sensor::BinarySensor *obj) { this->binary_sensors_.push_back(obj); }
    // void register_sensor(std::string id, sensor::Sensor *obj) { this->sensors_.push_back(obj); }
    void register_text_sensor(text_sensor::TextSensor *obj) { this->text_sensors_.push_back(obj); }

    // float get_float_parameter(std::string parameter) {
    //     if (paramter == 0){
    //         return 550.88
    //     } else if (paramter == 1) {
    //         return -188.22222
    //     }
    //     return 0
    // }

protected:
    void accept();
    void cleanup();
    void read();
    void flush();
    void write();

    std::vector<binary_sensor::BinarySensor *> binary_sensors_;
    // std::vector<sensor::Sensor *> sensors_;
    std::vector<text_sensor::TextSensor *> text_sensors_;

    size_t buf_index(size_t pos) { return pos & (this->buf_size_ - 1); }
    size_t buf_ahead(size_t pos) { return (pos | (this->buf_size_ - 1)) - pos + 1; }

    // typedef enum
    // {
    //     BINARY_SENSOR
    //     SENSOR,
    //     TEXT_SENSOR,
    // } entity_type;

    // struct Parameter {
    //     Parameter(std::int64_t reg, std::string identifier, size_t position);

    //     std::int64_t reg{};
    //     std::string identifier{};
    //     // bool disconnected{false};
    //     size_t position{0};
    // };

    // std::vector<Parameter> parameters_{};

    std::string model_;
    esphome::uart::UARTComponent *stream_{nullptr};
    size_t buf_size_;

    std::unique_ptr<uint8_t[]> buf_{};
    size_t buf_head_{0};
    size_t buf_tail_{0};
};

}  // namespace h60_interface
}  // namespace esphome