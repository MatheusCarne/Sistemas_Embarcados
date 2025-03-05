idf_component_register(SRCS "main.c" 
                       "../ProjetoFinal/ssd1306/ssd1306.c" 
                       "../ProjetoFinal/sensor_imu/sensor_imu.c"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES imu_tools sensor_imu)
