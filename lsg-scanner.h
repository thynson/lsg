/*
 * Copyright (C) 2012 LAN Xingcan
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef __LSG_SCANNER_H__
#define __LSG_SCANNER_H__

#include <stddef.h>

/* Forward declaration of struct lsg_scanner_t */
struct lsg_scanner_t;

typedef int (*lsg_machine_t)(struct lsg_scanner_t *scanner);
typedef int (*lsg_callback_t)(struct lsg_scanner_t *scanner, int event);

typedef struct lsg_scanner_t
{
    lsg_machine_t machine;
    lsg_callback_t callback;
    void *param;
    int state;
    const char *buff;
    size_t buff_size;
    const char *ptr;
} lsg_scanner_t;

static void lsg_init(struct lsg_scanner_t *scanner, lsg_machine_t machine,
                     lsg_callback_t callback, void *param)
{
    scanner->machine = machine;
    scanner->callback = callback;
    scanner->state = 0;
    scanner->param = param;
}

static void lsg_scan(struct lsg_scanner_t *scanner, const char *buff,
                     size_t size)
{
    scanner->buff = buff;
    scanner->buff_size = size;
    scanner->ptr = buff;
    for ( ; ; ) {
        int ret = scanner->machine(scanner);
        if (ret == 0 &&  scanner->buff + scanner->buff_size == scanner->ptr)
            break;
        else
            scanner->callback(scanner, ret);
    }
}

static void lsg_end(struct lsg_scanner_t *scanner)
{
    scanner->buff = scanner->ptr = NULL;
    scanner->buff_size = 0;
    scanner->machine(scanner);
}

#endif
