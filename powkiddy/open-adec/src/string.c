/*
 * Copyright 2021 Gianluca Pacchiella <gp@ktln2.org>
 *
 * This file is part of open-adec.
 *
 * open-adec is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * open-adec is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with open-adec.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <stdarg.h>
#include "regs_io.h"
#include "utils.h"
#include "uart.h"

enum va_state {
    VA_STATE_NORMAL = 0,
    VA_STATE_LENGTH = 1,
    VA_STATE_CONVERSION = 2
};

/*
 * size is the maximum count of character of the string (NOT INCLUDING THE
 * TERMINATING NULL BYTE!)
 *
 * https://www.pixelbeat.org/programming/gcc/format_specs.html
 */
int vsnprintf(char* str, size_t size, const char* format, va_list ap) {
    size_t n = 0;
    enum hd_mode mode;

    while (n < size) {
        char c = *format++;

        if (!c) {
            goto end;
        }

        if (c == '%') { /* we need conversion */
            mode = HD_MODE_32;
            int convert = 1;

            /* loop until you don't find the conversion char */
            while (convert) {
                char length_or_conversion = *format++;

                if (length_or_conversion == '\0')
                    goto end;

                switch (length_or_conversion) {
                    case '%': /* literal % */
                        *str++ = '%';
                        n++;
                        convert = 0;
                        break;

                        /* first we look for length */
                    case 'h':
                        /* h -> short  hh -> byte */
                        mode = (mode == HD_MODE_16) ? HD_MODE_8 : HD_MODE_16;
                        break;

                        /* then for the conversion */
                    case 'd':
                        convert = 0;
                        break;
                    case 'x': { /* hex representation */
                                  /* FIXME: you should check that n is less than size BEFORE
                                   * memcping it */
                                  u32 value = va_arg(ap, u32);
                                  memcpy(str, int2hex(value, mode), 8);
                                  /* FIXME: create define*/
                                  str += (mode + 1) * 2;
                                  n += (mode + 1) * 2;
                                  convert = 0;
                              }
                              break;
                    default: /* this shouldn't happen :P */
                              *str++ = length_or_conversion;
                              n++;
                } /* end switch */
            } /* end while */

        } else { /* if there is not conversion then use the char as is */
            *str++ = c;
            n++;
        }

    } /* main loop */

end:

    // NULL terminate the destination string
    *str = '\0';

    return n;
}

int printf(const char* format, ...) {
    char tmp[128];

    va_list ap;

    va_start(ap, format);

    int n = vsnprintf(tmp, 127, format, ap);

    va_end(ap);

    serial_puts(tmp);

    return n;
}
