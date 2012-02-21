// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_DNS_DNS_RESPONSE_H_
#define NET_DNS_DNS_RESPONSE_H_
#pragma once

#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/string_piece.h"
#include "base/time.h"
#include "net/base/net_export.h"
#include "net/base/net_util.h"

namespace net {

class AddressList;
class DnsQuery;
class IOBufferWithSize;

namespace dns_protocol {
struct Header;
}

// Parsed resource record.
struct NET_EXPORT_PRIVATE DnsResourceRecord {
  DnsResourceRecord();
  ~DnsResourceRecord();

  std::string name;  // in dotted form
  uint16 type;
  uint16 klass;
  uint32 ttl;
  base::StringPiece rdata;  // points to the original response buffer
};

// Iterator to walk over resource records of the DNS response packet.
class NET_EXPORT_PRIVATE DnsRecordParser {
 public:
  // Construct an uninitialized iterator.
  DnsRecordParser();

  // Construct an iterator to process the |packet| of given |length|.
  // |offset| points to the beginning of the answer section.
  DnsRecordParser(const void* packet, size_t length, size_t offset);

  // Returns |true| if initialized.
  bool IsValid() const { return packet_ != NULL; }

  // Returns |true| if no more bytes remain in the packet.
  bool AtEnd() const { return cur_ == packet_ + length_; }

  // Returns current offset into the packet.
  size_t GetOffset() const { return cur_ - packet_; }

  // Parses a (possibly compressed) DNS name from the packet starting at
  // |pos|. Stores output (even partial) in |out| unless |out| is NULL. |out|
  // is stored in the dotted form, e.g., "example.com". Returns number of bytes
  // consumed or 0 on failure.
  // This is exposed to allow parsing compressed names within RRDATA for TYPEs
  // such as NS, CNAME, PTR, MX, SOA.
  // See RFC 1035 section 4.1.4.
  unsigned ReadName(const void* pos, std::string* out) const;

  // Parses the next resource record into |record|. Returns true if succeeded.
  bool ReadRecord(DnsResourceRecord* record);

 private:
  const char* packet_;
  size_t length_;
  // Current offset within the packet.
  const char* cur_;
};

// Buffer-holder for the DNS response allowing easy access to the header fields
// and resource records. After reading into |io_buffer| must call InitParse to
// position the RR parser.
class NET_EXPORT_PRIVATE DnsResponse {
 public:
  // Possible results from ParseToAddressList
  enum Result {
    DNS_SUCCESS = 0,
    DNS_MALFORMED_RESPONSE,    // DnsRecordParser failed before the end of
                               // packet.
    DNS_MALFORMED_CNAME,       // Could not parse CNAME out of RRDATA.
    DNS_NAME_MISMATCH,         // Got an address but no ordered chain of CNAMEs
                               // leads there.
    DNS_SIZE_MISMATCH,         // Got an address but size does not match.
    DNS_CNAME_AFTER_ADDRESS,   // Found CNAME after an address record.
    DNS_ADDRESS_TTL_MISMATCH,  // TTL of all address records are not identical.
    DNS_NO_ADDRESSES,          // No address records found.
  };

  // Constructs an object with an IOBuffer large enough to read
  // one byte more than largest possible response, to detect malformed
  // responses.
  DnsResponse();
  // Constructs response from |data|. Used for testing purposes only!
  DnsResponse(const void* data, size_t length, size_t answer_offset);
  ~DnsResponse();

  // Internal buffer accessor into which actual bytes of response will be
  // read.
  IOBufferWithSize* io_buffer() { return io_buffer_.get(); }

  // Returns false if the packet is shorter than the header or does not match
  // |query| id or question.
  bool InitParse(int nbytes, const DnsQuery& query);

  // Returns true if response is valid, that is, after successful InitParse.
  bool IsValid() const;

  // All of the methods below are valid only if the response is valid.

  // Accessors for the header.
  uint16 flags() const;  // excluding rcode
  uint8 rcode() const;
  unsigned answer_count() const;

  // Accessors to the question. The qname is unparsed.
  base::StringPiece qname() const;
  uint16 qtype() const;

  // Returns qname in dotted format.
  std::string GetDottedName() const;

  // Returns an iterator to the resource records in the answer section.
  // The iterator is valid only in the scope of the DnsResponse.
  // This operation is idempotent.
  DnsRecordParser Parser() const;

  // Extracts an AddressList from this response. Returns SUCCESS if succeeded.
  // Otherwise returns a detailed error number.
  Result ParseToAddressList(AddressList* addr_list, base::TimeDelta* ttl) const;

 private:
  // Convenience for header access.
  const dns_protocol::Header* header() const;

  // Buffer into which response bytes are read.
  scoped_refptr<IOBufferWithSize> io_buffer_;

  // Iterator constructed after InitParse positioned at the answer section.
  // It is never updated afterwards, so can be used in accessors.
  DnsRecordParser parser_;

  DISALLOW_COPY_AND_ASSIGN(DnsResponse);
};

}  // namespace net

#endif  // NET_DNS_DNS_RESPONSE_H_
