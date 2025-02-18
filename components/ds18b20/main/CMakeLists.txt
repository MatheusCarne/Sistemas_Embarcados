idf_component_register(SRCS "main.c" 
                       "../components/ssr/ssr.c" 
                       INCLUDE_DIRS "."
                       PRIV_REQUIRES ds18b20)
