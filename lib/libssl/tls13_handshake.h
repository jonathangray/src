/* $OpenBSD: tls13_handshake.h,v 1.2 2019/01/20 22:36:19 tb Exp $ */
/*
 * Copyright (c) 2019 Theo Buehler <tb@openbsd.org>
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION
 * OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef HEADER_TLS13_HANDSHAKE_H
#define HEADER_TLS13_HANDSHAKE_H

#include <stddef.h>	/* for NULL */

__BEGIN_HIDDEN_DECLS

#define INITIAL			0x00
#define NEGOTIATED		0x01
#define WITH_HRR		0x02
#define WITHOUT_CR		0x04
#define WITH_PSK		0x08
#define WITH_CCV		0x10
#define WITH_0RTT		0x20

enum tls13_message_type {
	INVALID,
	CLIENT_HELLO,
	SERVER_HELLO,
	CLIENT_HELLO_RETRY,
	SERVER_ENCRYPTED_EXTENSIONS,
	SERVER_CERTIFICATE_REQUEST,
	SERVER_CERTIFICATE,
	SERVER_CERTIFICATE_VERIFY,
	SERVER_FINISHED,
	CLIENT_END_OF_EARLY_DATA,
	CLIENT_CERTIFICATE,
	CLIENT_CERTIFICATE_VERIFY,
	CLIENT_FINISHED,
	CLIENT_KEY_UPDATE,
	SERVER_NEW_SESSION_TICKET,
	APPLICATION_DATA,
	TLS13_NUM_MESSAGE_TYPES,
};

__END_HIDDEN_DECLS

#endif /* !HEADER_TLS13_HANDSHAKE_H */
