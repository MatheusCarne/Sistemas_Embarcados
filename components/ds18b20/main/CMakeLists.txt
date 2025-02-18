idf_component_register(SRCS "main.c" 
                       "../components/ds18b20/ds18b20.c" 
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES ds18b20)
