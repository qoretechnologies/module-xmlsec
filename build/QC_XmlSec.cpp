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

#include "QC_XmlSec.h"
#include "QoreXmlDoc.h"
#include "QoreXmlSecEncCtx.h"
#include "DSigCtx.h"

static int xmlSecAppAddIDAttr(xmlNodePtr node, const xmlChar* attrName, const xmlChar* nodeName, const xmlChar* nsHref) {
    xmlAttrPtr attr, tmpAttr;
    xmlNodePtr cur;
    xmlChar* id;

    if((node == NULL) || (attrName == NULL) || (nodeName == NULL)) {
        return(-1);
    }

    /* process children first because it does not matter much but does simplify code */
    cur = xmlSecGetNextElementNode(node->children);
    while(cur != NULL) {
        if(xmlSecAppAddIDAttr(cur, attrName, nodeName, nsHref) < 0) {
            return(-1);
        }
        cur = xmlSecGetNextElementNode(cur->next);
    }

    /* node name must match */
    if(!xmlStrEqual(node->name, nodeName)) {
        return(0);
    }

    /* if nsHref is set then it also should match */
    if((nsHref != NULL) && (node->ns != NULL) && (!xmlStrEqual(nsHref, node->ns->href))) {
        return(0);
    }

    /* the attribute with name equal to attrName should exist */
    for(attr = node->properties; attr != NULL; attr = attr->next) {
        if(xmlStrEqual(attr->name, attrName)) {
            break;
        }
    }
    if(attr == NULL) {
        return(0);
    }

    /* and this attr should have a value */
    id = xmlNodeListGetString(node->doc, attr->children, 1);
    if(id == NULL) {
        return(0);
    }

    /* check that we don't have same ID already */
    tmpAttr = xmlGetID(node->doc, id);
    if(tmpAttr == NULL) {
        xmlAddID(NULL, node->doc, id, attr);
    } else if(tmpAttr != attr) {
        fprintf(stderr, "Error: duplicate ID attribute \"%s\"\n", id);
        xmlFree(id);
        return(-1);
    }
    xmlFree(id);
    return(0);
}

static xmlNodePtr q_xmlsec_find_node(ExceptionSink* xsink, QoreXmlDoc& doc, unsigned offset, const QoreListNode* args) {
    xmlNodePtr node = nullptr;
    if (args && args->size() > offset) {
        ConstListIterator li(args, offset);
        while (li.next()) {
            QoreStringValueHelper str(li.getValue(), QCS_UTF8, xsink);
            if (*xsink) {
                return nullptr;
            }
            node = xmlSecGetNextElementNode(doc.getChildren());
            if (!node) {
                xsink->raiseException("XMLSEC-VERIFY-ERROR", "XML document has no child nodes");
                return nullptr;
            }

            qore_offset_t e = str->brindex("=");
            if (!e || e == -1) {
                xsink->raiseException("XMLSEC-VERIFY-ERROR", "path argument must have the format "
                    "<id>=<[ns:]name>; got \"%s\" instead", str->c_str());
                return nullptr;
            }
            assert(e > 0);
            QoreString attr(*str, (size_t)e);

            const char* name = str->c_str() + e + 1;

            // find namespace ref
            const char* p = strrchr(name, ':');
            QoreString buf;
            const char* node_name, *ns_href;
            if (!p || p == name || p == (str->c_str() + str->size() - e - 1)) {
                node_name = name;
                ns_href = nullptr;
            } else {
                buf.set(str->c_str(), p - name);
                node_name = buf.c_str();
                ns_href = p + 1;
            }

            while (node) {
                if (xmlSecAppAddIDAttr(node, (const xmlChar*)attr.c_str(), (const xmlChar*)node_name,
                    (const xmlChar*)ns_href) < 0) {
                    xsink->raiseException("XMLSEC-VERIFY-ERROR", "could not find node '%s' with attribute '%s'",
                        str->c_str() + e + 1, attr.c_str());
                    return nullptr;
                }
                node = xmlSecGetNextElementNode(node->next);
            }
        }
    } else {
        node = xmlSecFindNode(doc.getRootElement(), xmlSecNodeSignature, xmlSecDSigNs);
        if (!node) {
            xsink->raiseException("XMLSEC-VERIFY-ERROR", "start node not found in string");
            return nullptr;
        }
    }

    return node;
}

