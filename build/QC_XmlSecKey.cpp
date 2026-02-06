/*
    QC_XmlSecKey.qpp

    Qore Programming Language

    Copyright 2003 - 2021 Qore Technologies, s.r.o.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "qore-xmlsec.h"

#include <qore/QoreSSLCertificate.h>

#include "QC_XmlSecKey.h"
#include "QC_XmlSec.h"
/* Qore class Qore::XmlSec::XmlSecKey */

qore_classid_t CID_XMLSECKEY;
QoreClass* QC_XMLSECKEY;

// XmlSecKey::constructor(data key, int format, *string password) {}
static void XmlSecKey_constructor_VDViNs(QoreObject* self, const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
    QoreValue key = get_param_value(args, 0);
    int64 format = HARD_QORE_VALUE_INT(args, 1);
    const QoreStringNode* password = get_param_value(args, 2).get<const QoreStringNode>();
# 50 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSecKey.qpp"
const char* ptr;
    size_t len;
    q_get_data(key, ptr, len);

    SimpleRefHolder<QoreXmlSecKey> newkey(new QoreXmlSecKey(xsink, (xmlSecByte*)ptr, len,
        (xmlSecKeyDataFormat)format, password ? password->c_str() : nullptr));
    if (*xsink)
        return;
    self->setPrivate(CID_XMLSECKEY, newkey.release());
}

// XmlSecKey::constructor(string str, int num_bits, int type) {}
static void XmlSecKey_constructor_VsViVi(QoreObject* self, const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
    const QoreStringNode* str = HARD_QORE_VALUE_STRING(args, 0);
    int64 num_bits = HARD_QORE_VALUE_INT(args, 1);
    int64 type = HARD_QORE_VALUE_INT(args, 2);
# 74 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSecKey.qpp"
SimpleRefHolder<QoreXmlSecKey> key(new QoreXmlSecKey((const xmlChar*)str->c_str(), num_bits, (xmlSecKeyDataType)type, xsink));
    if (*xsink)
        return;

    self->setPrivate(CID_XMLSECKEY, key.release());
}

// XmlSecKey::constructor(int id, int num_bits, int type) {}
static void XmlSecKey_constructor_ViViVi(QoreObject* self, const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
    int64 id = HARD_QORE_VALUE_INT(args, 0);
    int64 num_bits = HARD_QORE_VALUE_INT(args, 1);
    int64 type = HARD_QORE_VALUE_INT(args, 2);
# 94 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSecKey.qpp"
xmlSecKeyDataId keyid = xmlsec_get_keydata_id(id);
    if (!keyid) {
        xsink->raiseException("XMLSECKEY-KEYID-ERROR", "invalid key ID %d given", id);
        return;
    }

    SimpleRefHolder<QoreXmlSecKey> key(new QoreXmlSecKey(keyid, num_bits, (xmlSecKeyDataType)type, xsink));
    if (*xsink) {
        return;
    }

    self->setPrivate(CID_XMLSECKEY, key.release());
}

// XmlSecKey::copy() {}
static void XmlSecKey_copy(QoreObject* self, QoreObject* old, QoreXmlSecKey* key, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
# 115 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSecKey.qpp"
QoreXmlSecKey *nk = key->copy(xsink);
    if (!nk) {
        assert(*xsink);
        return;
    }

    self->setPrivate(CID_XMLSECKEY, nk);
}

// string XmlSecKey::getName(){}
static QoreValue XmlSecKey_getName(QoreObject* self, QoreXmlSecKey* key, const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
# 179 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSecKey.qpp"
    return key->getName(xsink);
}

// int XmlSecKey::getSize(){}
static QoreValue XmlSecKey_getSize(QoreObject* self, QoreXmlSecKey* key, const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
# 205 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSecKey.qpp"
    return key->getSize(xsink);
}

// int XmlSecKey::getType(){}
static QoreValue XmlSecKey_getType(QoreObject* self, QoreXmlSecKey* key, const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
# 191 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSecKey.qpp"
    return (int64)key->getType(xsink);
}

// nothing XmlSecKey::setCertificate(data cert, int format){}
static QoreValue XmlSecKey_setCertificate_VDVi(QoreObject* self, QoreXmlSecKey* key, const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
    QoreValue cert = get_param_value(args, 0);
    int64 format = HARD_QORE_VALUE_INT(args, 1);
# 134 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSecKey.qpp"
    const char* ptr;
    size_t len;
    q_get_data(cert, ptr, len);

    key->setCertificate((xmlSecByte*)ptr, len, (xmlSecKeyDataFormat)format, xsink);
    return QoreValue();
}

// nothing XmlSecKey::setCertificate(SSLCertificate cert){}
static QoreValue XmlSecKey_setCertificate_C14SSLCertificate(QoreObject* self, QoreXmlSecKey* key, const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
    HARD_QORE_VALUE_OBJ_DATA(cert, QoreSSLCertificate, args, 0, CID_SSLCERTIFICATE, "XmlSecKey::setCertificate()", "SSLCertificate", xsink);
    if (*xsink)
        return 0;
# 150 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSecKey.qpp"
    ReferenceHolder<QoreSSLCertificate> holder(cert, xsink);
    SimpleRefHolder<QoreStringNode> cert_pem(cert->getPEM(xsink));
    if (*xsink)
        return QoreValue();

    key->setCertificate((xmlSecByte*)cert_pem->c_str(), cert_pem->size(), xmlSecKeyDataFormatCertPem, xsink);
    return QoreValue();
}

