#include <memory>
#include "message.pb.h"
#include <list>
#include <string>
#include "message.pb.h"
#include <exception>

template<typename Message>
std::shared_ptr<Message> parseDelimited(const void* data, size_t size, size_t* bytesConsumed = 0)
{


    if (!size) // нечего парсить, data пустая 
    {
        *bytesConsumed = 0;
        return nullptr;
    }

    
    // Десериализация сообщения из массива с префиксом длины
    //здесь надо попробовать static_cast
    google::protobuf::io::CodedInputStream input_stream(reinterpret_cast<const uint8_t*>(data), size);
    
    auto initial_position = input_stream.CurrentPosition();
    
    // Чтение префикса длины \\\\\\\\\\\\\\\\\\\\\\\\\\!!!!!!!!!!!!!!!!!!!!!!!!!!/////////////////////////////выжный момент для декодирования у меня
    uint32_t message_size;
    if (!input_stream.ReadVarint32(&message_size)) {
        *bytesConsumed = 0;
        return nullptr;
    }

    auto prefix_size = input_stream.CurrentPosition() - initial_position;

    if (message_size > static_cast<uint32_t>(size-1)) // в буфере нецелое сообщение
    {
        *bytesConsumed = 0;
        return nullptr;
    }
    
    // Чтение и десериализация сообщения
    std::shared_ptr<Message> deserialized_message = std::make_shared<Message>();
    if (!deserialized_message->ParseFromArray(data + prefix_size, message_size)) {
        throw std::runtime_error("Failed to parse message.");
    }
    

    *bytesConsumed = static_cast<size_t>(message_size) + static_cast<size_t>(prefix_size);
    std::shared_ptr<Message> mes_ptr = deserialized_message;

    return mes_ptr;
};




template<typename MessageType>
class DelimitedMessagesStreamParser
{
public:
    typedef std::shared_ptr<const MessageType> PointerToConstValue;

    std::list<PointerToConstValue> parse(const std::string& data) // ощущение такое будто сюда должен передаваться массив всех байтов
    {
        //тут переводим data в элементы char, запихиваем в m_buffer, m_buffer приводим к constvoid* через .data() и static_cast<constvoid*>
        for (const char ch : data)
        {
            m_buffer.push_back(ch);
        }

      
        size_t bytesConsumed = 0;
        std::list<PointerToConstValue> messages;
        do {

                m_buffer.erase(m_buffer.begin(), m_buffer.begin() + bytesConsumed); //затираем расшифрованное сообщение (длина + само сообщение)
                messages.push_back(parseDelimited<MessageType>(static_cast<const void*>(m_buffer.data()), m_buffer.size(), &bytesConsumed));
        } while (bytesConsumed); //пока получается расшифровать

        if (!messages.back()) //чистим пустое сообщение 
            messages.pop_back();

        return messages;
    }

private:
    std::vector<char> m_buffer;

};

