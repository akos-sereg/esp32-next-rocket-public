idf_component_register(SRCS "main.c" "tm1637.c" "timer.c" "https.c" "networking.c" "response-parser.c" "max7219.c"
                    EMBED_TXTFILES server_root_cert.pem
                    INCLUDE_DIRS ".")