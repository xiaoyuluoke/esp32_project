deps_config := \
	/root/esp/esp-idf/components/app_trace/Kconfig \
	/root/esp/esp-idf/components/aws_iot/Kconfig \
	/root/esp/esp-idf/components/bt/Kconfig \
	/root/esp/esp-idf/components/esp32/Kconfig \
	/root/esp/esp-idf/components/ethernet/Kconfig \
	/root/esp/esp-idf/components/fatfs/Kconfig \
	/root/esp/esp-idf/components/freertos/Kconfig \
	/root/esp/esp-idf/components/log/Kconfig \
	/root/esp/esp-idf/components/lwip/Kconfig \
	/root/esp32_project/esp32_project/main/Kconfig \
	/root/esp/esp-idf/components/mbedtls/Kconfig \
	/root/esp/esp-idf/components/openssl/Kconfig \
	/root/esp/esp-idf/components/spi_flash/Kconfig \
	/root/esp/esp-idf/components/bootloader/Kconfig.projbuild \
	/root/esp/esp-idf/components/esptool_py/Kconfig.projbuild \
	/root/esp32_project/esp32_project/main/Kconfig.projbuild \
	/root/esp/esp-idf/components/partition_table/Kconfig.projbuild \
	/root/esp/esp-idf/Kconfig

include/config/auto.conf: \
	$(deps_config)


$(deps_config): ;
