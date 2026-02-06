// Java created from Qore class Qore::XmlSec::XmlSecKeyManager
package qore.Qore.XmlSec;

/** @brief The \c XmlSecKeyManager class implements an xmlsec key manager (wrapper for a C++ \c xmlSecKeysManager structure)
   */
public class XmlSecKeyManager {
    // nothing XmlSecKeyManager::addKey(XmlSecKey key){}
    /** @brief adds an @ref Qore::XmlSec::XmlSecKey "XmlSecKey" to the \c XmlSecKeyManager object
     @par Example:
        @code{.py}
    mgr.addKey(key);
        @endcode
    
        @param key the key to add
    
        @throw XMLSECKEYMANAGER-ERROR error reported by libxmlsec assigning the key to the key manager
    */
    public qore.Qore. addKey(qore.Qore.XmlSecKey key) throws Throwable {
    }

    // XmlSecKeyManager::constructor() {}
    /** @brief creates the object
     @par Example:
        @code{.py}
    XmlSecKeyManager mgr();
        @endcode
    
        @throw XMLSECKEYMANAGER-ERROR error reported by \c libxmlsec creating or initializing the key manager
    */
    public XmlSecKeyManager() throws Throwable {
    }

    // nothing XmlSecKeyManager::loadCertFromMemory(data cert_data, int format, int type){}
    /** @brief adds certificate to the \c XmlSecKeyManager object and marks it according tot the arguments
     @par Example:
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
    public qore.Qore. loadCertFromMemory(qore.Qore.data cert_data, long format, long type) throws Throwable {
    }

    // nothing XmlSecKeyManager::loadCertFromPath(string path, int format, int type){}
    /** @brief adds certificate to the \c XmlSecKeyManager object and marks it according tot the arguments
     @par Restrictions:
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
    public qore.Qore. loadCertFromPath(String path, long format, long type) throws Throwable {
    }

    // nothing XmlSecKeyManager::verify(string signed_string, ...){}
    /** @brief Verifies the signature of the signed XML string passed
     @par Example:
        @code{.py}
    mgr.verify(signed_string);
        @endcode
    
        @param signed_string the signed XML string to verify
    
        If any errors occur, an exception is thrown
    
        @throw XMLSEC-SIGN-ERROR: error in arguments to the methods; signature verification failed
        @throw XMLSEC-DSIGCTX-ERROR: signature verification could not be processed by libxmlse
    */
    public void verify(String signed_string) throws Throwable {
    }

}
