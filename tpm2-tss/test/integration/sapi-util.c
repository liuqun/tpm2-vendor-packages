/*
 * Copyright (c) 2017, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */
#include <inttypes.h>

#include "log.h"
#include "sapi-util.h"
/*
 * Use te provide SAPI context to create & load a primary key. The key will
 * be a 2048 bit (restricted decryption) RSA key. The associated symmetric
 * key is a 128 bit AES (CFB mode) key.
 */
TSS2_RC
create_primary_rsa_2048_aes_128_cfb (
    TSS2_SYS_CONTEXT *sapi_context,
    TPM_HANDLE       *handle)
{
    TSS2_RC                 rc              = TSS2_RC_SUCCESS;
    TPM2B_SENSITIVE_CREATE  in_sensitive    = { 0 };
    TPM2B_PUBLIC            in_public       = { 0 };
    TPM2B_DATA              outside_info    = { 0 };
    TPML_PCR_SELECTION      creation_pcr    = { 0 };
    TPM2B_PUBLIC            out_public      = { 0 };
    TPM2B_CREATION_DATA     creation_data   = { 0 };
    TPM2B_DIGEST            creation_hash   = TPM2B_DIGEST_INIT;
    TPMT_TK_CREATION        creation_ticket = { 0 };
    TPM2B_NAME              name            = TPM2B_NAME_INIT;
    /* session parameters */
    /* command session info */
    TPMS_AUTH_COMMAND   session_cmd = { .sessionHandle = TPM_RS_PW };
    TPMS_AUTH_COMMAND  *session_cmd_array[1] = { &session_cmd };
    TSS2_SYS_CMD_AUTHS  sessions_cmd = {
        .cmdAuths      = session_cmd_array,
        .cmdAuthsCount = 1
    };
    /* response session info */
    TPMS_AUTH_RESPONSE  session_rsp          = { 0 };
    TPMS_AUTH_RESPONSE *session_rsp_array[1] = { &session_rsp };
    TSS2_SYS_RSP_AUTHS  sessions_rsp     = {
        .rspAuths      = session_rsp_array,
        .rspAuthsCount = 1
    };

    if (sapi_context == NULL || handle == NULL) {
        return TSS2_APP_RC_BAD_REFERENCE;
    }
    in_public.t.publicArea.type = TPM_ALG_RSA;
    in_public.t.publicArea.nameAlg = TPM_ALG_SHA256;
    in_public.t.publicArea.objectAttributes.restricted = 1;
    in_public.t.publicArea.objectAttributes.userWithAuth = 1;
    in_public.t.publicArea.objectAttributes.decrypt = 1;
    in_public.t.publicArea.objectAttributes.fixedTPM = 1;
    in_public.t.publicArea.objectAttributes.fixedParent = 1;
    in_public.t.publicArea.objectAttributes.sensitiveDataOrigin = 1;

    in_public.t.publicArea.parameters.rsaDetail.symmetric.algorithm = TPM_ALG_AES;
    in_public.t.publicArea.parameters.rsaDetail.symmetric.keyBits.aes = 128;
    in_public.t.publicArea.parameters.rsaDetail.symmetric.mode.aes = TPM_ALG_CFB;
    in_public.t.publicArea.parameters.rsaDetail.scheme.scheme = TPM_ALG_NULL;
    in_public.t.publicArea.parameters.rsaDetail.keyBits = 2048;

    print_log ("CreatePrimary RSA 2048, AES 128 CFB");
    rc = Tss2_Sys_CreatePrimary (sapi_context,
                                 TPM_RH_OWNER,
                                 &sessions_cmd,
                                 &in_sensitive,
                                 &in_public,
                                 &outside_info,
                                 &creation_pcr,
                                 handle,
                                 &out_public,
                                 &creation_data,
                                 &creation_hash,
                                 &creation_ticket,
                                 &name,
                                 &sessions_rsp);
    if (rc == TPM_RC_SUCCESS) {
        print_log ("success");
    } else {
        print_fail ("CreatePrimary FAILED! Response Code : 0x%x", rc);
    }

    return rc;
}

