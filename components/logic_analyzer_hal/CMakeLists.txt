set( srcs 
    logic_analyzer_hal.c
)
set( includedir 
    include 
    )
set( priv_includedir 
    private_include 
    )
set( priv_require 
    driver
    esp_mm
    esp_psram 
    )

  if(IDF_TARGET STREQUAL "esp32")
    list(APPEND srcs
        esp32/logic_analyzer_ll.c      
    )
    list(APPEND priv_includedir
        esp32/private_include
    )
  endif()

  if(IDF_TARGET STREQUAL "esp32s3")
    list(APPEND srcs
        esp32s3/logic_analyzer_ll.c      
    )
    list(APPEND priv_includedir
        esp32s3/private_include
    )
  endif()

idf_component_register(
    SRCS ${srcs}
    INCLUDE_DIRS ${includedir}
    PRIV_INCLUDE_DIRS ${priv_includedir}
    #REQUIRES ${require}
    PRIV_REQUIRES ${priv_require}
    EMBED_FILES ${embed}
)
