#;**********************************************************************;
# Copyright (c) 2015, 2016, 2017 Intel Corporation
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without 
# modification, are permitted provided that the following conditions are met:
# 
# 1. Redistributions of source code must retain the above copyright notice, 
# this list of conditions and the following disclaimer.
# 
# 2. Redistributions in binary form must reproduce the above copyright notice, 
# this list of conditions and the following disclaimer in the documentation 
# and/or other materials provided with the distribution.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF 
# THE POSSIBILITY OF SUCH DAMAGE.
#;**********************************************************************;

include src_vars.mk

INCLUDE_DIRS    = -I$(srcdir)/include -I$(srcdir)/sysapi/include
ACLOCAL_AMFLAGS = -I m4
AM_CFLAGS       = $(INCLUDE_DIRS) $(EXTRA_CFLAGS)
AM_CXXFLAGS     = $(AM_CFLAGS)
AM_LDFLAGS      = $(EXTRA_LDFLAGS)

# stuff to build, what that stuff is, and where/if to install said stuff
lib_LTLIBRARIES = $(libsapi) $(libtcti_device) $(libtcti_socket)
noinst_LTLIBRARIES = test/integration/libtest_utils.la

# test harness configuration
TEST_EXTENSIONS = .int
INT_LOG_COMPILER = $(srcdir)/script/int-log-compiler.sh
INT_LOG_FLAGS = --simulator-bin=$(SIMULATOR_BIN)

check_PROGRAMS = $(TESTS_UNIT) $(TESTS_INTEGRATION)
TESTS = $(check_PROGRAMS)
if UNIT
TESTS_UNIT  = \
    test/unit/CheckOverflow \
    test/unit/CommonPreparePrologue \
    test/unit/CopyCommandHeader \
    test/unit/GetNumHandles \
    test/unit/marshal-TPM2B-simple \
    test/unit/marshal-UINT16 \
    test/unit/marshal-UINT32 \
    test/unit/tcti-device \
    test/unit/tcti-socket \
    test/unit/unmarshal-UINT16 \
    test/unit/unmarshal-UINT32
endif #UNIT
if SIMULATOR_BIN
TESTS_INTEGRATION = \
    test/integration/asymmetric-encrypt-decrypt.int \
    test/integration/create-primary-rsa-2048-aes-128-cfb.int \
    test/integration/create-keyedhash-sha1-hmac.int \
    test/integration/encrypt-decrypt.int \
    test/integration/encrypt-decrypt-2.int \
    test/integration/evict-ctrl.int \
    test/integration/get-random.int \
    test/integration/pcr-extension.int \
    test/integration/self-test.int \
    test/integration/start-auth-session.int \
    test/tpmclient/tpmclient.int
endif #SIMULATOR_BIN

