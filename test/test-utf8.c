/*-*- Mode: C; c-basic-offset: 8; indent-tabs-mode: nil -*-*/

/***
  This file is part of systemd.

  Copyright 2013 Dave Reisner

  systemd is free software; you can redistribute it and/or modify it
  under the terms of the GNU Lesser General Public License as published by
  the Free Software Foundation; either version 2.1 of the License, or
  (at your option) any later version.

  systemd is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public License
  along with systemd; If not, see <http://www.gnu.org/licenses/>.
***/


#include "utf8.h"
#include "util.h"

/* helpers for test_udev_encode_string */
static char *do_encode_string(const char *in) {
        size_t out_len = strlen(in) * 4;
        char *out = malloc(out_len);

        assert_se(out);
        assert_se(udev_encode_string(in, out, out_len) >= 0);
        puts(out);

        return out;
}

static bool expect_encoded_as(const char *in, const char *expected) {
        _cleanup_free_ char *encoded = do_encode_string(in);
        return streq(encoded, expected);
}

static void test_udev_encode_string(void) {
        assert_se(expect_encoded_as("systemd sucks", "systemd\\x20sucks"));
        assert_se(expect_encoded_as("pinkiepie", "pinkiepie"));
        assert_se(expect_encoded_as("valíd\\ųtf8", "valíd\\x5cųtf8"));
        assert_se(expect_encoded_as("s/ash/ng", "s\\x2fash\\x2fng"));
}

static void test_utf8_is_valid(void) {
        assert_se(utf8_is_valid("ascii is valid unicode"));
        assert_se(utf8_is_valid("\341\204\242"));
        assert_se(!utf8_is_valid("\341\204"));
}

int main(int argc, char *argv[]) {
        test_utf8_is_valid();
        test_udev_encode_string();
}
