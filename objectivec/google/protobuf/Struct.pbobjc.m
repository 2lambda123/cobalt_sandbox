// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: google/protobuf/struct.proto

#import "GPBProtocolBuffers_RuntimeSupport.h"
#import "google/protobuf/Struct.pbobjc.h"
// @@protoc_insertion_point(imports)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"

#pragma mark - GPBStructRoot

@implementation GPBStructRoot

@end

#pragma mark - GPBStructRoot_FileDescriptor

static GPBFileDescriptor *GPBStructRoot_FileDescriptor(void) {
  // This is called by +initialize so there is no need to worry
  // about thread safety of the singleton.
  static GPBFileDescriptor *descriptor = NULL;
  if (!descriptor) {
    GPBDebugCheckRuntimeVersion();
    descriptor = [[GPBFileDescriptor alloc] initWithPackage:@"google.protobuf"
                                                     syntax:GPBFileSyntaxProto3];
  }
  return descriptor;
}

#pragma mark - Enum GPBNullValue

GPBEnumDescriptor *GPBNullValue_EnumDescriptor(void) {
  static GPBEnumDescriptor *descriptor = NULL;
  if (!descriptor) {
    static const char *valueNames =
        "NullValue\000";
    static const int32_t values[] = {
        GPBNullValue_NullValue,
    };
    GPBEnumDescriptor *worker =
        [GPBEnumDescriptor allocDescriptorForName:GPBNSStringifySymbol(GPBNullValue)
                                       valueNames:valueNames
                                           values:values
                                            count:(uint32_t)(sizeof(values) / sizeof(int32_t))
                                     enumVerifier:GPBNullValue_IsValidValue];
    if (!OSAtomicCompareAndSwapPtrBarrier(nil, worker, (void * volatile *)&descriptor)) {
      [worker release];
    }
  }
  return descriptor;
}

BOOL GPBNullValue_IsValidValue(int32_t value__) {
  switch (value__) {
    case GPBNullValue_NullValue:
      return YES;
    default:
      return NO;
  }
}

#pragma mark - GPBStruct

@implementation GPBStruct

@dynamic fields, fields_Count;

typedef struct GPBStruct__storage_ {
  uint32_t _has_storage_[1];
  NSMutableDictionary *fields;
} GPBStruct__storage_;

// This method is threadsafe because it is initially called
// in +initialize for each subclass.
+ (GPBDescriptor *)descriptor {
  static GPBDescriptor *descriptor = nil;
  if (!descriptor) {
    static GPBMessageFieldDescription fields[] = {
      {
        .name = "fields",
        .dataTypeSpecific.className = GPBStringifySymbol(GPBValue),
        .number = GPBStruct_FieldNumber_Fields,
        .hasIndex = GPBNoHasBit,
        .offset = (uint32_t)offsetof(GPBStruct__storage_, fields),
        .flags = GPBFieldMapKeyString,
        .dataType = GPBDataTypeMessage,
      },
    };
    GPBDescriptor *localDescriptor =
        [GPBDescriptor allocDescriptorForClass:[GPBStruct class]
                                     rootClass:[GPBStructRoot class]
                                          file:GPBStructRoot_FileDescriptor()
                                        fields:fields
                                    fieldCount:(uint32_t)(sizeof(fields) / sizeof(GPBMessageFieldDescription))
                                   storageSize:sizeof(GPBStruct__storage_)
                                         flags:0];
    NSAssert(descriptor == nil, @"Startup recursed!");
    descriptor = localDescriptor;
  }
  return descriptor;
}

@end

#pragma mark - GPBValue

@implementation GPBValue

@dynamic kindOneOfCase;
@dynamic nullValue;
@dynamic numberValue;
@dynamic stringValue;
@dynamic boolValue;
@dynamic structValue;
@dynamic listValue;

typedef struct GPBValue__storage_ {
  uint32_t _has_storage_[2];
  GPBNullValue nullValue;
  NSString *stringValue;
  GPBStruct *structValue;
  GPBListValue *listValue;
  double numberValue;
} GPBValue__storage_;