CLEANFILES = \
    $(nodist_pkgconfig_DATA) \
    $(man3_MANS) \
    $(man7_MANS) \
    test/integration/*.log \
    test/tpmclient/*.log \
    test/unit/*.log

# headers and where to install them
libsapidir      = $(includedir)/sapi
libsapi_HEADERS = $(srcdir)/include/sapi/*.h
libtctidir      = $(includedir)/tcti
libtcti_HEADERS = $(srcdir)/include/tcti/*.h
# pkg-config files
pkgconfigdir          = $(libdir)/pkgconfig
nodist_pkgconfig_DATA = lib/sapi.pc lib/tcti-device.pc lib/tcti-socket.pc
# man pages / documentation
man3_MANS = man/man3/InitDeviceTcti.3 man/man3/InitSocketTcti.3
man7_MANS = man/man7/tcti-device.7 man/man7/tcti-socket.7

EXTRA_DIST = \
    AUTHORS \
    CHANGELOG.md \
    CONTRIBUTING.md \
    INSTALL \
    LICENSE \
    README.md \
    RELEASE.md \
    VERSION \
    lib/debug_config.site \
    lib/tcti-device.pc.in \
    lib/tcti-socket.pc.in \
    lib/sapi.pc.in \
    man/man-postlude.troff \
    man/InitDeviceTcti.3.in \
    man/InitSocketTcti.3.in \
    man/tcti-device.7.in \
    man/tcti-socket.7.in \
    $(INT_LOG_COMPILER) \
    tcti/tcti_device.map \
    tcti/tcti_socket.map

if UNIT
test_unit_tcti_device_CFLAGS  = $(CMOCKA_CFLAGS) $(AM_CFLAGS)
test_unit_tcti_device_LDADD   = $(CMOCKA_LIBS)
test_unit_tcti_device_LDFLAGS = -Wl,--wrap=read -Wl,-wrap=write
test_unit_tcti_device_SOURCES = tcti/commonchecks.c tcti/tcti_device.c \
    ./sysapi/sysapi_util/changeEndian.c test/unit/tcti-device.c

test_unit_tcti_socket_CFLAGS  = $(CMOCKA_CFLAGS) $(AM_CFLAGS)
test_unit_tcti_socket_LDADD   = $(CMOCKA_LIBS) $(libmarshal)
test_unit_tcti_socket_LDFLAGS = -Wl,--wrap=connect,--wrap=recv,--wrap=select,--wrap=send
test_unit_tcti_socket_SOURCES = tcti/platformcommand.c tcti/tcti_socket.cpp \
    tcti/commonchecks.c tcti/commonchecks.h tcti/sockets.cpp tcti/sockets.h \
    common/debug.c common/debug.h tcti/logging.h test/unit/tcti-socket.c \
    sysapi/sysapi_util/changeEndian.c sysapi/include/endianConv.h

test_unit_CommonPreparePrologue_CFLAGS = $(CMOCKA_CFLAGS) $(AM_CFLAGS)
test_unit_CommonPreparePrologue_LDFLAGS = -Wl,--unresolved-symbols=ignore-all
test_unit_CommonPreparePrologue_LDADD = $(CMOCKA_LIBS) $(libsapi)
test_unit_CommonPreparePrologue_SOURCES = \
    test/unit/CommonPreparePrologue.c sysapi/sysapi_util/CommandUtil.c \
    sysapi/sysapi/ContextManagement.c

test_unit_GetNumHandles_CFLAGS  = $(CMOCKA_CFLAGS) $(AM_CFLAGS)
test_unit_GetNumHandles_LDADD   = $(CMOCKA_LIBS) $(libsapi)
test_unit_GetNumHandles_SOURCES = test/unit/GetNumHandles.c

test_unit_CopyCommandHeader_CFLAGS = $(CMOCKA_CFLAGS) $(AM_CFLAGS)
test_unit_CopyCommandHeader_LDFLAGS = -Wl,--unresolved-symbols=ignore-all
test_unit_CopyCommandHeader_LDADD = $(CMOCKA_LIBS) $(libsapi)
test_unit_CopyCommandHeader_SOURCES = \
    test/unit/CopyCommandHeader.c sysapi/sysapi_util/CommandUtil.c \
    sysapi/sysapi/ContextManagement.c sysapi/sysapi_util/changeEndian.c

test_unit_marshal_UINT16_CFLAGS  = $(CMOCKA_CFLAGS) $(AM_CFLAGS)
test_unit_marshal_UINT16_LDADD   = $(CMOCKA_LIBS)
test_unit_marshal_UINT16_SOURCES = \
    sysapi/sysapi_util/changeEndian.c \
    sysapi/sysapi_util/checkoverflow.c \
    sysapi/sysapi_util/marshal_uint16.c \
    test/unit/marshal-UINT16.c

test_unit_unmarshal_UINT16_CFLAGS  = $(CMOCKA_CFLAGS) $(AM_CFLAGS)
test_unit_unmarshal_UINT16_LDADD   = $(CMOCKA_LIBS)
test_unit_unmarshal_UINT16_SOURCES = \
    sysapi/sysapi_util/changeEndian.c \
    sysapi/sysapi_util/checkoverflow.c \
    sysapi/sysapi_util/unmarshal_uint16.c \
    test/unit/unmarshal-UINT16.c

test_unit_marshal_UINT32_CFLAGS  = $(CMOCKA_CFLAGS) $(AM_CFLAGS)
test_unit_marshal_UINT32_LDADD   = $(CMOCKA_LIBS)
test_unit_marshal_UINT32_SOURCES = \
    sysapi/sysapi_util/changeEndian.c \
    sysapi/sysapi_util/checkoverflow.c \
    sysapi/sysapi_util/marshal_uint32.c \
    test/unit/marshal-UINT32.c

test_unit_unmarshal_UINT32_CFLAGS  = $(CMOCKA_CFLAGS) $(AM_CFLAGS)
test_unit_unmarshal_UINT32_LDADD   = $(CMOCKA_LIBS)
test_unit_unmarshal_UINT32_SOURCES = \
    sysapi/sysapi_util/changeEndian.c \
    sysapi/sysapi_util/checkoverflow.c \
    sysapi/sysapi_util/unmarshal_uint32.c \
    test/unit/unmarshal-UINT32.c

test_unit_marshal_TPM2B_simple_CFLAGS  = $(CMOCKA_CFLAGS) $(AM_CFLAGS)
test_unit_marshal_TPM2B_simple_LDADD   = $(CMOCKA_LIBS)
test_unit_marshal_TPM2B_simple_SOURCES = \
    sysapi/sysapi_util/changeEndian.c \
    sysapi/sysapi_util/checkoverflow.c \
    sysapi/sysapi_util/marshal_uint8.c \
    sysapi/sysapi_util/unmarshal_uint8.c \
    sysapi/sysapi_util/marshal_uint16.c \
    sysapi/sysapi_util/unmarshal_uint16.c \
    sysapi/sysapi_util/marshal_uint32.c \
    sysapi/sysapi_util/CopySessionData.c \
    sysapi/sysapi_util/marshal_simple_tpm2b.c \
    sysapi/sysapi_util/unmarshal_simple_tpm2b.c \
    sysapi/sysapi_util/unmarshal_simple_tpm2b_no_size_check.c \
    test/unit/marshal-TPM2B-simple.c

test_unit_CheckOverflow_CFLAGS  = $(CMOCKA_CFLAGS) $(AM_CFLAGS)
test_unit_CheckOverflow_LDADD   = $(CMOCKA_LIBS)
test_unit_CheckOverflow_SOURCES = \
    sysapi/sysapi_util/checkoverflow.c \
    test/unit/CheckOverflow.c
endif # UNIT

# how to build stuff
sysapi_libsapi_la_LDFLAGS = $(LIBRARY_LDFLAGS)
sysapi_libsapi_la_SOURCES = $(SYSAPI_C) $(SYSAPI_H) $(SYSAPIUTIL_C) \
    $(SYSAPIUTIL_H)

tcti_libtcti_device_la_CFLAGS   = $(AM_CFLAGS)
tcti_libtcti_device_la_LDFLAGS  = -Wl,--version-script=$(srcdir)/tcti/tcti_device.map
tcti_libtcti_device_la_SOURCES  = tcti/tcti_device.c tcti/commonchecks.c \
    tcti/commonchecks.h common/debug.c common/debug.h tcti/logging.h \
    sysapi/sysapi_util/changeEndian.c

tcti_libtcti_socket_la_CFLAGS   = $(AM_CFLAGS)
tcti_libtcti_socket_la_CXXFLAGS = $(AM_CXXFLAGS)
tcti_libtcti_socket_la_LDFLAGS  = -Wl,--version-script=$(srcdir)/tcti/tcti_socket.map
tcti_libtcti_socket_la_SOURCES  = tcti/platformcommand.c tcti/tcti_socket.cpp \
    tcti/commonchecks.c tcti/commonchecks.h tcti/sockets.cpp tcti/sockets.h \
    common/debug.c common/debug.h tcti/logging.h

test_tpmclient_tpmclient_int_CFLAGS   = $(AM_CFLAGS) -DNO_RM_TESTS
test_tpmclient_tpmclient_int_CXXFLAGS = $(AM_CXXFLAGS) -DNO_RM_TESTS
test_tpmclient_tpmclient_int_LDADD    = $(TESTS_LDADD)
test_tpmclient_tpmclient_int_SOURCES  = $(COMMON_C) \
    test/tpmclient/CatSizedByteBuffer.c test/tpmclient/CopySizedBuffer.c \
    test/tpmclient/CreateNullSession.c test/tpmclient/DecryptEncrypt.c \
    test/tpmclient/Entity.c test/tpmclient/kdfa.c \
    test/tpmclient/LoadExternalHMACKey.c test/integration/main.c \
    test/tpmclient/sample.h \
    test/tpmclient/SessionHmac.c test/tpmclient/SetLocality.c \
    test/tpmclient/StartAuthSession.c test/tpmclient/syscontext.c \
    test/tpmclient/syscontext.h test/tpmclient/tcti_util.c \
    test/tpmclient/tcti_util.h test/tpmclient/TpmCalcPHash.c \
    test/tpmclient/tpmclient.int.cpp test/tpmclient/tpmclient.h \
    test/tpmclient/tpmclient_wo_rm.h test/tpmclient/TpmHandleToName.c \
    test/tpmclient/TpmHash.c test/tpmclient/TpmHmac.c

test_integration_libtest_utils_la_SOURCES = \
    test/integration/context-util.c test/integration/context-util.h \
    test/integration/log.h \
    test/integration/sapi-util.c    test/integration/sapi-util.h \
    test/integration/test-options.c test/integration/test-options.h \
    test/integration/test.h

test_integration_asymmetric_encrypt_decrypt_int_LDADD   = $(TESTS_LDADD)
test_integration_asymmetric_encrypt_decrypt_int_SOURCES = \
    test/integration/asymmetric-encrypt-decrypt.int.c \
    test/integration/main.c

test_integration_create_primary_rsa_2048_aes_128_cfb_int_LDADD = $(TESTS_LDADD)
test_integration_create_primary_rsa_2048_aes_128_cfb_int_SOURCES = \
    test/integration/create-primary-rsa-2048-aes-128-cfb.int.c \
    test/integration/main.c

test_integration_create_keyedhash_sha1_hmac_int_LDADD   = $(TESTS_LDADD)
test_integration_create_keyedhash_sha1_hmac_int_SOURCES = \
    test/integration/create-keyedhash-sha1-hmac.int.c \
    test/integration/main.c

test_integration_encrypt_decrypt_int_LDADD   = $(TESTS_LDADD)
test_integration_encrypt_decrypt_int_SOURCES = \
    test/integration/encrypt-decrypt.int.c \
    test/integration/main.c

test_integration_encrypt_decrypt_2_int_LDADD   = $(TESTS_LDADD)
test_integration_encrypt_decrypt_2_int_SOURCES = \
    test/integration/encrypt-decrypt-2.int.c \
    test/integration/main.c

test_integration_evict_ctrl_int_LDADD   = $(TESTS_LDADD)
test_integration_evict_ctrl_int_SOURCES = \
    test/integration/evict-ctrl.int.c \
    test/integration/main.c

test_integration_get_random_int_LDADD   = $(TESTS_LDADD)
test_integration_get_random_int_SOURCES = test/integration/get-random.int.c \
    test/integration/main.c

test_integration_pcr_extension_int_LDADD   = $(TESTS_LDADD)
test_integration_pcr_extension_int_SOURCES = test/integration/pcr-extension.int.c \
    test/integration/main.c

test_integration_self_test_int_LDADD   = $(TESTS_LDADD)
test_integration_self_test_int_SOURCES = test/integration/self-test.int.c \
    test/integration/main.c

test_integration_start_auth_session_int_LDADD = $(TESTS_LDADD)
test_integration_start_auth_session_int_SOURCES = test/integration/main.c \
    test/integration/start-auth-session.int.c

TESTS_LDADD = $(noinst_LTLIBRARIES) $(lib_LTLIBRARIES)

%.pc : %.pc.in
	$(call make_parent_dir,$@)
	sed -e "s,[@]VERSION[@],$(PACKAGE_VERSION),g; \
	        s,[@]includedir[@],$(includedir),g;" $^ > $@

man/man3/%.3 : man/%.3.in $(srcdir)/man/man-postlude.troff
	$(call make_man,$@,$<,$(srcdir)/man/man-postlude.troff)

man/man7/%.7 : man/%.7.in $(srcdir)/man/man-postlude.troff
	$(call make_man,$@,$<,$(srcdir)/man/man-postlude.troff)

# simple variables
TCTICOMMON_INC = -I$(srcdir)/include -I$(srcdir)/common \
    -I$(srcdir)/sysapi/include
TCTICOMMON_C   = tcti/commonchecks.c

TCTIDEVICE_INC = $(TCTICOMMON_INC)
TCTIDEVICE_C   = tcti/tcti_device.c

TCTISOCKET_INC = $(TCTICOMMON_INC)
TCTISOCKET_C   = tcti/platformcommand.c
TCTISOCKET_CXX = tcti/tcti_socket.cpp

libsapi = sysapi/libsapi.la
libtcti_device = tcti/libtcti-device.la
libtcti_socket = tcti/libtcti-socket.la

define make_parent_dir
    if [ ! -d $(dir $1) ]; then mkdir -p $(dir $1); fi
endef
# function to transform man .in files to man pages
# $1: target
# $2: .in file
# $3: man postlude file
define make_man
    $(call make_parent_dir,$1)
    cat $2 $3 > $1
    sed -i -e "s,[@]VERSION[@],$(PACKAGE_VERSION),g;" $1
endef
