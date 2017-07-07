#ifndef MGF_H
#define MGF_H

namespace cry {

    template <class HashType>
    struct MGF1 {
        template <class InputIterator, class OutputIterator>
        void operator()(InputIterator first, InputIterator last, OutputIterator result, size_t maskLen) const {

            if (maskLen > 0xffffffff) {
                throw 1; // mask too long
            }

            size_t hLen = HashType::size;
            HashType hash;

            size_t i = 0;
            size_t rest = maskLen;
            for (; i < maskLen / hLen; ++i) {

                std::vector<uint8_t> mgfSeed(first, last);

                mgfSeed.push_back((i & 0xFF000000) >> 24);
                mgfSeed.push_back((i & 0x00FF0000) >> 16);
                mgfSeed.push_back((i & 0x0000FF00) >> 8);
                mgfSeed.push_back((i & 0x000000FF) >> 0);

                hash(mgfSeed.begin(), mgfSeed.end(), result);

                result += hLen;
                rest -= hLen;
            }

            {
                std::vector<uint8_t> mgfSeed(first, last);

                mgfSeed.push_back((i & 0xFF000000) >> 24);
                mgfSeed.push_back((i & 0x00FF0000) >> 16);
                mgfSeed.push_back((i & 0x0000FF00) >> 8);
                mgfSeed.push_back((i & 0x000000FF) >> 0);

                std::vector<uint8_t> digest(hLen);
                hash(mgfSeed.begin(), mgfSeed.end(), digest.begin());
                std::copy_n(digest.begin(), rest, result);
            }
        }

      protected:
    };
}

#endif