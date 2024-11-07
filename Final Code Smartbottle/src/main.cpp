#include <OneWire.h>
#include <DallasTemperature.h>
#include <HX711.h>
#include "soc/rtc.h"

// GPIO untuk DS18B20
const int oneWireBus = 4;     
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// Pin untuk HX711
const int LOADCELL_DOUT_PIN = 16;
const int LOADCELL_SCK_PIN = 4;
HX711 scale;

void setup() {
  Serial.begin(115200);

  // Konfigurasi frekuensi CPU untuk ESP32
  rtc_cpu_freq_config_t config;
  rtc_clk_cpu_freq_get_config(&config);
  rtc_clk_cpu_freq_to_config(RTC_CPU_FREQ_80M, &config);
  rtc_clk_cpu_freq_set_config_fast(&config);

  // Inisialisasi sensor DS18B20
  sensors.begin();

  // Inisialisasi HX711
  Serial.println("Inisialisasi HX711");
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale(-471.497);  // Set nilai kalibrasi (sesuaikan dengan kebutuhan)
  scale.tare();               // Reset skala ke 0

  Serial.println("Pengaturan selesai.");
}

void loop() {
  // Pembacaan Suhu dari DS18B20
  sensors.requestTemperatures(); 
  float temperatureC = sensors.getTempCByIndex(0);
  float temperatureF = sensors.getTempFByIndex(0);
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.print(" ºC / ");
  Serial.print(temperatureF);
  Serial.println(" ºF");

  // Pembacaan Berat dari HX711
  Serial.print("Berat (satuan): ");
  Serial.print(scale.get_units(), 1);
  Serial.print("\t| Rata-rata: ");
  Serial.println(scale.get_units(10), 5);

  // Tidurkan HX711 untuk hemat daya
  scale.power_down();
  delay(5000);
  scale.power_up();
}
