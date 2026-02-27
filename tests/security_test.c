#include <vibe_std.h>
#include <vibe_crypt.h>
#include <vibe_string.h>
#include <vibe_test.h>
#include <vibe_json.h>
#include <vibe_regex.h>

void test_null_checks() {
    // These should not crash
    VIBE_ASSERT(vibe_str_eq(NULL, "test") == false);
    VIBE_ASSERT(vibe_str_eq("test", NULL) == false);
    VIBE_ASSERT(vibe_str_eq(NULL, NULL) == true);
    VIBE_ASSERT(vibe_json_new_string(NULL) == NULL);
    vibe_json_print(NULL); // Should print "null"

    // Test regex NULL checks
    VIBE_ASSERT(vibe_regex_match(NULL, "test") == false);
    VIBE_ASSERT(vibe_regex_match(".*", NULL) == false);
    VIBE_ASSERT(vibe_regex_match(NULL, NULL) == false);

    VIBE_ASSERT(true); // If we reached here, no crash
}

void test_constant_time_eq() {
    VIBE_ASSERT(vibe_str_eq_constant_time("test", "test") == true);
    VIBE_ASSERT(vibe_str_eq_constant_time("test", "fail") == false);
    VIBE_ASSERT(vibe_str_eq_constant_time("test", "test1") == false);
    VIBE_ASSERT(vibe_str_eq_constant_time("test1", "test") == false);
    VIBE_ASSERT(vibe_str_eq_constant_time("", "") == true);
    VIBE_ASSERT(vibe_str_eq_constant_time("a", "a") == true);
    VIBE_ASSERT(vibe_str_eq_constant_time("a", "b") == false);
    VIBE_ASSERT(vibe_str_eq_constant_time(NULL, NULL) == true);
    VIBE_ASSERT(vibe_str_eq_constant_time(NULL, "test") == false);
}

void test_xor_cipher() {
    uint8_t data[] = "hello world";
    uint8_t key[] = "key";
    uint8_t expected[] = {
        'h' ^ 'k', 'e' ^ 'e', 'l' ^ 'y',
        'l' ^ 'k', 'o' ^ 'e', ' ' ^ 'y',
        'w' ^ 'k', 'o' ^ 'e', 'r' ^ 'y',
        'l' ^ 'k', 'd' ^ 'e'
    };
    size_t len = sizeof(data) - 1;

    // Encrypt
    vibe_xor_cipher(data, len, key, 3);
    for (size_t i = 0; i < len; i++) {
        VIBE_ASSERT(data[i] == expected[i]);
    }

    // Decrypt (XOR is its own inverse)
    vibe_xor_cipher(data, len, key, 3);
    VIBE_ASSERT(memcmp(data, "hello world", len) == 0);

    // Test with key_len = 1
    uint8_t data2[] = "aaaaa";
    uint8_t key2[] = {0xFF};
    vibe_xor_cipher(data2, 5, key2, 1);
    for (int i = 0; i < 5; i++) {
        VIBE_ASSERT(data2[i] == (uint8_t)('a' ^ 0xFF));
    }
}

int main() {
    test_null_checks();
    test_constant_time_eq();
    test_xor_cipher();
    VIBE_TEST_SUMMARY();
    return 0;
}
