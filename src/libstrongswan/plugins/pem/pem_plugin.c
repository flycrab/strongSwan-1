/*
 * Copyright (C) 2009 Martin Willi
 * Hochschule fuer Technik Rapperswil
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include "pem_plugin.h"

#include <library.h>
#include "pem_builder.h"

typedef struct private_pem_plugin_t private_pem_plugin_t;

/**
 * private data of pem_plugin
 */
struct private_pem_plugin_t {

	/**
	 * public functions
	 */
	pem_plugin_t public;
};

/**
 * Implementation of pem_plugin_t.pemtroy
 */
static void destroy(private_pem_plugin_t *this)
{
	lib->creds->remove_builder(lib->creds,
							   (builder_constructor_t)private_key_pem_builder);
	lib->creds->remove_builder(lib->creds,
							   (builder_constructor_t)public_key_pem_builder);
	lib->creds->remove_builder(lib->creds,
							   (builder_constructor_t)certificate_pem_builder);
	lib->creds->remove_builder(lib->creds,
							   (builder_constructor_t)pluto_pem_builder);
	free(this);
}

/*
 * see header file
 */
plugin_t *plugin_create()
{
	private_pem_plugin_t *this = malloc_thing(private_pem_plugin_t);
	
	this->public.plugin.destroy = (void(*)(plugin_t*))destroy;
	
	/* register private key PEM decoding builders */
	lib->creds->add_builder(lib->creds, CRED_PRIVATE_KEY, KEY_ANY,
							(builder_constructor_t)private_key_pem_builder);
	lib->creds->add_builder(lib->creds, CRED_PRIVATE_KEY, KEY_RSA,
							(builder_constructor_t)private_key_pem_builder);
	lib->creds->add_builder(lib->creds, CRED_PRIVATE_KEY, KEY_ECDSA,
							(builder_constructor_t)private_key_pem_builder);
	lib->creds->add_builder(lib->creds, CRED_PRIVATE_KEY, KEY_DSA,
							(builder_constructor_t)private_key_pem_builder);
	
	/* register public key PEM decoding builders */
	lib->creds->add_builder(lib->creds, CRED_PUBLIC_KEY, KEY_ANY,
							(builder_constructor_t)public_key_pem_builder);
	lib->creds->add_builder(lib->creds, CRED_PUBLIC_KEY, KEY_RSA,
							(builder_constructor_t)public_key_pem_builder);
	lib->creds->add_builder(lib->creds, CRED_PUBLIC_KEY, KEY_ECDSA,
							(builder_constructor_t)public_key_pem_builder);
	lib->creds->add_builder(lib->creds, CRED_PUBLIC_KEY, KEY_DSA,
							(builder_constructor_t)public_key_pem_builder);
	
	/* register certificate PEM decoding builders */
	lib->creds->add_builder(lib->creds, CRED_CERTIFICATE, CERT_ANY,
							(builder_constructor_t)certificate_pem_builder);
	lib->creds->add_builder(lib->creds, CRED_CERTIFICATE, CERT_X509,
							(builder_constructor_t)certificate_pem_builder);
	lib->creds->add_builder(lib->creds, CRED_CERTIFICATE, CERT_X509_CRL,
							(builder_constructor_t)certificate_pem_builder);
	lib->creds->add_builder(lib->creds, CRED_CERTIFICATE, CERT_X509_OCSP_REQUEST,
							(builder_constructor_t)certificate_pem_builder);
	lib->creds->add_builder(lib->creds, CRED_CERTIFICATE, CERT_X509_OCSP_RESPONSE,
							(builder_constructor_t)certificate_pem_builder);
	lib->creds->add_builder(lib->creds, CRED_CERTIFICATE, CERT_X509_AC,
							(builder_constructor_t)certificate_pem_builder);
	lib->creds->add_builder(lib->creds, CRED_CERTIFICATE, CERT_TRUSTED_PUBKEY,
							(builder_constructor_t)certificate_pem_builder);
	lib->creds->add_builder(lib->creds, CRED_CERTIFICATE, CERT_PGP,
							(builder_constructor_t)certificate_pem_builder);
	
	/* pluto specific credentials formats */
	lib->creds->add_builder(lib->creds, CRED_PLUTO_CERT, 0,
							(builder_constructor_t)pluto_cert_pem_builder);
	lib->creds->add_builder(lib->creds, CRED_PLUTO_CRL, 0,
							(builder_constructor_t)pluto_crl_pem_builder);
	lib->creds->add_builder(lib->creds, CRED_PLUTO_AC, 0,
							(builder_constructor_t)pluto_ac_pem_builder);
	
	return &this->public.plugin;
}

