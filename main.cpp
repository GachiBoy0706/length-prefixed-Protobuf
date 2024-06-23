#include "DelimitedMessagesStreamParser.h"
#include <iostream>
#include <vector>


int main()
{

    GOOGLE_PROTOBUF_VERIFY_VERSION;

    ProtMes::WrapperMessage wrapper_message;

    ProtMes::FastResponse* fast_response = new ProtMes::FastResponse();
    fast_response->set_current_date_time("20240622T123456.789");

    wrapper_message.set_allocated_fast_response(fast_response);

    int message_size = wrapper_message.ByteSizeLong();

    int prefix_size = google::protobuf::io::CodedOutputStream::VarintSize32(message_size);

    std::vector<char> messages(prefix_size + message_size);

    google::protobuf::io::CodedOutputStream::WriteVarint32ToArray(message_size, reinterpret_cast<uint8_t*>(messages.data()));

    // Сериализация сообщения в массив
    wrapper_message.SerializeToArray(messages.data() + prefix_size, message_size);

    

    typedef DelimitedMessagesStreamParser<ProtMes::WrapperMessage> Parser;

    Parser parser;

    // идем по одному байту по входному потоку сообщений

    typedef DelimitedMessagesStreamParser<ProtMes::WrapperMessage>::PointerToConstValue ParsedMessage;

    const std::list<ParsedMessage> parsedMessages = parser.parse(std::string(messages.begin(), messages.end()));


    // тут код проверки, что все сообщения расшифровались верно
    for (const ParsedMessage& value : parsedMessages)
    {

        if (value->has_fast_response()) {
            std::cout << "Deserialized FastResponse current_date_time: "
                    << value->fast_response().current_date_time() << std::endl;
        }

        if (value->has_slow_response()) {
            std::cout << "Deserialized SlowResponse current_date_time: "
                    << value->slow_response().connected_client_count() << std::endl;
        }

        if (value->has_request_for_slow_response()) {
            std::cout << "Deserialized RequestForSlowResponse current_date_time: "
                    << value->request_for_slow_response().time_in_seconds_to_sleep() << std::endl;
        }

    }


    // Очистка ресурсов, используемых библиотекой Protocol Buffers
    google::protobuf::ShutdownProtobufLibrary();

   return 0;

}