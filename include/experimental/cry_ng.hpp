#include <stdint.h>
#include <stdlib.h>
#include <string.h>

namespace
{
	using base_t = uint32_t;
	using wide_t = uint64_t;
	const wide_t base = static_cast<wide_t>(UINT32_MAX) + 1;
	const auto BASE_SZ = sizeof(base_t);
}

void Cry_add(uint32_t** out, size_t* outLen, uint32_t* a, size_t alen, uint32_t* b, size_t blen)
{
	// Находим максимальную длину массива для результата
	size_t maxLen = (alen > blen) ? alen : blen;

	// Выделяем память под массив результата
	uint32_t* result = new uint32_t[maxLen + 1];
	if (result == NULL)
		return; // Не удалось выделить память

	uint64_t carry = 0; // Перенос на следующий разряд
	uint64_t i = 0;

	// Складываем числа побайтно
	while (i < maxLen)
	{
		// Суммируем текущие разряды и перенос
		uint64_t sum = carry;
		if (i < alen)
		{
			sum += a[i];
		}

		if (i < blen)
		{
			sum += b[i];
		}

		// Обновляем значение нынешнего разряда и переноса
		result[i] = (uint32_t)sum;
		carry = sum >> sizeof(uint32_t) * 8;

		i++;
	}

	if (carry > 0)
	{
		// Если есть оставшийся перенос, то добавляем его в результат
		result[i] = (uint32_t)carry;
		*outLen = maxLen + 1;
	}
	else
	{
		*outLen = maxLen;
	}

	*out = result;
}

void Cry_multiply(uint32_t** production, size_t* productionLen, uint32_t* a, size_t alen, uint32_t* b, size_t blen)
{
	auto pResult = new uint32_t[alen + blen];
	if (pResult == nullptr)
		return;

	memset(pResult, 0x00, (alen + blen) * sizeof(uint32_t));

	base_t carry = 0x00;
			
	auto last_result = pResult;

	for (auto first1 = a, last1 = a + alen; first1 < last1; ++first1)
	{
		auto resultIter = last_result;

		for (auto first2 = b, last2 = b + blen; first2 < last2; ++first2)
		{
			wide_t temp = static_cast<wide_t>(*resultIter) + static_cast<wide_t>(*first1) * static_cast<wide_t>(*first2) + static_cast<wide_t>(carry);
			*resultIter = static_cast<base_t>(temp % base);
			carry = static_cast<base_t>(temp / base);

			resultIter++;
		}

		*(resultIter) = carry;
		carry = 0;

		last_result++;
	}
	

	*production = pResult;
	*productionLen = alen + blen;
}

void Cry_subtract(uint32_t** result, size_t* resultLen, uint32_t* a, size_t alen, uint32_t* b, size_t blen)
{
	auto out = new base_t[std::max(alen, blen)];
	memset(out, 0x00, std::max(alen, blen) * sizeof(base_t));

	auto resultIter = out;

	wide_t carry = 0;

	for (auto first1 = a, last1 = a + alen, first2 = b, last2 = b + blen; (first1 < last1) && (first2 < last2); ++first1, ++first2)
	{
		if (*first1 < *first2)
		{
			*resultIter++ = static_cast<base_t>((*first1) - (*first2) - carry + base);
			carry = 1;
		}
		else
		{
			*resultIter++ = static_cast<base_t>((*first1) - (*first2) - carry);
			carry = 0;
		}
	}

	*result = out;
	*resultLen = std::max(alen, blen);
}