int q_xmlsec_verify(ExceptionSink* xsink, const QoreStringNode* signed_string, QoreXmlSecKeyManager* mgr,
        unsigned offset, const QoreListNode* args) {
    TempEncodingHelper str_utf8(signed_string, QCS_UTF8, xsink);
    if (!str_utf8) {
        return -1;
    }

    QoreXmlDoc doc(str_utf8->c_str());
    if (!doc || !doc.getRootElement()) {
        xsink->raiseException("XMLSEC-VERIFY-ERROR", "unable to parse signed XML string");
        return -1;
    }

    // find start node
    xmlNodePtr node = q_xmlsec_find_node(xsink, doc, offset, args);
    if (!node) {
        return -1;
    }

    DSigCtx dsigCtx(mgr->getKeyManager());
    if (!dsigCtx) {
        xsink->raiseException("XMLSEC-VERIFY-ERROR", "failed to create signature context from key manager");
        return -1;
    }

    if (dsigCtx.verify(node, xsink)) {
        return -1;
    }

    //printd(5, "stat=%d success=%d (signMethod->status=%d/%d, fail=%d, ok=%d)\n", dsigCtx.getStatus(), xmlSecDSigStatusSucceeded, dsigCtx.dsigCtx->signMethod->status, dsigCtx.getTransformStatus(), xmlSecTransformStatusFail, xmlSecTransformStatusOk);

    // check if signatures do not match
    if (dsigCtx.getTransformStatus() == xmlSecTransformStatusFail) {
        xsink->raiseException("XMLSEC-VERIFY-ERROR", "signature verification failed; signatures do not match");
        return -1;
    } else if (dsigCtx.getStatus() != xmlSecDSigStatusSucceeded) {
        xsink->raiseException("XMLSEC-VERIFY-ERROR", "signature verification failed; crypto error");
        return -1;
    }

    return 0;
}

int q_xmlsec_verify(ExceptionSink* xsink, const QoreStringNode* signed_string, QoreXmlSecKey* key, unsigned offset,
        const QoreListNode* args) {
    TempEncodingHelper str_utf8(signed_string, QCS_UTF8, xsink);
    if (!str_utf8) {
        return -1;
    }

    QoreXmlDoc doc(str_utf8->getBuffer());
    if (!doc || !doc.getRootElement()) {
        xsink->raiseException("XMLSEC-VERIFY-ERROR", "unable to parse signed XML string");
        return -1;
    }

    // find start node
    xmlNodePtr node = q_xmlsec_find_node(xsink, doc, offset, args);
    if (!node) {
        return -1;
    }

    DSigCtx dsigCtx;
    if (!dsigCtx) {
        xsink->raiseException("XMLSEC-VERIFY-ERROR", "failed to create signature context");
        return -1;
    }

    xmlSecKeyPtr new_key = key->clone(xsink);
    if (!new_key) {
        return -1;
    }

    // set key data
    dsigCtx.setKey(new_key);

    if (dsigCtx.verify(node, xsink)) {
        return -1;
    }

    //printd(5, "stat=%d success=%d (signMethod->status=%d/%d, fail=%d, ok=%d)\n", dsigCtx.getStatus(), xmlSecDSigStatusSucceeded, dsigCtx.dsigCtx->signMethod->status, dsigCtx.getTransformStatus(), xmlSecTransformStatusFail, xmlSecTransformStatusOk);

    // check if signatures do not match
    if (dsigCtx.getTransformStatus() == xmlSecTransformStatusFail) {
        xsink->raiseException("XMLSEC-VERIFY-ERROR", "signature verification failed; signatures do not match");
        return -1;
    } else if (dsigCtx.getStatus() != xmlSecDSigStatusSucceeded) {
        xsink->raiseException("XMLSEC-VERIFY-ERROR", "signature verification failed; crypto error");
        return -1;
    }
    return 0;
}
/* Qore class Qore::XmlSec::XmlSec */

qore_classid_t CID_XMLSEC;
QoreClass* QC_XMLSEC;

