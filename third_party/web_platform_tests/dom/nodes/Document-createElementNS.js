var createElementNS_tests = [
  /* Arrays with three elements:
   *   the namespace argument
   *   the qualifiedName argument
   *   the expected exception, or null if none
   */
  [null, undefined, null],
  [null, "foo", null],
  [null, "1foo", "INVALID_CHARACTER_ERR"],
  [null, "f1oo", null],
  [null, "foo1", null],
  [null, "1foo", "INVALID_CHARACTER_ERR"],
  [null, "\u0300foo", "INVALID_CHARACTER_ERR"],
  [null, "}foo", "INVALID_CHARACTER_ERR"],
  [null, "f}oo", "INVALID_CHARACTER_ERR"],
  [null, "foo}", "INVALID_CHARACTER_ERR"],
  [null, "\uFFFFfoo", "INVALID_CHARACTER_ERR"],
  [null, "f\uFFFFoo", "INVALID_CHARACTER_ERR"],
  [null, "foo\uFFFF", "INVALID_CHARACTER_ERR"],
  [null, "<foo", "INVALID_CHARACTER_ERR"],
  [null, "foo>", "INVALID_CHARACTER_ERR"],
  [null, "<foo>", "INVALID_CHARACTER_ERR"],
  [null, "f<oo", "INVALID_CHARACTER_ERR"],
  [null, "^^", "INVALID_CHARACTER_ERR"],
  [null, ":foo", "NAMESPACE_ERR"],
  [null, "f:oo", "NAMESPACE_ERR"],
  [null, "foo:", "NAMESPACE_ERR"],
  [null, ":", "NAMESPACE_ERR"],
  [null, "xml", null],
  [null, "xmlns", "NAMESPACE_ERR"],
  [null, "xmlfoo", null],
  [null, "xml:foo", "NAMESPACE_ERR"],
  [null, "xmlns:foo", "NAMESPACE_ERR"],
  [null, "xmlfoo:bar", "NAMESPACE_ERR"],
  [null, "null:xml", "NAMESPACE_ERR"],
  ["", ":foo", "NAMESPACE_ERR"],
  ["", "f:oo", "NAMESPACE_ERR"],
  ["", "foo:", "NAMESPACE_ERR"],
  [undefined, undefined, null],
  [undefined, "foo", null],
  [undefined, "1foo", "INVALID_CHARACTER_ERR"],
  [undefined, "f1oo", null],
  [undefined, "foo1", null],
  [undefined, ":foo", "NAMESPACE_ERR"],
  [undefined, "f:oo", "NAMESPACE_ERR"],
  [undefined, "foo:", "NAMESPACE_ERR"],
  [undefined, "xml", null],
  [undefined, "xmlns", "NAMESPACE_ERR"],
  [undefined, "xmlfoo", null],
  [undefined, "xml:foo", "NAMESPACE_ERR"],
  [undefined, "xmlns:foo", "NAMESPACE_ERR"],
  [undefined, "xmlfoo:bar", "NAMESPACE_ERR"],
  ["http://example.com/", "foo", null],
  ["http://example.com/", "1foo", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "f1oo", null],
  ["http://example.com/", "foo1", null],
  ["http://example.com/", ":foo", "NAMESPACE_ERR"],
  ["http://example.com/", "f:oo", null],
  ["http://example.com/", "foo:", "NAMESPACE_ERR"],
  ["http://example.com/", "_:_", null],
  ["http://example.com/", "_:h0", null],
  ["http://example.com/", "_:test", null],
  ["http://example.com/", "l_:_", null],
  ["http://example.com/", "ns:_0", null],
  ["http://example.com/", "ns:a0", null],
  ["http://example.com/", "ns0:test", null],
  ["http://example.com/", "a.b:c", null],
  ["http://example.com/", "a-b:c", null],
  ["http://example.com/", "a-b:c", null],
  ["http://example.com/", "xml", null],
  ["http://example.com/", "xmlns", "NAMESPACE_ERR"],
  ["http://example.com/", "XMLNS", null],
  ["http://example.com/", "xmlfoo", null],
  ["http://example.com/", "xml:foo", "NAMESPACE_ERR"],
  ["http://example.com/", "XML:foo", null],
  ["http://example.com/", "xmlns:foo", "NAMESPACE_ERR"],
  ["http://example.com/", "XMLNS:foo", null],
  ["http://example.com/", "xmlfoo:bar", null],
  ["http://example.com/", "prefix::local", "NAMESPACE_ERR"],
  ["http://example.com/", "namespaceURI:{", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:}", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:~", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:'", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:!", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:@", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:#", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:$", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:%", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:^", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:&", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:*", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:(", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:)", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:+", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:=", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:[", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:]", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:\\", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:/", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:;", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:`", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:<", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:>", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:,", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:a ", "INVALID_CHARACTER_ERR"],
  ["http://example.com/", "namespaceURI:\"", "INVALID_CHARACTER_ERR"],
  ["/", "foo", null],
  ["/", "1foo", "INVALID_CHARACTER_ERR"],
  ["/", "f1oo", null],
  ["/", "foo1", null],
  ["/", ":foo", "NAMESPACE_ERR"],
  ["/", "f:oo", null],
  ["/", "foo:", "NAMESPACE_ERR"],
  ["/", "xml", null],
  ["/", "xmlns", "NAMESPACE_ERR"],
  ["/", "xmlfoo", null],
  ["/", "xml:foo", "NAMESPACE_ERR"],
  ["/", "xmlns:foo", "NAMESPACE_ERR"],
  ["/", "xmlfoo:bar", null],
  ["http://www.w3.org/XML/1998/namespace", "foo", null],
  ["http://www.w3.org/XML/1998/namespace", "1foo", "INVALID_CHARACTER_ERR"],
  ["http://www.w3.org/XML/1998/namespace", "f1oo", null],
  ["http://www.w3.org/XML/1998/namespace", "foo1", null],
  ["http://www.w3.org/XML/1998/namespace", ":foo", "NAMESPACE_ERR"],
  ["http://www.w3.org/XML/1998/namespace", "f:oo", null],
  ["http://www.w3.org/XML/1998/namespace", "foo:", "NAMESPACE_ERR"],
  ["http://www.w3.org/XML/1998/namespace", "xml", null],
  ["http://www.w3.org/XML/1998/namespace", "xmlns", "NAMESPACE_ERR"],
  ["http://www.w3.org/XML/1998/namespace", "xmlfoo", null],
  ["http://www.w3.org/XML/1998/namespace", "xml:foo", null],
  ["http://www.w3.org/XML/1998/namespace", "xmlns:foo", "NAMESPACE_ERR"],
  ["http://www.w3.org/XML/1998/namespace", "xmlfoo:bar", null],
  ["http://www.w3.org/XML/1998/namespaces", "xml:foo", "NAMESPACE_ERR"],
  ["http://www.w3.org/xml/1998/namespace", "xml:foo", "NAMESPACE_ERR"],
  ["http://www.w3.org/2000/xmlns/", "foo", "NAMESPACE_ERR"],
  ["http://www.w3.org/2000/xmlns/", "1foo", "INVALID_CHARACTER_ERR"],
  ["http://www.w3.org/2000/xmlns/", "f1oo", "NAMESPACE_ERR"],
  ["http://www.w3.org/2000/xmlns/", "foo1", "NAMESPACE_ERR"],
  ["http://www.w3.org/2000/xmlns/", ":foo", "NAMESPACE_ERR"],
  ["http://www.w3.org/2000/xmlns/", "f:oo", "NAMESPACE_ERR"],
  ["http://www.w3.org/2000/xmlns/", "foo:", "NAMESPACE_ERR"],
  ["http://www.w3.org/2000/xmlns/", "xml", "NAMESPACE_ERR"],
  ["http://www.w3.org/2000/xmlns/", "xmlns", null],
  ["http://www.w3.org/2000/xmlns/", "xmlfoo", "NAMESPACE_ERR"],
  ["http://www.w3.org/2000/xmlns/", "xml:foo", "NAMESPACE_ERR"],
  ["http://www.w3.org/2000/xmlns/", "xmlns:foo", null],
  ["http://www.w3.org/2000/xmlns/", "xmlfoo:bar", "NAMESPACE_ERR"],
  ["http://www.w3.org/2000/xmlns/", "foo:xmlns", "NAMESPACE_ERR"],
  ["foo:", "foo", null],
  ["foo:", "1foo", "INVALID_CHARACTER_ERR"],
  ["foo:", "f1oo", null],
  ["foo:", "foo1", null],
  ["foo:", ":foo", "NAMESPACE_ERR"],
  ["foo:", "f:oo", null],
  ["foo:", "foo:", "NAMESPACE_ERR"],
  ["foo:", "xml", null],
  ["foo:", "xmlns", "NAMESPACE_ERR"],
  ["foo:", "xmlfoo", null],
  ["foo:", "xml:foo", "NAMESPACE_ERR"],
  ["foo:", "xmlns:foo", "NAMESPACE_ERR"],
  ["foo:", "xmlfoo:bar", null],
]
