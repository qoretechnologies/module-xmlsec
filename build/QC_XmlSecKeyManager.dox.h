//! Qore namespace
namespace Qore {
//! XmlSec namespace
namespace XmlSec {
//! The \c XmlSecKeyManager class implements an xmlsec key manager (wrapper for a C++ \c xmlSecKeysManager structure)
/***/
class XmlSecKeyManager {

public:
//! adds an @ref Qore::XmlSec::XmlSecKey "XmlSecKey" to the \c XmlSecKeyManager object
/** @par Example:
    @code{.py}
mgr.addKey(key);
    @endcode

    @param key the key to add

    @throw XMLSECKEYMANAGER-ERROR error reported by libxmlsec assigning the key to the key manager
*/
 addKey(XmlSecKey key);

public:
//! creates the object
/** @par Example:
    @code{.py}
XmlSecKeyManager mgr();
    @endcode

    @throw XMLSECKEYMANAGER-ERROR error reported by \c libxmlsec creating or initializing the key manager
*/
 constructor();

public:
//! throws an exception; \c XmlSecKeyManager objects cannot be copied
/** @throw XMLSECKEYMANAGER-COPY-ERROR XmlSecKeyManager objects cannot be copied
*/
 copy();

public:
//! adds certificate to the \c XmlSecKeyManager object and marks it according tot the arguments
/** @par Example:
    @code{.py}
mgr.loadCertFromMemory(cert_data, xmlSecKeyDataFormatCertPem, xmlSecKeyDataTypeTrusted);
    @endcode

    @param cert_data the certificate data
    @param format the format of the file data (for possible values, see @ref xmlsec_keydataformat_constants for
    possible values)
    @param type the certificate type; see @ref xmlsec_keydatatype_constants for possible values

    @throw XMLSECKEYMANAGER-ERROR error reported by libxmlsec loading the certificate or assigning the certificate to
    the key manager
*/
 loadCertFromMemory(data cert_data, int format, int type);

public:
//! adds certificate to the \c XmlSecKeyManager object and marks it according tot the arguments
/** @par Restrictions:
    @ref Qore::PO_NO_FILESYSTEM

    @par Example:
    @code{.py}
mgr.loadCertFromPath(path, xmlSecKeyDataFormatCertPem, xmlSecKeyDataTypeTrusted);
    @endcode

    @param path the path to the certificate on the filesystem
    @param format the format of the file data (for possible values, see @ref xmlsec_keydataformat_constants for
    possible values)
    @param type the certificate type; see @ref xmlsec_keydatatype_constants for possible values

    @throw XMLSECKEYMANAGER-ERROR error reported by libxmlsec loading the certificate or assigning the certificate to
    the key manager
*/
 loadCertFromPath(string path, int format, int type);

public:
//! Verifies the signature of the signed XML string passed
/** @par Example:
    @code{.py}
mgr.verify(signed_string);
    @endcode

    @param signed_string the signed XML string to verify

    If any errors occur, an exception is thrown

    @throw XMLSEC-SIGN-ERROR: error in arguments to the methods; signature verification failed
    @throw XMLSEC-DSIGCTX-ERROR: signature verification could not be processed by libxmlse
*/
nothing verify(string signed_string,  ...);
};
}
}