// XmlSec::constructor() {}
static void XmlSec_constructor(QoreObject* self, const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
# 331 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSec.qpp"
xsink->raiseException("XMLSEC-CONSTRUCTOR-ERROR", "This class contains only static methods and cannot be instantiated");
}

// static data XmlSec::decrypt(string xml, XmlSecKey key){}
static QoreValue static_XmlSec_decrypt_VsC9XmlSecKey(const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
    const QoreStringNode* xml = HARD_QORE_VALUE_STRING(args, 0);
    HARD_QORE_VALUE_OBJ_DATA(key, QoreXmlSecKey, args, 1, CID_XMLSECKEY, "XmlSec::decrypt()", "XmlSecKey", xsink);
    if (*xsink)
        return 0;
# 485 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSec.qpp"
SimpleRefHolder<QoreXmlSecKey> holder(key);

    TempEncodingHelper xml_utf8(xml, QCS_UTF8, xsink);
    if (!xml_utf8) {
        return QoreValue();
    }

    QoreXmlDoc doc(xml_utf8->getBuffer());
    if (!doc || !doc.getRootElement()) {
        xsink->raiseException("XMLSEC-DECRYPT-ERROR", "unable to parse XML string");
        return QoreValue();
    }

    // find start node
    xmlNodePtr node = xmlSecFindNode(doc.getRootElement(), xmlSecNodeEncryptedData, xmlSecEncNs);
    if (!node) {
        xsink->raiseException("XMLSEC-DECRYPT-ERROR", "start node not found in template");
        return QoreValue();
    }

    QoreXmlSecEncCtx encCtx(xsink, nullptr);
    if (!encCtx) {
        xsink->raiseException("XMLSEC-DECRYPT-ERROR", "failed to create decryption context");
        return QoreValue();
    }

    xmlSecKeyPtr new_key = key->clone(xsink);
    if (!new_key) {
        return QoreValue();
    }

    encCtx.setKey(new_key);

    BinaryNode* b;
    if (encCtx.decrypt(node, b, xsink)) {
        return QoreValue();
    }

    return b ? (AbstractQoreNode*)b : (AbstractQoreNode*)doc.getString();
}

// static data XmlSec::decrypt(string xml, XmlSecKeyManager key_manager){}
static QoreValue static_XmlSec_decrypt_VsC16XmlSecKeyManager(const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
    const QoreStringNode* xml = HARD_QORE_VALUE_STRING(args, 0);
    HARD_QORE_VALUE_OBJ_DATA(key_manager, QoreXmlSecKeyManager, args, 1, CID_XMLSECKEYMANAGER, "XmlSec::decrypt()", "XmlSecKeyManager", xsink);
    if (*xsink)
        return 0;
# 540 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSec.qpp"
SimpleRefHolder<QoreXmlSecKeyManager> mgr_holder(key_manager);

    TempEncodingHelper xml_utf8(xml, QCS_UTF8, xsink);
    if (!xml_utf8) {
        return QoreValue();
    }

    QoreXmlDoc doc(xml_utf8->getBuffer());
    if (!doc || !doc.getRootElement()) {
        xsink->raiseException("XMLSEC-DECRYPT-ERROR", "unable to parse XML string");
        return QoreValue();
    }

    // find start node
    xmlNodePtr node = xmlSecFindNode(doc.getRootElement(), xmlSecNodeEncryptedData, xmlSecEncNs);
    if (!node) {
        xsink->raiseException("XMLSEC-DECRYPT-ERROR", "start node not found in template");
        return QoreValue();
    }

    //printd(5, "mgr=%08p\n", mgr ? mgr->getKeyManager() : 0);
    QoreXmlSecEncCtx encCtx(xsink, key_manager->getKeyManager());
    if (!encCtx) {
        xsink->raiseException("XMLSEC-DECRYPT-ERROR", "failed to create decryption context");
        return QoreValue();
    }

    BinaryNode *b;
    if (encCtx.decrypt(node, b, xsink)) {
        return QoreValue();
    }

    return b ? (AbstractQoreNode*)b : (AbstractQoreNode*)doc.getString();
}

