cmake_minimum_required(VERSION 3.10)
project(ProtobufProject)

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
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

# Укажите исходные файлы вашего проекта
#set(MAIN_SOURCE_FILES main.cpp ${PROTO_SRCS} ${PROTO_HDRS})

# Создайте исполняемый файл
# add_executable(ProtobufExecutable ${MAIN_SOURCE_FILES})

# Связать исполняемый файл с библиотеками Protocol Buffers
# target_link_libraries(ProtobufExecutable ${Protobuf_LIBRARIES})

# Найдите Google Test
 find_package(GTest REQUIRED)
 include_directories(${GTEST_INCLUDE_DIRS})

# Укажите исходные файлы для тестов
 set(TEST_SOURCE_FILES test1.cpp ${PROTO_SRCS} ${PROTO_HDRS}) # if proto files included into test.cpp everything falls

# Добавьте исполняемый файл для тестов
 add_executable(TestExecutable ${TEST_SOURCE_FILES}) #проблема вот тут возникает, если собирать чисто с test1.cpp, то все ок

# Связать исполняемый файл для тестов с библиотеками Google Test
 target_link_libraries(TestExecutable ${GTEST_BOTH_LIBRARIES} pthread ${Protobuf_LIBRARIES})
