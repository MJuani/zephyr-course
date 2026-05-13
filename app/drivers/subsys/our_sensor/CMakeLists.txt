if(CONFIG_OUR_SENSOR)
    target_sources(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/our_sensor.c)
endif()

zephyr_include_directories(include)