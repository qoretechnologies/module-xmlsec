/*
    QC_XmlSecKeyManager.qpp

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

#include "QC_XmlSecKeyManager.h"
#include "QC_XmlSecKey.h"
#include "QC_XmlSec.h"
/* Qore class Qore::XmlSec::XmlSecKeyManager */

qore_classid_t CID_XMLSECKEYMANAGER;
QoreClass* QC_XMLSECKEYMANAGER;

// nothing XmlSecKeyManager::addKey(XmlSecKey key){}
static QoreValue XmlSecKeyManager_addKey_C9XmlSecKey(QoreObject* self, QoreXmlSecKeyManager* mgr, const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
    HARD_QORE_VALUE_OBJ_DATA(key, QoreXmlSecKey, args, 0, CID_XMLSECKEY, "XmlSecKeyManager::addKey()", "XmlSecKey", xsink);
    if (*xsink)
        return 0;
# 71 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSecKeyManager.qpp"
    SimpleRefHolder<QoreXmlSecKey> holder(key);

    xmlSecKeyPtr new_key = key->clone(xsink);
    if (!new_key) {
        assert(*xsink);
        return QoreValue();
    }

    if (mgr->adoptKey(new_key, xsink)) {
        assert(*xsink);
        return QoreValue();
    }
    return QoreValue();
}

// XmlSecKeyManager::constructor() {}
static void XmlSecKeyManager_constructor(QoreObject* self, const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
# 45 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSecKeyManager.qpp"
SimpleRefHolder<QoreXmlSecKeyManager> mgr(new QoreXmlSecKeyManager(xsink));
    if (*xsink) {
        return;
    }

    self->setPrivate(CID_XMLSECKEYMANAGER, mgr.release());
}

// XmlSecKeyManager::copy() {}
static void XmlSecKeyManager_copy(QoreObject* self, QoreObject* old, QoreXmlSecKeyManager* mgr, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
# 57 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSecKeyManager.qpp"
xsink->raiseException("XMLSECKEYMANAGER-COPY-ERROR", "The XmlSecKeyManager class cannot be copied");
}

// nothing XmlSecKeyManager::loadCertFromMemory(data cert_data, int format, int type){}
static QoreValue XmlSecKeyManager_loadCertFromMemory_VDViVi(QoreObject* self, QoreXmlSecKeyManager* mgr, const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
    QoreValue cert_data = get_param_value(args, 0);
    int64 format = HARD_QORE_VALUE_INT(args, 1);
    int64 type = HARD_QORE_VALUE_INT(args, 2);
# 118 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSecKeyManager.qpp"
    const char* bin_ptr;
    size_t bin_len;
    q_get_data(cert_data, bin_ptr, bin_len);
    mgr->loadCertFromMemory(xsink, (const xmlSecByte*)bin_ptr, (xmlSecSize)bin_len, (xmlSecKeyDataFormat)format,
        (xmlSecKeyDataType)type);
    return QoreValue();
}

// nothing XmlSecKeyManager::loadCertFromPath(string path, int format, int type){}
static QoreValue XmlSecKeyManager_loadCertFromPath_VsViVi(QoreObject* self, QoreXmlSecKeyManager* mgr, const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
    const QoreStringNode* path = HARD_QORE_VALUE_STRING(args, 0);
    int64 format = HARD_QORE_VALUE_INT(args, 1);
    int64 type = HARD_QORE_VALUE_INT(args, 2);
# 100 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSecKeyManager.qpp"
    mgr->loadCertFromPath(xsink, path->c_str(), (xmlSecKeyDataFormat)format, (xmlSecKeyDataType)type);
    return QoreValue();
}

// nothing XmlSecKeyManager::verify(string signed_string, ...){}
static QoreValue XmlSecKeyManager_verify_VsVV(QoreObject* self, QoreXmlSecKeyManager* mgr, const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
    const QoreStringNode* signed_string = HARD_QORE_VALUE_STRING(args, 0);
# 139 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSecKeyManager.qpp"
    q_xmlsec_verify(xsink, signed_string, mgr, 1, args);
    return QoreValue();
}

DLLLOCAL void preinitXmlSecKeyManagerClass() {
    QC_XMLSECKEYMANAGER = new QoreBuiltinClass("XmlSecKeyManager", "::Qore::XmlSec::XmlSecKeyManager", QDOM_DEFAULT);
    CID_XMLSECKEYMANAGER = QC_XMLSECKEYMANAGER->getID();
    QC_XMLSECKEYMANAGER->setSystem();
}

DLLLOCAL QoreClass* initXmlSecKeyManagerClass(QoreNamespace& ns) {
    if (!QC_XMLSECKEYMANAGER)
        preinitXmlSecKeyManagerClass();

    // nothing XmlSecKeyManager::addKey(XmlSecKey key){}
    QC_XMLSECKEYMANAGER->addMethod("addKey", (q_method_t)XmlSecKeyManager_addKey_C9XmlSecKey, Public, QCF_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 1, QC_XMLSECKEY->getTypeInfo(), QORE_PARAM_NO_ARG, "key");

    // XmlSecKeyManager::constructor() {}
    QC_XMLSECKEYMANAGER->addConstructor(XmlSecKeyManager_constructor, Public, QCF_NO_FLAGS, QDOM_DEFAULT);

    // XmlSecKeyManager::copy() {}
    QC_XMLSECKEYMANAGER->setCopy((q_copy_t)XmlSecKeyManager_copy);

    // nothing XmlSecKeyManager::loadCertFromMemory(data cert_data, int format, int type){}
    QC_XMLSECKEYMANAGER->addMethod("loadCertFromMemory", (q_method_t)XmlSecKeyManager_loadCertFromMemory_VDViVi, Public, QCF_NO_FLAGS, QDOM_DEFAULT, nothingTypeInfo, 3, dataTypeInfo, QORE_PARAM_NO_ARG, "cert_data", bigIntTypeInfo, QORE_PARAM_NO_ARG, "format", bigIntTypeInfo, QORE_PARAM_NO_ARG, "type");

    // nothing XmlSecKeyManager::loadCertFromPath(string path, int format, int type){}
    QC_XMLSECKEYMANAGER->addMethod("loadCertFromPath", (q_method_t)XmlSecKeyManager_loadCertFromPath_VsViVi, Public, QCF_NO_FLAGS, QDOM_FILESYSTEM, nothingTypeInfo, 3, stringTypeInfo, QORE_PARAM_NO_ARG, "path", bigIntTypeInfo, QORE_PARAM_NO_ARG, "format", bigIntTypeInfo, QORE_PARAM_NO_ARG, "type");

    // nothing XmlSecKeyManager::verify(string signed_string, ...){}
    QC_XMLSECKEYMANAGER->addMethod("verify", (q_method_t)XmlSecKeyManager_verify_VsVV, Public, QCF_USES_EXTRA_ARGS, QDOM_DEFAULT, nothingTypeInfo, 1, stringTypeInfo, QORE_PARAM_NO_ARG, "signed_string");

    return QC_XMLSECKEYMANAGER;
}