TSS2_RC
create_aes_128_cfb (
    TSS2_SYS_CONTEXT *sapi_context,
    TPM_HANDLE        handle_parent,
    TPM_HANDLE       *handle)
{
    TSS2_RC                 rc              = TSS2_RC_SUCCESS;
    TPM2B_SENSITIVE_CREATE  in_sensitive    = { 0 };
    /* template defining key type */
    TPM2B_PUBLIC            in_public       = {
        .t = {
            .publicArea.type = TPM_ALG_SYMCIPHER,
            .publicArea.nameAlg = TPM_ALG_SHA256,
            .publicArea.objectAttributes = {
                .decrypt = 1,
                .fixedTPM = 1,
                .fixedParent = 1,
                .sensitiveDataOrigin = 1,
                .sign = 1,
                .userWithAuth = 1,
            },
            .publicArea.parameters.symDetail.sym = {
                .algorithm = TPM_ALG_AES,
                .keyBits.sym = 128,
                .mode.sym = TPM_ALG_CFB,
            },
        }
    };

    TPM2B_DATA              outside_info    = { 0 };
    TPML_PCR_SELECTION      creation_pcr    = { 0 };
    TPM2B_PRIVATE           out_private     = TPM2B_PRIVATE_INIT;
    TPM2B_PUBLIC            out_public      = { 0 };
    TPM2B_CREATION_DATA     creation_data   = { 0 };
    TPM2B_DIGEST            creation_hash   = TPM2B_DIGEST_INIT;
    TPMT_TK_CREATION        creation_ticket = { 0 };
    TPM2B_NAME              name            = TPM2B_NAME_INIT;
    /* session parameters */
    /* command session info */
    TPMS_AUTH_COMMAND   session_cmd = { .sessionHandle = TPM_RS_PW };
    TPMS_AUTH_COMMAND  *session_cmd_array[1] = { &session_cmd };
    TSS2_SYS_CMD_AUTHS  sessions_cmd = {
        .cmdAuths      = session_cmd_array,
        .cmdAuthsCount = 1
    };
    /* response session info */
    TPMS_AUTH_RESPONSE  session_rsp          = { 0 };
    TPMS_AUTH_RESPONSE *session_rsp_array[1] = { &session_rsp };
    TSS2_SYS_RSP_AUTHS  sessions_rsp     = {
        .rspAuths      = session_rsp_array,
        .rspAuthsCount = 1
    };

    rc = TSS2_RETRY_EXP (Tss2_Sys_Create (sapi_context,
                                          handle_parent,
                                          &sessions_cmd,
                                          &in_sensitive,
                                          &in_public,
                                          &outside_info,
                                          &creation_pcr,
                                          &out_private,
                                          &out_public,
                                          &creation_data,
                                          &creation_hash,
                                          &creation_ticket,
                                          &sessions_rsp));
    if (rc != TPM_RC_SUCCESS) {
        return rc;
    }

    return Tss2_Sys_Load (sapi_context,
                          handle_parent,
                          &sessions_cmd,
                          &out_private,
                          &out_public,
                          handle,
                          &name,
                          &sessions_rsp);
}

