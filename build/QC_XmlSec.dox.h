//! Qore namespace
namespace Qore {
//! XmlSec namespace
namespace XmlSec {
//! The \c XmlSec class implements static methods for working with XmlSec keys
/***/
class XmlSec {

public:
//! throws an exception as this class is not meant to be instantiated
/** @throw XMLSEC-CONSTRUCTOR-ERROR this class is not meant to be instantiated
*/
 constructor();

public:
//! Decrypts the encrypted XML data in the XML string using the given key
/** @par Code Flags:
    @ref RET_VALUE_ONLY

    @par Example:
    @code{.py}
data d = XmlSec::decrypt(xml, key);
    @endcode

    @param xml the XML to decrypt
    @param key the decryption key

    @return an XML string with decrypted data or a inary

    @throw XMLSEC-DECRYPT-ERROR decryption failed, libxmlsec error
*/
static data decrypt(string xml, XmlSecKey key);

public:
//! Decryps the encrypted XML data that was encrypted with a session key using the @ref Qore::XmlSec::XmlSecKeyManager "XmlSecKeyManager" object to decrypt the session key and then decrypt the message using the decrypted session key
/** @par Code Flags:
    @ref RET_VALUE_ONLY

    @par Example:
    @code{.py}
data d = XmlSec::decrypt(xml, key_manager);
    @endcode

    @param xml the XML to decrypt
    @param key_manager the decryption key

    @return an XML string with decrypted data or a binary

    @throw XMLSEC-DECRYPT-ERROR decryption failed, libxmlsec error
*/
static data decrypt(string xml, XmlSecKeyManager key_manager);

public:
//! Encrypts data using an XML template and an @ref Qore::XmlSec::XmlSecKey "XmlSecKey" object and optionally an @ref Qore::XmlSec::XmlSecKeyManager "XmlSecKeyManager" object
/** @par Example:
    @code{.py}
string xml = XmlSec::encrypt(str, encryption_template, key);
    @endcode

    @param str_data the string data to encrypt
    @param tmpl the XML template for encrypting the data
    @param key the key to use to encrypt the data
    @param manager the optional key manager to use for encryption

    @return the XML string with the encrypted data

    Make sure the key type corresponds to the \c Algorithm attribute of the \c EncryptionMethod tag
    in the XML template or the method call will fail.  If any errors occur an appropriate exception
    is thrown.

    @throw XMLSEC-ENCRYPT-ERROR error in arguments to the methods; encryption failed, libxmlsec error
*/
static string encrypt(string str_data, string tmpl, XmlSecKey key, __7_ XmlSecKeyManager key_manager);

public:
//! Encrypts data using an XML template and an @ref Qore::XmlSec::XmlSecKey "XmlSecKey" object and optionally an @ref Qore::XmlSec::XmlSecKeyManager "XmlSecKeyManager" object
/** @par Example:
    @code{.py}
string xml = XmlSec::encrypt(str, encryption_template, key);
    @endcode

    @param bin_data the data to encrypt
    @param tmpl the XML template for encrypting the data
    @param key the key to use to encrypt the data
    @param manager the optional key manager to use for encryption

    @return the XML string with the encrypted data

    Make sure the key type corresponds to the \c Algorithm attribute of the \c EncryptionMethod tag
    in the XML template or the method call will fail.  If any errors occur an appropriate exception
    is thrown.

    @throw XMLSEC-ENCRYPT-ERROR error in arguments to the methods; encryption failed, libxmlsec error
*/
static string encrypt(binary bin_data, string tmpl, XmlSecKey key, __7_ XmlSecKeyManager key_manager);

public:
//! Creates a signed XML string based on an XML template string and an @ref Qore::XmlSec::XmlSecKey "XmlSecKey" object
/** @par Code Flags:
    @ref RET_VALUE_ONLY

    @par Example:
    @code{.py}
string xml = XmlSec::sign(template_string, key);
    @endcode

    @param tmpl the XML template
    @param key the key to use to sign the string

    @return the signed XML string

    If errors occur an appropriate exception is raised. Transformations are given in the XML template.

    @throw XMLSEC-SIGN-ERROR error in arguments to the methods; libxmlsec error
    @throw XMLSEC-DSIGCTX-ERROR error producing the signed XML string
*/
static string sign(string tmpl, XmlSecKey key);

public:
//! Verifies the signature of the signed XML string passed as the first argument with the given key
/** @par Example:
    @code{.py}
XmlSec::verify(signed_string, key);
    @endcode

    @param signed_string the signed XML string to verify
    @param key the key to use to verify the signed string

    If any errors occur, an exception is thrown

    @throw XMLSEC-SIGN-ERROR: error in arguments to the methods; signature verification failed
    @throw XMLSEC-DSIGCTX-ERROR: signature verification could not be processed by libxmlse
*/
static nothing verify(string signed_string, XmlSecKey key,  ...);

public:
//! Verifies the signature of the signed XML string passed as the first argument with the given key manager
/** @par Example:
    @code{.py}
XmlSec::verify(signed_string, mgr);
    @endcode

    @param signed_string the signed XML string to verify
    @param mgr the key manager to use to verify the signed string

    If any errors occur, an exception is thrown

    @throw XMLSEC-SIGN-ERROR: error in arguments to the methods; signature verification failed
    @throw XMLSEC-DSIGCTX-ERROR: signature verification could not be processed by libxmlse
*/
static nothing verify(string signed_string, XmlSecKeyManager mgr,  ...);
};
/** @defgroup xmlsec_constants xmlsec Module Constants
    xmlsec module constants
*/
///@{

//! the version of this module
    const ModuleVersion = qore(new QoreStringNode(PACKAGE_VERSION));
///@}
/** @defgroup xmlsec_keydataid_constants xmlsec Key Data ID Constants
    Key data ID constants
*/
///@{

//! AES
    const xmlSecKeyDataAesId = XMLSEC_KEYDATA_AESID;
//! DES
    const xmlSecKeyDataDesId = XMLSEC_KEYDATA_DESID;
//! DSA
    const xmlSecKeyDataDsaId = XMLSEC_KEYDATA_DSAID;
//! HMAC
    const xmlSecKeyDataHmacId = XMLSEC_KEYDATA_HMACID;
//! Raw X.509 certificate
    const xmlSecKeyDataRawX509CertId = XMLSEC_KEYDATA_RAWX509CERTID;
//! RSA
    const xmlSecKeyDataRsaId = XMLSEC_KEYDATA_RSAID;
//! X.509
    const xmlSecKeyDataX509Id = XMLSEC_KEYDATA_X509ID;
///@}
/** @defgroup xmlsec_keydataformat_constants xmlsec Key Format Constants
    Key data format constants
*/
///@{

//! binary
    const xmlSecKeyDataFormatBinary = qore(xmlSecKeyDataFormatBinary);
//! Certificate DER
    const xmlSecKeyDataFormatCertDer = qore(xmlSecKeyDataFormatCertDer);
//! Certificate PEM
    const xmlSecKeyDataFormatCertPem = qore(xmlSecKeyDataFormatCertPem);
//! DER
    const xmlSecKeyDataFormatDer = qore(xmlSecKeyDataFormatDer);
//! PEM
    const xmlSecKeyDataFormatPem = qore(xmlSecKeyDataFormatPem);
//! PKCS12
    const xmlSecKeyDataFormatPkcs12 = qore(xmlSecKeyDataFormatPkcs12);
//! PKCS8DER
    const xmlSecKeyDataFormatPkcs8Der = qore(xmlSecKeyDataFormatPkcs8Der);
//! PKCS8PEM
    const xmlSecKeyDataFormatPkcs8Pem = qore(xmlSecKeyDataFormatPkcs8Pem);
//! unknown
    const xmlSecKeyDataFormatUnknown = qore(xmlSecKeyDataFormatUnknown);
///@}
/** @defgroup xmlsec_keydatatype_constants xmlsec Key Type Constants
    Key data type constants
*/
///@{

//! any
    const xmlSecKeyDataTypeAny = qore(xmlSecKeyDataTypeAny);
//! none
    const xmlSecKeyDataTypeNone = qore(xmlSecKeyDataTypeNone);
//! permanent
    const xmlSecKeyDataTypePermanent = qore(xmlSecKeyDataTypePermanent);
//! private
    const xmlSecKeyDataTypePrivate = qore(xmlSecKeyDataTypePrivate);
//! public
    const xmlSecKeyDataTypePublic = qore(xmlSecKeyDataTypePublic);
//! session
    const xmlSecKeyDataTypeSession = qore(xmlSecKeyDataTypeSession);
//! symmetric
    const xmlSecKeyDataTypeSymmetric = qore(xmlSecKeyDataTypeSymmetric);
//! trusted
    const xmlSecKeyDataTypeTrusted = qore(xmlSecKeyDataTypeTrusted);
//! unknown
    const xmlSecKeyDataTypeUnknown = qore(xmlSecKeyDataTypeUnknown);
///@}
}
}
