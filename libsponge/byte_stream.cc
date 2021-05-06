#include "byte_stream.hh"

// Dummy implementation of a flow-controlled in-memory byte stream.

// For Lab 0, please replace with a real implementation that passes the
// automated checks run by `make check_lab0`.

// You will need to add private members to the class declaration in `byte_stream.hh`

template <typename... Targs>
void DUMMY_CODE(Targs &&... /* unused */) {}

using namespace std;

ByteStream::ByteStream(const size_t capacity) {
    this->capacity_ = capacity;
    this->byte_read_ = 0;
    this->byte_written_ = 0;
    this->input_ended_ = false;
    this->data_ = "";
}

size_t ByteStream::write(const string &data) {
    auto length = data.length();
    auto remaining_capacity = this->remaining_capacity();
    size_t res;
    if (length <= remaining_capacity) {
        res = length;
        this->data_ += data;
    } else {
        res = remaining_capacity;
        this->data_ += data.substr(0, remaining_capacity);
    }
    this->byte_written_ += res;
    return res;
}

//! \param[in] len bytes will be copied from the output side of the buffer
string ByteStream::peek_output(const size_t len) const {
    auto buffer_size = this->buffer_size();
    string data;
    if (len <= buffer_size) {
        data = this->data_.substr(0, len);
    } else {
        data = this->data_;
    }
    return data;
}

//! \param[in] len bytes will be removed from the output side of the buffer
void ByteStream::pop_output(const size_t len) {
    auto buffer_size = this->buffer_size();
    if (buffer_size >= len) {
        this->byte_read_ += len;
        this->data_ = this->data_.substr(len, buffer_size);
    } else {
        this->byte_read_ += buffer_size;
        this->data_ = "";
    }
}

//! Read (i.e., copy and then pop) the next "len" bytes of the stream
//! \param[in] len bytes will be popped and returned
//! \returns a string
std::string ByteStream::read(const size_t len) {
    auto data = this->peek_output(len);
    this->pop_output(len);
    return data;
}

void ByteStream::end_input() {
    this->input_ended_ = true;
}

bool ByteStream::input_ended() const { return this->input_ended_; }

size_t ByteStream::buffer_size() const { return this->data_.length(); }

bool ByteStream::buffer_empty() const { return this->data_.empty() ; }

bool ByteStream::eof() const { return this->input_ended() && this->buffer_empty(); }

size_t ByteStream::bytes_written() const { return this->byte_written_; }

size_t ByteStream::bytes_read() const { return this->byte_read_; }

size_t ByteStream::remaining_capacity() const { return this->capacity_ - this->buffer_size(); }
