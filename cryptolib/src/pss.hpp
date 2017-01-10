#ifndef PSS_H
#define PSS_H

template<class DigestType>
class PSS {

    template<class InputIterator, class OutputIterator>
    void Encode(InpitIterator first, InputIterator last, OutputIterator result) {
	
      size_t hLen = DigestType::size;
      
      DigestType digest;
      
      std::vector<uint8_t> hash(hLen);
      digest(first, last, hash.begin());
      
      
      
    }

    template<class InputIterator, class OutputIterator>
    void Decode(InpitIterator first, InputIterator last, OutputIterator result) {
	
    }
};

#endif
