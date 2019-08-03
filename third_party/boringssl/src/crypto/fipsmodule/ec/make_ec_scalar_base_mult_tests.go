/* Copyright (c) 2018, Google Inc.
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
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. */

package main

import (
	"crypto/elliptic"
	"fmt"
	"math/big"
)

const numPoints = 64

func printPadded(key string, n, max *big.Int) {
	padded := make([]byte, len(max.Bytes()))
	b := n.Bytes()
	copy(padded[len(padded)-len(b):], b)
	fmt.Printf("%s = %x\n", key, padded)
}

func printMultiples(name string, curve elliptic.Curve) {
	n := new(big.Int)
	for i := -numPoints; i <= numPoints; i++ {
		fmt.Printf("Curve = %s\n", name)
		n.SetInt64(int64(i))
		if i < 0 {
			n = n.Add(n, curve.Params().N)
		}
		fmt.Printf("# N = %d\n", i)
		printPadded("N", n, curve.Params().N)
		x, y := curve.ScalarBaseMult(n.Bytes())
		printPadded("X", x, curve.Params().P)
		printPadded("Y", y, curve.Params().P)
		fmt.Printf("\n")
	}
}

func main() {
	fmt.Printf(`# This file contains multiples of the base point for various curves. The point
# at infinity is represented as X = 0, Y = 0.
#
# This file is generated by make_ec_scalar_base_mult_tests.go

`)
	printMultiples("P-224", elliptic.P224())
	printMultiples("P-256", elliptic.P256())
	printMultiples("P-384", elliptic.P384())
	printMultiples("P-521", elliptic.P521())
}
