cmake_minimum_required(VERSION 3.10)
project(Protobuf)

# Укажите стандарт C++
set(CMAKE_CXX_STANDARD 11)

# Найдите пакет Protobuf
find_package(Protobuf REQUIRED)
include_directories(${Protobuf_INCLUDE_DIRS})

# Указываем путь к сгенерированным файлам protobuf
include_directories(${CMAKE_CURRENT_BINARY_DIR})

# Укажите путь к вашим .proto файлам
set(PROTO_FILES ${CMAKE_CURRENT_SOURCE_DIR}/message.proto)

# Сгенерируйте C++ файлы из .proto
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${CMAKE_CURRENT_BINARY_DIR} ${PROTO_FILES})

# Укажите исходные файлы вашего проекта
set(SOURCE_FILES main.cpp ${PROTO_SRCS} ${PROTO_HDRS})

# Создайте исполняемый файл
add_executable(Protobuf ${SOURCE_FILES})

# Связать исполняемый файл с библиотеками Protocol Buffers
target_link_libraries(Protobuf ${Protobuf_LIBRARIES})