// static string XmlSec::encrypt(string str_data, string tmpl, XmlSecKey key, *XmlSecKeyManager key_manager){}
static QoreValue static_XmlSec_encrypt_VsVsC9XmlSecKeyC17_XmlSecKeyManager(const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
    const QoreStringNode* str_data = HARD_QORE_VALUE_STRING(args, 0);
    const QoreStringNode* tmpl = HARD_QORE_VALUE_STRING(args, 1);
    HARD_QORE_VALUE_OBJ_DATA(key, QoreXmlSecKey, args, 2, CID_XMLSECKEY, "XmlSec::encrypt()", "XmlSecKey", xsink);
    if (*xsink)
        return 0;
    HARD_QORE_VALUE_OBJ_OR_NOTHING_DATA(key_manager, QoreXmlSecKeyManager, args, 3, CID_XMLSECKEYMANAGER, xsink);
    if (*xsink)
        return 0;
# 354 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSec.qpp"
SimpleRefHolder<QoreXmlSecKey> holder(key);
    SimpleRefHolder<QoreXmlSecKeyManager> mgr_holder(key_manager);

    TempEncodingHelper template_utf8(tmpl, QCS_UTF8, xsink);
    if (!template_utf8) {
        return QoreValue();
    }

    QoreXmlDoc doc(template_utf8->getBuffer());
    if (!doc || !doc.getRootElement()) {
        xsink->raiseException("XMLSEC-ENCRYPT-ERROR", "unable to parse XML template string");
        return QoreValue();
    }

    // find start node
    xmlNodePtr node = xmlSecFindNode(doc.getRootElement(), xmlSecNodeEncryptedData, xmlSecEncNs);
    if (!node) {
        xsink->raiseException("XMLSEC-ENCRYPT-ERROR", "start node not found in template");
        return QoreValue();
    }

    //printd(5, "mgr=%08p\n", mgr ? mgr->getKeyManager() : 0);
    QoreXmlSecEncCtx encCtx(xsink, key_manager ? key_manager->getKeyManager() : nullptr);
    if (!encCtx) {
        xsink->raiseException("XMLSEC-ENCRYPT-ERROR", "failed to create encryption context");
        return QoreValue();
    }

    xmlSecKeyPtr new_key = key->clone(xsink);
    if (!new_key) {
        return QoreValue();
    }

    encCtx.setKey(new_key);

    // do XML encryption
    TempEncodingHelper edoc_utf8(str_data, QCS_UTF8, xsink);
    if (!edoc_utf8) {
        return QoreValue();
    }

    QoreXmlDoc edoc(edoc_utf8->getBuffer());
    if (!edoc || !edoc.getRootElement()) {
        xsink->raiseException("XMLSEC-ENCRYPT-ERROR", "failed to parse XML data to encrypt passed as first argument to XmlSec::encrypt()");
        return QoreValue();
    }

    if (encCtx.encryptNode(node, edoc.getRootElement())) {
        xsink->raiseException("XMLSEC-ENCRYPT-ERROR", "encryption failed");
        return QoreValue();
    }

    return edoc.getString();
}

// static string XmlSec::encrypt(binary bin_data, string tmpl, XmlSecKey key, *XmlSecKeyManager key_manager){}
static QoreValue static_XmlSec_encrypt_VyVsC9XmlSecKeyC17_XmlSecKeyManager(const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
    const BinaryNode* bin_data = HARD_QORE_VALUE_BINARY(args, 0);
    const QoreStringNode* tmpl = HARD_QORE_VALUE_STRING(args, 1);
    HARD_QORE_VALUE_OBJ_DATA(key, QoreXmlSecKey, args, 2, CID_XMLSECKEY, "XmlSec::encrypt()", "XmlSecKey", xsink);
    if (*xsink)
        return 0;
    HARD_QORE_VALUE_OBJ_OR_NOTHING_DATA(key_manager, QoreXmlSecKeyManager, args, 3, CID_XMLSECKEYMANAGER, xsink);
    if (*xsink)
        return 0;
# 429 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSec.qpp"
SimpleRefHolder<QoreXmlSecKey> holder(key);
    SimpleRefHolder<QoreXmlSecKeyManager> mgr_holder(key_manager);

    TempEncodingHelper template_utf8(tmpl, QCS_UTF8, xsink);
    if (!template_utf8) {
        return QoreValue();
    }

    QoreXmlDoc doc(template_utf8->getBuffer());
    if (!doc || !doc.getRootElement()) {
        xsink->raiseException("XMLSEC-ENCRYPT-ERROR", "unable to parse XML template string");
        return QoreValue();
    }

    // find start node
    xmlNodePtr node = xmlSecFindNode(doc.getRootElement(), xmlSecNodeEncryptedData, xmlSecEncNs);
    if (!node) {
        xsink->raiseException("XMLSEC-ENCRYPT-ERROR", "start node not found in template");
        return QoreValue();
    }

    //printd(5, "mgr=%08p\n", mgr ? mgr->getKeyManager() : 0);
    QoreXmlSecEncCtx encCtx(xsink, key_manager ? key_manager->getKeyManager() : nullptr);
    if (!encCtx) {
        xsink->raiseException("XMLSEC-ENCRYPT-ERROR", "failed to create encryption context");
        return QoreValue();
    }

    xmlSecKeyPtr new_key = key->clone(xsink);
    if (!new_key) {
        return QoreValue();
    }

    encCtx.setKey(new_key);

    if (encCtx.encryptBinary(node, bin_data)) {
        xsink->raiseException("XMLSEC-ENCRYPT-ERROR", "encryption failed");
        return QoreValue();
    }
    return doc.getString();
}

