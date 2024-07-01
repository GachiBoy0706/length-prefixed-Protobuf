#include "DelimitedMessagesStreamParser.h"
#include <iostream>
#include <vector>
#include <gtest/gtest.h>


void AddMessage(std::vector<char>& messages, ProtMes::WrapperMessage& wrapper_message)
{
    int message_size = wrapper_message.ByteSizeLong();

    int prefix_size = google::protobuf::io::CodedOutputStream::VarintSize32(message_size);

    int old_size = messages.size();
    messages.resize(old_size + prefix_size + message_size);

    google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(message_size, reinterpret_cast<uint8_t*>(messages.data() + old_size));

    // Сериализация сообщения в массив
    wrapper_message.SerializeToArray(messages.data() + old_size + prefix_size, message_size);

}

TEST(DelimitedMessagesStreamParser, parse)
{
    
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    ProtMes::WrapperMessage wrapper_message;

    ProtMes::FastResponse* fast_response = new ProtMes::FastResponse();
    fast_response->set_current_date_time("20240622T123456.789");

    wrapper_message.set_allocated_fast_response(fast_response);

    std::vector<char> messages;

    AddMessage(messages, wrapper_message);
    

    typedef DelimitedMessagesStreamParser<ProtMes::WrapperMessage> Parser;

    Parser parser;

    // идем по одному байту по входному потоку сообщений

    typedef DelimitedMessagesStreamParser<ProtMes::WrapperMessage>::PointerToConstValue ParsedMessage;

    const std::list<ParsedMessage> parsedMessages = parser.parse(std::string(messages.begin(), messages.end()));

    // тут код проверки, что все сообщения расшифровались верно
    
    const ParsedMessage& value = parsedMessages.front();

    EXPECT_EQ(value->fast_response().current_date_time(), "20240622T123456.789");
    

    // Очистка ресурсов, используемых библиотекой Protocol Buffers
    google::protobuf::ShutdownProtobufLibrary();


}


//TEST()

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}