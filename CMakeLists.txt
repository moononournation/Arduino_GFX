
get_filename_component(dir ${CMAKE_CURRENT_LIST_FILE} PATH)
FILE(GLOB_RECURSE app_sources ${dir}/src/*.cpp)

idf_component_register(SRCS ${app_sources}      
                    REQUIRES wear_levelling esp_lcd Arduino Arduino_GFX
                    INCLUDE_DIRS "src"
)