// static string XmlSec::sign(string tmpl, XmlSecKey key){}
static QoreValue static_XmlSec_sign_VsC9XmlSecKey(const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
    const QoreStringNode* tmpl = HARD_QORE_VALUE_STRING(args, 0);
    HARD_QORE_VALUE_OBJ_DATA(key, QoreXmlSecKey, args, 1, CID_XMLSECKEY, "XmlSec::sign()", "XmlSecKey", xsink);
    if (*xsink)
        return 0;
# 592 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSec.qpp"
SimpleRefHolder<QoreXmlSecKey> holder(key);

    TempEncodingHelper template_utf8(tmpl, QCS_UTF8, xsink);
    if (!template_utf8) {
        return QoreValue();
    }

    QoreXmlDoc doc(template_utf8->getBuffer());
    if (!doc || !doc.getRootElement()) {
        xsink->raiseException("XMLSEC-SIGN-ERROR", "unable to parse XML template string");
        return QoreValue();
    }

    // find start node
    xmlNodePtr node = xmlSecFindNode(doc.getRootElement(), xmlSecNodeSignature, xmlSecDSigNs);
    if (!node) {
        xsink->raiseException("XMLSEC-SIGN-ERROR", "start node not found in template");
        return QoreValue();
    }

    DSigCtx dsigCtx;
    if (!dsigCtx) {
        xsink->raiseException("XMLSEC-SIGN-ERROR", "failed to create signature context");
        return QoreValue();
    }

    xmlSecKeyPtr new_key = key->clone(xsink);
    if (!new_key) {
        return QoreValue();
    }

    // set key data
    dsigCtx.setKey(new_key);

    if (dsigCtx.sign(node, xsink)) {
        assert(*xsink);
        return QoreValue();
    }

    return doc.getString();
}

// static nothing XmlSec::verify(string signed_string, XmlSecKey key, ...){}
static QoreValue static_XmlSec_verify_VsC9XmlSecKeyVV(const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
    const QoreStringNode* signed_string = HARD_QORE_VALUE_STRING(args, 0);
    HARD_QORE_VALUE_OBJ_DATA(key, QoreXmlSecKey, args, 1, CID_XMLSECKEY, "XmlSec::verify()", "XmlSecKey", xsink);
    if (*xsink)
        return 0;
# 649 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSec.qpp"
SimpleRefHolder<QoreXmlSecKey> holder(key);

    q_xmlsec_verify(xsink, signed_string, key, 2, args);
    return QoreValue();
}

// static nothing XmlSec::verify(string signed_string, XmlSecKeyManager mgr, ...){}
static QoreValue static_XmlSec_verify_VsC16XmlSecKeyManagerVV(const QoreListNode* args, RuntimeConfig& runtime_cfg, ExceptionSink* xsink) {
    const QoreStringNode* signed_string = HARD_QORE_VALUE_STRING(args, 0);
    HARD_QORE_VALUE_OBJ_DATA(mgr, QoreXmlSecKeyManager, args, 1, CID_XMLSECKEYMANAGER, "XmlSec::verify()", "XmlSecKeyManager", xsink);
    if (*xsink)
        return 0;
# 669 "/home/david/src/qore/git/module-xmlsec/src/QC_XmlSec.qpp"
SimpleRefHolder<QoreXmlSecKeyManager> holder(mgr);

    q_xmlsec_verify(xsink, signed_string, mgr, 2, args);
    return QoreValue();
}

