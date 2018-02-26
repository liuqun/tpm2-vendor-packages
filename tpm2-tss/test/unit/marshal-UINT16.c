#include <stdlib.h>
#include <arpa/inet.h>
#include <stdio.h>

#include <setjmp.h>
#include <cmocka.h>
#include "sapi/tpm20.h"

#include "sys_api_marshalUnmarshal.h"
#include "sysapi_util.h"

typedef struct {
    uint8_t *buffer;
    size_t   buffer_size;
    uint16_t data_host;
    uint16_t data_net;
    TSS2_RC  rc;
} marshal_uint16_data_t;

static int
marshal_UINT16_setup (void **state)
{
    marshal_uint16_data_t *data;

    data              = calloc (1, sizeof (marshal_uint16_data_t));
    data->buffer      = calloc (1, sizeof (uint16_t));
    data->buffer_size = sizeof (uint16_t);
    data->data_host   = 0xdead;
    data->data_net    = htons (data->data_host);
    data->rc          = TSS2_RC_SUCCESS;

    *state = data;
    return 0;
}

static int
marshal_UINT16_teardown (void **state)
{
    marshal_uint16_data_t *data;

    data = (marshal_uint16_data_t*)*state;
    if (data) {
        if (data->buffer)
            free (data->buffer);
        free (data);
    }

    return 0;
}
/**
 * Make a call to Marshal_UINT16 function that should succeed. The *_setup
 * function is expected to have allocated sufficient buffer to hold a
 * uint16_t. This test just 'marshals' a known uint16_t into this data buffer
 * and then compares the value to the expected result.
 * The workings of the Marshal_UINT16 function is a bit complex, so we
 * assert the expected results as well.
 */
void
marshal_UINT16_good (void **state)
{
    marshal_uint16_data_t *data;

    data = (marshal_uint16_data_t*)*state;
    uint8_t *nextData = data->buffer;

    Marshal_UINT16 (data->buffer,
                    data->buffer_size,
                    &nextData,
                    data->data_host,
                    &data->rc);
    /**
     * uint16_t that was marshalled into the data buffer should be equal to
     * the expected value (data converted from host byte order to network
     * byte order).
     */
    assert_int_equal (*(uint16_t*)(data->buffer), data->data_net);
    /**
     * The Marshal_* functions advance the 'nextData' parameter by the size of
     * the marshalled data.
     */
    assert_int_equal (data->buffer, nextData - sizeof (uint16_t));
    /* Finally the return code should indicate success. */
    assert_int_equal (data->rc, TSS2_RC_SUCCESS);
}
/**
 * Attempt to marshal a uint16_t into a buffer that's only got room for a
 * uint8_t. The call to the marshalling function should set the TSS2_RC value
 * to TSS2_SYS_RC_INSUFFICIENT_CONTEXT and the nextData parameter should not
 * be changed.
 */
void
marshal_UINT16_too_small (void **state)
{
    marshal_uint16_data_t *data;

    data = (marshal_uint16_data_t*)*state;
    /**
     * Set the locate where data will be marshalled to half way through the
     * buffer. This only leaves us space for a uint8_t.
     */
    uint8_t *nextData = data->buffer + sizeof (uint8_t);
    data->buffer_size = sizeof (uint8_t);

    Marshal_UINT16 (data->buffer,
                    data->buffer_size,
                    &nextData,
                    data->data_host,
                    &data->rc);
    /**
     * The return code should indicate we don't have enough space and the
     * nextData pointer shouldn't have moved.
     */
    assert_int_equal (data->rc, TSS2_SYS_RC_INSUFFICIENT_CONTEXT);
    assert_int_equal (data->buffer, nextData - sizeof (uint8_t));
}
void
marshal_UINT16_under_ptr (void **state)
{
    marshal_uint16_data_t *data;

    data = (marshal_uint16_data_t*)*state;
    /**
     * Set nextData to a byte *before* the buffer.
     */
    uint8_t *nextData = data->buffer - sizeof (uint8_t);
    Marshal_UINT16 (data->buffer,
                    data->buffer_size,
                    &nextData,
                    data->data_host,
                    &data->rc);
    /* again, not enough space, and no change to nextData */
    assert_int_equal (data->rc, TSS2_SYS_RC_INSUFFICIENT_CONTEXT);
    assert_int_equal (data->buffer, nextData - sizeof (uint8_t));
}
/**
 * Test error condition where nextData pointer is already beyond the end
 * of the data buffer. We set this value to 1 byte past the buffers end.
 * The expected result is the 'rc' parameter being set to indicate the
 * size of the buffer is insufficient. The nextData pointer should also
 * remain unchanged.
 */
void
marshal_UINT16_past_end (void **state)
{
    marshal_uint16_data_t *data;

    data = (marshal_uint16_data_t*)*state;
    /* Set nextData beyond the end of the buffer. */
    uint8_t *nextData = data->buffer + data->buffer_size + sizeof (uint8_t);
    Marshal_UINT16 (data->buffer,
                    data->buffer_size,
                    &nextData,
                    data->data_host,
                    &data->rc);
    /* rc should indicate error and no change to the pointers */
    assert_int_equal (data->rc, TSS2_SYS_RC_INSUFFICIENT_CONTEXT);
    assert_int_equal (nextData,
                      data->buffer + data->buffer_size + sizeof (uint8_t));
}
/**
 * Test an edge case where the 'rc' parameter is set to something other
 * than TSS2_RC_SUCCESS. A pattern I see emerging is the use of the TSS2_RC
 * member of the sapi context structure to skip over large blocks of code.
 * Many (most?) of the functions in the guts of the serialization logic check
 * this member data and quickly return when it's set. This is the reason
 * these fucntions often have no return value: they're using the 'rc' member
 * in the context structure.
 */
void
marshal_UINT16_rc_previous_fail (void **state)
{
    /* Set 'rc' to an error condition. */
    TSS2_RC rc = TSS2_SYS_RC_BAD_SIZE;
    /* 'rc' is checked first, all other parameters can be NULL.*/
    Marshal_UINT16 (NULL, 0, NULL, 0, &rc);
    assert_int_equal (rc, TSS2_SYS_RC_BAD_SIZE);
}
int
main (void)
{
    const struct CMUnitTest tests [] = {
        cmocka_unit_test_setup_teardown (marshal_UINT16_good,
                                         marshal_UINT16_setup,
                                         marshal_UINT16_teardown),
        cmocka_unit_test_setup_teardown (marshal_UINT16_too_small,
                                         marshal_UINT16_setup,
                                         marshal_UINT16_teardown),
        cmocka_unit_test_setup_teardown (marshal_UINT16_past_end,
                                         marshal_UINT16_setup,
                                         marshal_UINT16_teardown),
        cmocka_unit_test (marshal_UINT16_rc_previous_fail),
    };
    return cmocka_run_group_tests (tests, NULL, NULL);
}
