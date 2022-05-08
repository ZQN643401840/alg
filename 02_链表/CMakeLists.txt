cmake_minimum_required(VERSION 3.5)  

project(list)

# 不宜这么写
# if(${CMAKE_TOOLCHAIN_FILE})    
# 用于判断时都不宜用取变量的方式,宜直接写,不加修饰
if(CMAKE_TOOLCHAIN_FILE)  
message("=========Define CMAKE_TOOLCHAIN_FILE ============")
message(${CMAKE_TOOLCHAIN_FILE})
endif()

# 这个能生效,但为了与上面保持统一,也直接用变量,不用取变量的方式
# if(${FLAG} STREQUAL ON)  
# 这种是判断变量值
if(FLAG STREQUAL ON)  
message("FLAG is ON")  
message(${3RD_VAR})
else()
message("FLAG is OFF")
endif()

# 这种是判断是否定义变量  
# 变量至少要赋个任意值,才能检测到,比如下面这样  
# cmake  -DCMAKE_TOOLCHAIN_FILE=../tool_chain.cmake -DFLAG=ON -DIMPORT_SYMBOL=a ../
if(DEFINED  IMPORT_SYMBOL)  
message("Has symbol")  
message(${3RD_VAR})
else()
message("Hasn't symbol")
endif()

message(INCLUDE_DIRECTORIES)   

aux_source_directory(. CPP_SRC)  

add_executable(${PROJECT_NAME} ${CPP_SRC})
