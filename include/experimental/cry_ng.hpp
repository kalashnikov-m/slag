#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void add(uint32_t** out, size_t* outLen, uint32_t* a, size_t alen, uint32_t* b, size_t blen)
{
	// Находим максимальную длину массива для результата
	size_t maxLen = (alen > blen) ? alen : blen;

	// Выделяем память под массив результата
	uint32_t* result = new uint32_t[maxLen + 1];
	if (result == NULL)
	{
		return; // Не удалось выделить память
	}

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