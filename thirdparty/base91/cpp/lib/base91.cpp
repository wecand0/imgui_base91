#include "base91.h"

#include <stdexcept>

void Base91::encode(const std::string_view &data) {
    if (data.empty()) {
        throw std::invalid_argument("Input string cannot be empty.");
    }

    uint32_t data_size = data.size();
    uint32_t out_size = data_size;
    auto compute_encoded_size = [&out_size, this]() {
        out_size <<= 4;
        if (0 != out_size % b91word_bit_c) {
            out_size /= b91word_bit_c;
            out_size += 1;
        } else {
            out_size /= b91word_bit_c;
        }
        return out_size;
    };

    if (out_size == 0) {
        throw std::invalid_argument("Out size of encoding string cannot be 0.");
    }
    data_.clear();
    data_.reserve(compute_encoded_size());

    const uint8_t *ib = (uint8_t *) data.data();
    uint32_t queue{};
    uint32_t nbits{};
    for (uint32_t len = 0; len < data_size; len++) {
        queue |= *ib++ << nbits;
        nbits += 8;
        if (nbits > b91word_bit_c) { /* enough bits in queue */
            uint32_t val = queue & 8191;
            if (val > 88) {
                queue >>= b91word_bit_c;
                nbits -= b91word_bit_c;
            } else { /* we can take 14 bits */
                val = queue & 16383;
                queue >>= 14;
                nbits -= 14;
            }
            data_.push_back(basicAlphabet_c[val % 91]);
            data_.push_back(basicAlphabet_c[val / 91]);
        }
    }
    if (nbits) {
        data_.push_back(basicAlphabet_c[queue % 91]);
        if (nbits > 7 || queue > 90) {
            data_.push_back(basicAlphabet_c[queue / 91]);
        }
    }
}

void Base91::decode(const std::string_view &data) {
    if (data.empty()) {
        throw std::invalid_argument("Input string cannot be empty.");
    }

    uint32_t data_size = data.size();
    uint32_t out_size = data_size;
    auto compute_decoded_size = [&out_size, this]() {
        out_size *= b91word_bit_c;
        out_size >>= 4;
        return out_size;
    };

    if (out_size == 0) {
        throw std::invalid_argument("Out size of decoding string cannot be 0.");
    }

    data_.clear();
    //data_.reserve(compute_decoded_size());

    const uint8_t *ib = (uint8_t *) data.data();
    uint32_t queue{};
    uint32_t nbits{};
    uint32_t val = -1;
    for (uint32_t len = 0; len < data_size; len++) {
        uint32_t d = decAlphabet_c[*ib++];
        if (d == 91)
            continue; /* ignore non-alphabet chars */
        if (val == -1)
            val = d; /* start next value */
        else {
            val += d * 91;
            queue |= val << nbits;
            nbits += (val & 8191) > 88 ? b91word_bit_c : 14;
            do {
                data_.push_back((char) queue);
                queue >>= 8;
                nbits -= 8;
            } while (nbits > 7);
            val = -1; /* mark value complete */
        }
    }
    /* process remaining bits; write at most 1 byte */
    if (val != -1) {
        data_.push_back(char(queue | val << nbits));
    }
}