// This method is threadsafe because it is initially called
// in +initialize for each subclass.
+ (GPBDescriptor *)descriptor {
  static GPBDescriptor *descriptor = nil;
  if (!descriptor) {
    static GPBMessageFieldDescription fields[] = {
      {
        .name = "nullValue",
        .dataTypeSpecific.enumDescFunc = GPBNullValue_EnumDescriptor,
        .number = GPBValue_FieldNumber_NullValue,
        .hasIndex = -1,
        .offset = (uint32_t)offsetof(GPBValue__storage_, nullValue),
        .flags = GPBFieldOptional | GPBFieldHasEnumDescriptor,
        .dataType = GPBDataTypeEnum,
      },
      {
        .name = "numberValue",
        .dataTypeSpecific.className = NULL,
        .number = GPBValue_FieldNumber_NumberValue,
        .hasIndex = -1,
        .offset = (uint32_t)offsetof(GPBValue__storage_, numberValue),
        .flags = GPBFieldOptional,
        .dataType = GPBDataTypeDouble,
      },
      {
        .name = "stringValue",
        .dataTypeSpecific.className = NULL,
        .number = GPBValue_FieldNumber_StringValue,
        .hasIndex = -1,
        .offset = (uint32_t)offsetof(GPBValue__storage_, stringValue),
        .flags = GPBFieldOptional,
        .dataType = GPBDataTypeString,
      },
      {
        .name = "boolValue",
        .dataTypeSpecific.className = NULL,
        .number = GPBValue_FieldNumber_BoolValue,
        .hasIndex = -1,
        .offset = 0,  // Stored in _has_storage_ to save space.
        .flags = GPBFieldOptional,
        .dataType = GPBDataTypeBool,
      },
      {
        .name = "structValue",
        .dataTypeSpecific.className = GPBStringifySymbol(GPBStruct),
        .number = GPBValue_FieldNumber_StructValue,
        .hasIndex = -1,
        .offset = (uint32_t)offsetof(GPBValue__storage_, structValue),
        .flags = GPBFieldOptional,
        .dataType = GPBDataTypeMessage,
      },
      {
        .name = "listValue",
        .dataTypeSpecific.className = GPBStringifySymbol(GPBListValue),
        .number = GPBValue_FieldNumber_ListValue,
        .hasIndex = -1,
        .offset = (uint32_t)offsetof(GPBValue__storage_, listValue),
        .flags = GPBFieldOptional,
        .dataType = GPBDataTypeMessage,
      },
    };
    GPBDescriptor *localDescriptor =
        [GPBDescriptor allocDescriptorForClass:[GPBValue class]
                                     rootClass:[GPBStructRoot class]
                                          file:GPBStructRoot_FileDescriptor()
                                        fields:fields
                                    fieldCount:(uint32_t)(sizeof(fields) / sizeof(GPBMessageFieldDescription))
                                   storageSize:sizeof(GPBValue__storage_)
                                         flags:0];
    static const char *oneofs[] = {
      "kind",
    };
    [localDescriptor setupOneofs:oneofs
                           count:(uint32_t)(sizeof(oneofs) / sizeof(char*))
                   firstHasIndex:-1];
    NSAssert(descriptor == nil, @"Startup recursed!");
    descriptor = localDescriptor;
  }
  return descriptor;
}

@end

int32_t GPBValue_NullValue_RawValue(GPBValue *message) {
  GPBDescriptor *descriptor = [GPBValue descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:GPBValue_FieldNumber_NullValue];
  return GPBGetMessageInt32Field(message, field);
}

void SetGPBValue_NullValue_RawValue(GPBValue *message, int32_t value) {
  GPBDescriptor *descriptor = [GPBValue descriptor];
  GPBFieldDescriptor *field = [descriptor fieldWithNumber:GPBValue_FieldNumber_NullValue];
  GPBSetInt32IvarWithFieldInternal(message, field, value, descriptor.file.syntax);
}

void GPBValue_ClearKindOneOfCase(GPBValue *message) {
  GPBDescriptor *descriptor = [message descriptor];
  GPBOneofDescriptor *oneof = descriptor->oneofs_[0];
  GPBMaybeClearOneof(message, oneof, -1, 0);
}
#pragma mark - GPBListValue

@implementation GPBListValue

@dynamic valuesArray, valuesArray_Count;

typedef struct GPBListValue__storage_ {
  uint32_t _has_storage_[1];
  NSMutableArray *valuesArray;
} GPBListValue__storage_;

// This method is threadsafe because it is initially called
// in +initialize for each subclass.
+ (GPBDescriptor *)descriptor {
  static GPBDescriptor *descriptor = nil;
  if (!descriptor) {
    static GPBMessageFieldDescription fields[] = {
      {
        .name = "valuesArray",
        .dataTypeSpecific.className = GPBStringifySymbol(GPBValue),
        .number = GPBListValue_FieldNumber_ValuesArray,
        .hasIndex = GPBNoHasBit,
        .offset = (uint32_t)offsetof(GPBListValue__storage_, valuesArray),
        .flags = GPBFieldRepeated,
        .dataType = GPBDataTypeMessage,
      },
    };
    GPBDescriptor *localDescriptor =
        [GPBDescriptor allocDescriptorForClass:[GPBListValue class]
                                     rootClass:[GPBStructRoot class]
                                          file:GPBStructRoot_FileDescriptor()
                                        fields:fields
                                    fieldCount:(uint32_t)(sizeof(fields) / sizeof(GPBMessageFieldDescription))
                                   storageSize:sizeof(GPBListValue__storage_)
                                         flags:0];
    NSAssert(descriptor == nil, @"Startup recursed!");
    descriptor = localDescriptor;
  }
  return descriptor;
}

@end


#pragma clang diagnostic pop

// @@protoc_insertion_point(global_scope)
