#ifndef CBOR_LIB_H
#define CBOR_LIB_H

#include "src/cbor.h"
#include "Arduino.h"

static void indent(int nestingLevel)
{
    while (nestingLevel--)
        Serial.print("  ");
}

static void dumpbytes(const uint8_t *buf, size_t len)
{
    while (len--)
        Serial.print(*buf++);
    Serial.println("");
}

static bool dumprecursive(CborValue *it, int nestingLevel)
{
    while (!cbor_value_at_end(it)) {
        CborError err;
        bool ret;
        CborType type = cbor_value_get_type(it);

        indent(nestingLevel);
        switch (type) {
        case CborArrayType:
        case CborMapType: {
            // recursive type
            CborValue recursed;
            assert(cbor_value_is_container(it));
            Serial.print((type == CborArrayType) ? "Array[" : "Map[");
            err = cbor_value_enter_container(it, &recursed);
            if (err)
                return err;       // parse error
            ret = dumprecursive(&recursed, nestingLevel + 1);
            if (err)
                return err;       // parse error
            err = cbor_value_leave_container(it, &recursed);
            if (err)
                return err;       // parse error
            indent(nestingLevel);
            Serial.println("]");
            continue;
        }

        case CborIntegerType: {
            int64_t val;
            cbor_value_get_int64(it, &val);     // can't fail
            //Serial.println(val);
            //printf("%lld\n", (long long)val);
            break;
        }

        case CborByteStringType: {
            uint8_t *buf;
            size_t n;
            err = cbor_value_dup_byte_string(it, &buf, &n, it);
            if (err)
                return err;     // parse error
            dumpbytes(buf, n);
            free(buf);
            continue;
        }

        case CborTextStringType: {
            char *buf;
            size_t n;
            err = cbor_value_dup_text_string(it, &buf, &n, it);
            if (err)
                return err;     // parse error
            Serial.println(buf);
            free(buf);
            continue;
        }
/*
        case CborTagType: {
            CborTag tag;
            cbor_value_get_tag(it, &tag);       // can't fail
            Serial.println(tag);
            break;
        }
        case CborSimpleType: {
            uint8_t type;
            cbor_value_get_simple_type(it, &type);  // can't fail
            Serial.print("Simple: "); Serial.println(type);
            break;
        }

        case CborNullType:
            Serial.println("null");
            break;

        case CborUndefinedType:
            Serial.println("undefined");
            break;
*/
        case CborBooleanType: {
            bool val;
            cbor_value_get_boolean(it, &val);       // can't fail
            Serial.println(val ? "true" : "false");
            break;
        }

        case CborDoubleType: {
            double val;
            if (false) {
                float f;
        case CborFloatType:
                Serial.print("Float val: ");
                cbor_value_get_float(it, &f);
                val = f;
            } else {
                Serial.print("Double val: ");
                cbor_value_get_double(it, &val);
            }
            Serial.println(val);
            break;
        }
        case CborHalfFloatType: {
            uint16_t val;
            Serial.print("Half Float val: ");
            cbor_value_get_half_float(it, &val);
            Serial.println(val);
            break;
        }

        case CborInvalidType:
            assert(false);      // can't happen
            break;
        }

        err = cbor_value_advance_fixed(it);
        if (err)
            return err;
    }
    return CborNoError;
}


#endif