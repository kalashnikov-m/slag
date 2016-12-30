#ifndef OID_HPP
#define OID_HPP

#include <sha1.hpp>
#include <sha256.hpp>
#include <sha224.hpp>
#include <sha256.hpp>
#include <sha384.hpp>
#include <sha512.hpp>

namespace cry {

    template <class Obj, class SequenceType = std::initializer_list<uint8_t>>
    struct OID;

    template <class SequenceType>
    struct OID<SHA1, SequenceType> {

        static const SequenceType value() { return {0x30, 0x21, 0x30, 0x09, 0x06, 0x05, 0x2b, 0x0e, 0x03, 0x02, 0x1a, 0x05, 0x00, 0x04, 0x14}; }
    };

    template <class SequenceType>
    struct OID<SHA224, SequenceType> {

        static const SequenceType value() { return {0x30, 0x2d, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x04, 0x05, 0x00, 0x04, 0x1c}; }
    };

    template <class SequenceType>
    struct OID<SHA256, SequenceType> {

        static const SequenceType value() { return {0x30, 0x31, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x01, 0x05, 0x00, 0x04, 0x20}; }
    };

    template <class SequenceType>
    struct OID<SHA384, SequenceType> {

        static const SequenceType value() { return {0x30, 0x41, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x02, 0x05, 0x00, 0x04, 0x30}; }
    };

    template <class SequenceType>
    struct OID<SHA512, SequenceType> {

        static const SequenceType value() { return {0x30, 0x51, 0x30, 0x0d, 0x06, 0x09, 0x60, 0x86, 0x48, 0x01, 0x65, 0x03, 0x04, 0x02, 0x03, 0x05, 0x00, 0x04, 0x40}; }
    };
}
#endif
