#pragma once

#include <array>
#include <cstddef>
#include <iostream>
#include <memory>

namespace serialcom {

    /// Adapted from https://en.cppreference.com/w/cpp/io/basic_streambuf/overflow's example
    template<std::size_t size, class CharT = char>
    struct ArrayedStreamBuffer : std::basic_streambuf<CharT>
    {
        using Base = std::basic_streambuf<CharT>;
        using char_type = typename Base::char_type;
        using int_type = typename Base::int_type;
    
        ArrayedStreamBuffer()
        {
            // put area pointers to work with 'buffer'
            Base::setp(buffer.data(), buffer.data() + size);
        }

        ~ArrayedStreamBuffer()
        {
            if (this->stream && this->originalBuffer)  
                this->stream->rdbuf(originalBuffer);
        }

        void installOnStream(std::ostream* stream)
        {
            this->stream = stream;
            this->originalBuffer = this->stream->rdbuf();
            this->stream->rdbuf(this);
        }
        
        void flushBuffer()
        {
            if (!this->stream || !this->originalBuffer)
                return;
            this->stream->rdbuf(originalBuffer);
            this->stream->write(buffer.data(), this->pptr() - this->pbase());
            this->stream->flush(); // Ensure it's flushed to output
            this->stream->rdbuf(this);
            buffer.fill(0);
            Base::setp(buffer.data(), buffer.data() + size);
        }
        
        void log(const std::string& log)
        {
            this->buffer->write(log);
        }

        void directLog(const std::string& log, bool newLine)
        {
            if (!this->stream || !this->originalBuffer)
                return;
            this->stream->rdbuf(originalBuffer);
            *(this->stream) << log;
            if (newLine)
                *(this->stream) << std::endl;
            else
                *(this->stream) << std::flush;
            this->stream->rdbuf(this);
        }

        std::streambuf* changeDefaultBuffer(std::streambuf* buffer)
        {
            std::streambuf* oldBuffer = this->originalBuffer;
            this->originalBuffer = buffer;
            return oldBuffer;
        }
    private:
        int_type overflow(int_type ch) override
        {
            if (!this->stream)
                return Base::overflow(ch);
            
            flushBuffer();
            *(this->stream) << (char)ch;
            return 1; // overflow management succeeded
        }

        std::array<char_type, size> buffer{}; // value-initialize buffer
        std::streambuf* originalBuffer;
        std::ostream* stream;
    };
}