TSS2_RC
encrypt_decrypt_cfb (
    TSS2_SYS_CONTEXT *sapi_context,
    TPMI_DH_OBJECT    handle,
    TPMI_YES_NO       decrypt,
    TPM2B_MAX_BUFFER *data_in,
    TPM2B_MAX_BUFFER *data_out)
{
    TPMI_ALG_SYM_MODE mode = TPM_ALG_NULL;
    TPM2B_IV iv_in = TPM2B_IV_INIT;
    TPM2B_IV iv_out = TPM2B_IV_INIT;

    /* session parameters */
    /* command session info */
    TPMS_AUTH_COMMAND   session_cmd = { .sessionHandle = TPM_RS_PW };
    TPMS_AUTH_COMMAND  *session_cmd_array[1] = { &session_cmd };
    TSS2_SYS_CMD_AUTHS  sessions_cmd = {
        .cmdAuths = session_cmd_array,
        .cmdAuthsCount = 1
    };
    /* response session info */
    TPMS_AUTH_RESPONSE  session_rsp = { 0 };
    TPMS_AUTH_RESPONSE *session_rsp_array[1] = { &session_rsp };
    TSS2_SYS_RSP_AUTHS  sessions_rsp = {
        .rspAuths = session_rsp_array,
        .rspAuthsCount = 1
    };

    return Tss2_Sys_EncryptDecrypt (sapi_context,
                                    handle,
                                    &sessions_cmd,
                                    decrypt,
                                    mode,
                                    &iv_in,
                                    data_in,
                                    data_out,
                                    &iv_out,
                                    &sessions_rsp);
}

TSS2_RC
decrypt_cfb (
    TSS2_SYS_CONTEXT *sapi_context,
    TPMI_DH_OBJECT    handle,
    TPM2B_MAX_BUFFER *data_in,
    TPM2B_MAX_BUFFER *data_out)
{
    return encrypt_decrypt_cfb (sapi_context, handle, YES, data_in, data_out);
}

TSS2_RC
encrypt_cfb (
    TSS2_SYS_CONTEXT *sapi_context,
    TPMI_DH_OBJECT    handle,
    TPM2B_MAX_BUFFER *data_in,
    TPM2B_MAX_BUFFER *data_out)
{
    return encrypt_decrypt_cfb (sapi_context, handle, NO, data_in, data_out);
}

TSS2_RC
encrypt_decrypt_2_cfb (
    TSS2_SYS_CONTEXT *sapi_context,
    TPMI_DH_OBJECT    handle,
    TPMI_YES_NO       decrypt,
    TPM2B_MAX_BUFFER *data_in,
    TPM2B_MAX_BUFFER *data_out)
{
    TPMI_ALG_SYM_MODE mode = TPM_ALG_NULL;
    TPM2B_IV iv_in = TPM2B_IV_INIT;
    TPM2B_IV iv_out = TPM2B_IV_INIT;

    /* session parameters */
    /* command session info */
    TPMS_AUTH_COMMAND   session_cmd = { .sessionHandle = TPM_RS_PW };
    TPMS_AUTH_COMMAND  *session_cmd_array[1] = { &session_cmd };
    TSS2_SYS_CMD_AUTHS  sessions_cmd = {
        .cmdAuths = session_cmd_array,
        .cmdAuthsCount = 1
    };
    /* response session info */
    TPMS_AUTH_RESPONSE  session_rsp = { 0 };
    TPMS_AUTH_RESPONSE *session_rsp_array[1] = { &session_rsp };
    TSS2_SYS_RSP_AUTHS  sessions_rsp = {
        .rspAuths = session_rsp_array,
        .rspAuthsCount = 1
    };

    return Tss2_Sys_EncryptDecrypt2 (sapi_context,
                                     handle,
                                     &sessions_cmd,
                                     data_in,
                                     decrypt,
                                     mode,
                                     &iv_in,
                                     data_out,
                                     &iv_out,
                                     &sessions_rsp);
}

TSS2_RC
decrypt_2_cfb (
    TSS2_SYS_CONTEXT *sapi_context,
    TPMI_DH_OBJECT    handle,
    TPM2B_MAX_BUFFER *data_in,
    TPM2B_MAX_BUFFER *data_out)
{
    return encrypt_decrypt_2_cfb (sapi_context, handle, YES, data_in, data_out);
}

TSS2_RC
encrypt_2_cfb (
    TSS2_SYS_CONTEXT *sapi_context,
    TPMI_DH_OBJECT    handle,
    TPM2B_MAX_BUFFER *data_in,
    TPM2B_MAX_BUFFER *data_out)
{
    return encrypt_decrypt_2_cfb (sapi_context, handle, NO, data_in, data_out);
}
