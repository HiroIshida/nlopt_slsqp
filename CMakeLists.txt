set(SRC src/slsqp.c src/stop.c)
include_directories(include)
add_library(nlopt_slsqp SHARED ${SRC})
add_library(nlopt_slsqp_static STATIC ${SRC})

add_executable(example example/main.cpp)
target_link_libraries(example nlopt_slsqp)
