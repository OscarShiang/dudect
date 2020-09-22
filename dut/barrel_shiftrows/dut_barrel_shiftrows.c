#include <stdint.h>
#include <stdlib.h>
#include <string.h>  // memcmp
#include "aes.h"
#include "dut.h"
#include "random.h"
#if 0
#include "rijndael-alg-fst.h"
#endif

static uint32_t rk[352] = {0};
const size_t chunk_size = 128;
const size_t number_measurements = 1e6;  // per test

uint8_t do_one_computation(uint8_t *data)
{
    uint8_t in[128] = {0};
    uint8_t out[128] = {0};
    uint8_t ret = 0;

    memcpy(in, data, 16);

    // chain some encryptions
    for (int i = 0; i < 10; i++) {
        // rijndaelEncrypt(rk, 10, in, out);
        aes128_encrypt(out, in, rk);
        memcpy(in, out, chunk_size);
    }

    ret ^= out[0];
    return ret;
}

void init_dut(void)
{
    uint8_t key[16] = {0};
    // rijndaelKeySetupEnc(rk, cipherKey, 128);
    aes128_keyschedule_lut(rk, key);
}

void prepare_inputs(uint8_t *input_data, uint8_t *classes)
{
    randombytes(input_data, number_measurements * chunk_size);
    for (size_t i = 0; i < number_measurements; i++) {
        classes[i] = randombit();
        if (classes[i] == 0) {
            memset(input_data + (size_t) i * chunk_size, 0x00, chunk_size);
        } else {
            // leave random
        }
    }
}