DLLLOCAL void preinitXmlSecClass() {
    QC_XMLSEC = new QoreBuiltinClass("XmlSec", "::Qore::XmlSec::XmlSec", QDOM_DEFAULT);
    CID_XMLSEC = QC_XMLSEC->getID();
    QC_XMLSEC->setSystem();
}

DLLLOCAL QoreClass* initXmlSecClass(QoreNamespace& ns) {
    if (!QC_XMLSEC)
        preinitXmlSecClass();

    // XmlSec::constructor() {}
    QC_XMLSEC->addConstructor(XmlSec_constructor, Public, QCF_NO_FLAGS, QDOM_DEFAULT);

    // static data XmlSec::decrypt(string xml, XmlSecKey key){}
    QC_XMLSEC->addStaticMethod("decrypt", (q_func_t)static_XmlSec_decrypt_VsC9XmlSecKey, Public, QCF_RET_VALUE_ONLY, QDOM_DEFAULT, dataTypeInfo, 2, stringTypeInfo, QORE_PARAM_NO_ARG, "xml", QC_XMLSECKEY->getTypeInfo(), QORE_PARAM_NO_ARG, "key");

    // static data XmlSec::decrypt(string xml, XmlSecKeyManager key_manager){}
    QC_XMLSEC->addStaticMethod("decrypt", (q_func_t)static_XmlSec_decrypt_VsC16XmlSecKeyManager, Public, QCF_RET_VALUE_ONLY, QDOM_DEFAULT, dataTypeInfo, 2, stringTypeInfo, QORE_PARAM_NO_ARG, "xml", QC_XMLSECKEYMANAGER->getTypeInfo(), QORE_PARAM_NO_ARG, "key_manager");

    // static string XmlSec::encrypt(string str_data, string tmpl, XmlSecKey key, *XmlSecKeyManager key_manager){}
    QC_XMLSEC->addStaticMethod("encrypt", (q_func_t)static_XmlSec_encrypt_VsVsC9XmlSecKeyC17_XmlSecKeyManager, Public, QCF_NO_FLAGS, QDOM_DEFAULT, stringTypeInfo, 4, stringTypeInfo, QORE_PARAM_NO_ARG, "str_data", stringTypeInfo, QORE_PARAM_NO_ARG, "tmpl", QC_XMLSECKEY->getTypeInfo(), QORE_PARAM_NO_ARG, "key", QC_XMLSECKEYMANAGER->getOrNothingTypeInfo(), QORE_PARAM_NO_ARG, "key_manager");

    // static string XmlSec::encrypt(binary bin_data, string tmpl, XmlSecKey key, *XmlSecKeyManager key_manager){}
    QC_XMLSEC->addStaticMethod("encrypt", (q_func_t)static_XmlSec_encrypt_VyVsC9XmlSecKeyC17_XmlSecKeyManager, Public, QCF_NO_FLAGS, QDOM_DEFAULT, stringTypeInfo, 4, binaryTypeInfo, QORE_PARAM_NO_ARG, "bin_data", stringTypeInfo, QORE_PARAM_NO_ARG, "tmpl", QC_XMLSECKEY->getTypeInfo(), QORE_PARAM_NO_ARG, "key", QC_XMLSECKEYMANAGER->getOrNothingTypeInfo(), QORE_PARAM_NO_ARG, "key_manager");

    // static string XmlSec::sign(string tmpl, XmlSecKey key){}
    QC_XMLSEC->addStaticMethod("sign", (q_func_t)static_XmlSec_sign_VsC9XmlSecKey, Public, QCF_RET_VALUE_ONLY, QDOM_DEFAULT, stringTypeInfo, 2, stringTypeInfo, QORE_PARAM_NO_ARG, "tmpl", QC_XMLSECKEY->getTypeInfo(), QORE_PARAM_NO_ARG, "key");

    // static nothing XmlSec::verify(string signed_string, XmlSecKey key, ...){}
    QC_XMLSEC->addStaticMethod("verify", (q_func_t)static_XmlSec_verify_VsC9XmlSecKeyVV, Public, QCF_USES_EXTRA_ARGS, QDOM_DEFAULT, nothingTypeInfo, 2, stringTypeInfo, QORE_PARAM_NO_ARG, "signed_string", QC_XMLSECKEY->getTypeInfo(), QORE_PARAM_NO_ARG, "key");

    // static nothing XmlSec::verify(string signed_string, XmlSecKeyManager mgr, ...){}
    QC_XMLSEC->addStaticMethod("verify", (q_func_t)static_XmlSec_verify_VsC16XmlSecKeyManagerVV, Public, QCF_USES_EXTRA_ARGS, QDOM_DEFAULT, nothingTypeInfo, 2, stringTypeInfo, QORE_PARAM_NO_ARG, "signed_string", QC_XMLSECKEYMANAGER->getTypeInfo(), QORE_PARAM_NO_ARG, "mgr");
    ns.addConstant("ModuleVersion", (new QoreStringNode(PACKAGE_VERSION)));
    ns.addConstant("xmlSecKeyDataAesId", ((int64)XMLSEC_KEYDATA_AESID));
    ns.addConstant("xmlSecKeyDataDesId", ((int64)XMLSEC_KEYDATA_DESID));
    ns.addConstant("xmlSecKeyDataDsaId", ((int64)XMLSEC_KEYDATA_DSAID));
    ns.addConstant("xmlSecKeyDataHmacId", ((int64)XMLSEC_KEYDATA_HMACID));
    ns.addConstant("xmlSecKeyDataRawX509CertId", ((int64)XMLSEC_KEYDATA_RAWX509CERTID));
    ns.addConstant("xmlSecKeyDataRsaId", ((int64)XMLSEC_KEYDATA_RSAID));
    ns.addConstant("xmlSecKeyDataX509Id", ((int64)XMLSEC_KEYDATA_X509ID));
    ns.addConstant("xmlSecKeyDataFormatBinary", (xmlSecKeyDataFormatBinary));
    ns.addConstant("xmlSecKeyDataFormatCertDer", (xmlSecKeyDataFormatCertDer));
    ns.addConstant("xmlSecKeyDataFormatCertPem", (xmlSecKeyDataFormatCertPem));
    ns.addConstant("xmlSecKeyDataFormatDer", (xmlSecKeyDataFormatDer));
    ns.addConstant("xmlSecKeyDataFormatPem", (xmlSecKeyDataFormatPem));
    ns.addConstant("xmlSecKeyDataFormatPkcs12", (xmlSecKeyDataFormatPkcs12));
    ns.addConstant("xmlSecKeyDataFormatPkcs8Der", (xmlSecKeyDataFormatPkcs8Der));
    ns.addConstant("xmlSecKeyDataFormatPkcs8Pem", (xmlSecKeyDataFormatPkcs8Pem));
    ns.addConstant("xmlSecKeyDataFormatUnknown", (xmlSecKeyDataFormatUnknown));
    ns.addConstant("xmlSecKeyDataTypeAny", (xmlSecKeyDataTypeAny));
    ns.addConstant("xmlSecKeyDataTypeNone", (xmlSecKeyDataTypeNone));
    ns.addConstant("xmlSecKeyDataTypePermanent", (xmlSecKeyDataTypePermanent));
    ns.addConstant("xmlSecKeyDataTypePrivate", (xmlSecKeyDataTypePrivate));
    ns.addConstant("xmlSecKeyDataTypePublic", (xmlSecKeyDataTypePublic));
    ns.addConstant("xmlSecKeyDataTypeSession", (xmlSecKeyDataTypeSession));
    ns.addConstant("xmlSecKeyDataTypeSymmetric", (xmlSecKeyDataTypeSymmetric));
    ns.addConstant("xmlSecKeyDataTypeTrusted", (xmlSecKeyDataTypeTrusted));
    ns.addConstant("xmlSecKeyDataTypeUnknown", (xmlSecKeyDataTypeUnknown));

    return QC_XMLSEC;
}
