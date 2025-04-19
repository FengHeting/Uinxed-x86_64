/*
 *
 *		stdlib.c
 *		Standard library
 *
 *		2024/10/2 By MicroFish
 *		Based on GPL-3.0 open source agreement
 *		Copyright © 2020 ViudiraTech, based on the GPLv3 agreement.
 *
 */

#include "stdlib.h"
#include "string.h"

/* Determine whether it is a number */
int is_digit(int c)
{
	return c >= '0' && c <= '9';
}

/* Convert a string number to an integer number */
int atoi(const char *pstr)
{
	int ret_integer	 = 0;
	int integer_sign = 1;

	if (*pstr == '-') integer_sign = -1;
	if (*pstr == '-' || *pstr == '+') pstr++;

	while (*pstr >= '0' && *pstr <= '9') {
		ret_integer = ret_integer * 10 + *pstr - '0';
		pstr++;
	}
	ret_integer = integer_sign * ret_integer;
	return ret_integer;
}

/* Skip numbers in a string and return the value of those consecutive numbers */
int skip_atoi(const char **s)
{
	int i = 0;
	while (is_digit(**s)) {
		i = i * 10 + *((*s)++) - '0';
	}
	return i;
}

/* Formatting an integer as a string */
char *number(char *str, int64_t num, int base, int size, int precision, int type)
{
	char		c, sign, tmp[65];
	const char *digits = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int			i; // index of tmp

	if (type & SMALL) digits = "0123456789abcdefghijklmnopqrstuvwxyz";
	if (type & LEFT) type &= ~ZEROPAD;	 // left adjust without zero padding
	if (base < 2 || base > 36) return 0; // Invalid base

	c = (type & ZEROPAD) ? '0' : ' '; // Default is space

	if (type & SIGN && num < 0) {
		sign = '-';
		num	 = -num;
	} else {
		sign = (type & PLUS) ? '+' : ((type & SPACE) ? ' ' : 0);
	}
	if (sign) {
		size--;
	}
	if (type & SPECIAL) {
		if (base == 16) {
			size -= 2;
		} else if (base == 8) {
			size--;
		}
	}

	i = 0;

	if (num == 0) {
		tmp[i++] = '0';
	} else {
		while (num != 0) {
			// tmp[i++] = digits[do_div(num, base)];
			// This code is a assembly code,
			// but I think the compiler will optimize it.
			tmp[i++] = digits[(uint64_t)num % (uint64_t)base];
			num		 = (uint64_t)num / (uint64_t)base;
		}
	}
	if (i > precision) {
		precision = i;
	}
	size -= precision;

	if (!(type & (ZEROPAD + LEFT))) {
		while (size-- > 0) {
			*str++ = ' ';
		}
	}
	if (sign) {
		*str++ = sign;
	}
	if (type & SPECIAL) {
		if (base == 8) {
			*str++ = '0';
		} else if (base == 16) {
			*str++ = '0';
			*str++ = digits[33];
		}
	}
	if (!(type & LEFT)) {
		while (size-- > 0) {
			*str++ = c;
		}
	}
	while (i < precision--) {
		*str++ = '0';
	}
	while (i-- > 0) {
		*str++ = tmp[i];
	}
	while (size-- > 0) {
		*str++ = ' ';
	}
	return str;
}

/* Returns the size of a string with an integer formatted by `number()` */
uint64_t number_length(int64_t num, int base, int size, int precision, int type)
{
	char sign		   = 0; // is there a sign // 0: no sign, 1: sign
	int	 number_digits = 0;
	if (num < 0) {
		num = -num;
	}
	if (type & SIGN || type & PLUS || type & SPACE) {
		sign = 1;
	}
	if (num == 0) {
		number_digits = 1;
	} else {
		while (num != 0) {
			num = (uint64_t)num / (uint64_t)base;
			number_digits++;
		}
	}
	if (type & SPECIAL) {
		if (base == 16) {
			number_digits += 2;
		} else if (base == 8) {
			number_digits += 1;
		}
	}
	if (number_digits + sign < size) {
		return size;
	}
	if (number_digits + sign < precision) {
		return size;
	}
	return number_digits + sign;
};
