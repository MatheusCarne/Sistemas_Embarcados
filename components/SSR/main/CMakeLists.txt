idf_component_register(SRCS "main.c" 
                       "../components/servo_tools/servo_tools.c" 
                       "../components/servo_hw/servo_hw.c"
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES servo_tools servo_hw)
