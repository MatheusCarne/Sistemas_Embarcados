idf_component_register(SRCS "main.c" 
                       "../components/ssd1306/ssd1306.c" 
                       "../components/sensor_imu/sensor_imu.c"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES ssd1306 sensor_imu)