// nothing XmlSecKey::setName(string name){}
static QoreValue XmlSecKey_setName_Vs(QoreObject* self, QoreXmlSecKey* key, const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
    const QoreStringNode* name = HARD_QORE_VALUE_STRING(args, 0);
# 167 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSecKey.qpp"
    key->setName(name->c_str(), xsink);
    return QoreValue();
}

// nothing XmlSecKey::verify(string signed_string, ...){}
static QoreValue XmlSecKey_verify_VsVV(QoreObject* self, QoreXmlSecKey* key, const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
    const QoreStringNode* signed_string = HARD_QORE_VALUE_STRING(args, 0);
# 222 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSecKey.qpp"
    q_xmlsec_verify(xsink, signed_string, key, 1, args);
    return QoreValue();
}

DLLLOCAL void preinitXmlSecKeyClass() {
    QC_XMLSECKEY = new QoreBuiltinClass("XmlSecKey", "::Qore::XmlSec::XmlSecKey", QDOM_DEFAULT);
    CID_XMLSECKEY = QC_XMLSECKEY->getID();
    QC_XMLSECKEY->setSystem();
}

DLLLOCAL QoreClass* initXmlSecKeyClass(QoreNamespace& ns) {
    if (!QC_XMLSECKEY)
        preinitXmlSecKeyClass();

    // XmlSecKey::constructor(data key, int format, *string password) {}
    QC_XMLSECKEY->addConstructor(XmlSecKey_constructor_VDViNs, Public, QCF_NO_FLAGS, QDOM_DEFAULT, 3, dataTypeInfo, QORE_PARAM_NO_ARG, "key", bigIntTypeInfo, QORE_PARAM_NO_ARG, "format", stringOrNothingTypeInfo, QORE_PARAM_NO_ARG, "password");

    // XmlSecKey::constructor(string str, int num_bits, int type) {}
    QC_XMLSECKEY->addConstructor(XmlSecKey_constructor_VsViVi, Public, QCF_NO_FLAGS, QDOM_DEFAULT, 3, stringTypeInfo, QORE_PARAM_NO_ARG, "str", bigIntTypeInfo, QORE_PARAM_NO_ARG, "num_bits", bigIntTypeInfo, QORE_PARAM_NO_ARG, "type");

    // XmlSecKey::constructor(int id, int num_bits, int type) {}
    QC_XMLSECKEY->addConstructor(XmlSecKey_constructor_ViViVi, Public, QCF_NO_FLAGS, QDOM_DEFAULT, 3, bigIntTypeInfo, QORE_PARAM_NO_ARG, "id", bigIntTypeInfo, QORE_PARAM_NO_ARG, "num_bits", bigIntTypeInfo, QORE_PARAM_NO_ARG, "type");

    // XmlSecKey::copy() {}
    QC_XMLSECKEY->setCopy((q_copy_t)XmlSecKey_copy);

    // string XmlSecKey::getName(){}
    QC_XMLSECKEY->addMethod("getName", (q_method_t)XmlSecKey_getName, Public, QCF_RET_VALUE_ONLY, QDOM_DEFAULT, stringTypeInfo);

    // int XmlSecKey::getSize(){}
    QC_XMLSECKEY->addMethod("getSize", (q_method_t)XmlSecKey_getSize, Public, QCF_RET_VALUE_ONLY, QDOM_DEFAULT, bigIntTypeInfo);

    // int XmlSecKey::getType(){}
    QC_XMLSECKEY->addMethod("getType", (q_method_t)XmlSecKey_getType, Public, QCF_RET_VALUE_ONLY, QDOM_DEFAULT, bigIntTypeInfo);

    // nothing XmlSecKey::setCertificate(data cert, int format){}
    QC_XMLSECKEY->addMethod("setCertificate", (q_method_t)XmlSecKey_setCertificate_VDVi, Public, QCF_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 2, dataTypeInfo, QORE_PARAM_NO_ARG, "cert", bigIntTypeInfo, QORE_PARAM_NO_ARG, "format");

    // nothing XmlSecKey::setCertificate(SSLCertificate cert){}
    QC_XMLSECKEY->addMethod("setCertificate", (q_method_t)XmlSecKey_setCertificate_C14SSLCertificate, Public, QCF_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, QC_SSLCERTIFICATE->getTypeInfo(), QORE_PARAM_NO_ARG, "cert");

    // nothing XmlSecKey::setName(string name){}
    QC_XMLSECKEY->addMethod("setName", (q_method_t)XmlSecKey_setName_Vs, Public, QCF_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, stringTypeInfo, QORE_PARAM_NO_ARG, "name");

    // nothing XmlSecKey::verify(string signed_string, ...){}
    QC_XMLSECKEY->addMethod("verify", (q_method_t)XmlSecKey_verify_VsVV, Public, QCF_USES_EXTRA_ARGS, QDOM_DEFAULT, nothingTypeInfo, 1, stringTypeInfo, QORE_PARAM_NO_ARG, "signed_string");

    return QC_XMLSECKEY;
}
