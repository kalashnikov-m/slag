//
// Created by max on 07.07.17.
//

#ifndef RSAPSS_PSS_H
#define RSAPSS_PSS_H
#include "basic_int.hpp"
#include "emsa_pss.hpp"

namespace cry {

	template<class Encoder=emsa_pss<SHA1>, class T = bigint8_t>
	struct rsass_pss
	{
		template<class InputIterator>
		std::vector<uint8_t> sign(InputIterator m_first, InputIterator m_last, const T& n, const T& d, size_t modBits, const std::vector<uint8_t>& salt = std::vector<uint8_t>()) const
		{
			//////////////////////////
			// 1. EMSA-PSS encoding:
			Encoder encoder;
			std::vector<uint8_t> EM(modBits / 8);
			encoder.encode(m_first, m_first, EM.begin(), modBits, salt);

			//////////////////////////
			// 2. RSA signature:

			/////////////////////////////////////////////////////////////////////////////
			// 2a. Convert the encoded message EM to an integer message representative m
			const T m(EM.begin(), EM.end());

			////////////////////////////////////////////
			// 2b. Apply the RSASP1 signature primitive
			const T s = cry::PowMod(m, d, n);

			//////////////////////////////////////////////////////////////////////////////////
			// 2c. Convert the signature representative s to a signature S of length k octets
			std::vector<uint8_t> S(s);

			return S;
		}

		template<class InputIterator, class OutputIterator>
		bool verify(InputIterator m_first, InputIterator m_last, const T& n, const T& e) const
		{

		}
	};

} // namespace cry

#endif // RSAPSS_PSS_H
