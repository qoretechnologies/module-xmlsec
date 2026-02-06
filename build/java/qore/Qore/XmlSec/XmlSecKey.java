// Java created from Qore class Qore::XmlSec::XmlSecKey
package qore.Qore.XmlSec;

/** @brief The \c XmlSecKey class implements an \c xmlsec key (wrapper for an \c xmlSecKey C++ structure
   */
public class XmlSecKey {
    // XmlSecKey::constructor(data key, int format, *string password) {}
    /** @brief Creates a new \c XmlSecKey based on the arguments passed.
     @par Example:
        @code{.py}
    XmlSec key(key_pem, xmlSecKeyDataFormatPem, password);
        @endcode
    
        @param key the key in PEM or DER format
        @param format the format of the key (for possible values, see @ref xmlsec_keydataformat_constants for possible values)
        @param password the optional password for the key
    
        Creates a new key based on key data (for example, in PEM or DER format), the second argument will normally be xmlSecKeyDataFormatPem or xmlSecKeyDataFormatDer.
    
        The password argument is required for private keys with a password.
    */
    public XmlSecKey(qore.Qore.data key, long format, String password) throws Throwable {
    }

    // XmlSecKey::constructor(string str, int num_bits, int type) {}
    /** @brief Creates a new \c XmlSecKey based on the arguments passed.
     @par Example:
        @code{.py}
    XmlSec key(str, num_bits, type);
        @endcode
    
        @param key the key in PEM or DER format
        @param num_bits the number of bits
        @param type the key type; see @ref xmlsec_keydatatype_constants for possible values
    
        Creates a new key based on key data (for example, in PEM or DER format), the second argument will normally be xmlSecKeyDataFormatPem or xmlSecKeyDataFormatDer.
    */
    public XmlSecKey(String str, long num_bits, long type) throws Throwable {
    }

    // XmlSecKey::constructor(int id, int num_bits, int type) {}
    /** @brief Creates a new \c XmlSecKey based on the arguments passed.
     @par Example:
        @code{.py}
    XmlSec key(int id, num_bits, type);
        @endcode
    
        @param id an integer constant giving the key ID; see @ref xmlsec_keydataid_constants for possible values
        @param num_bits the number of bits
        @param type the key type; see @ref xmlsec_keydatatype_constants for possible values
    
        @throw XMLSECKEY-KEYID-ERROR invalid key ID given
    */
    public XmlSecKey(long id, long num_bits, long type) throws Throwable {
    }

    // string XmlSecKey::getName(){}
    /** @brief Returns the key name; if no key name is set, returns @ref nothing
     @par Code Flags:
        @ref RET_VALUE_ONLY
    
        @par Example:
        @code{.py}
    *string name = key.getName();
        @endcode
    
        @throw XMLSECKEY-ERROR error reported by libxmlsec getting the name
    */
    public String getName() throws Throwable {
    }

    // int XmlSecKey::getSize(){}
    /** @brief Returns the key's bit size as an integer
     @par Code Flags:
        @ref RET_VALUE_ONLY
    
        @par Example:
        @code{.py}
    int size = key.getSize());
        @endcode
    
        @return the key's bit size as an integer
    
        @throw XMLSECKEY-ERROR error reported by libxmlsec reading the key
    */
    public long getSize() throws Throwable {
    }

    // int XmlSecKey::getType(){}
    /** @brief Returns the key's type as an integer value; see @ref xmlsec_keydatatype_constants for possible values
     @par Code Flags:
        @ref RET_VALUE_ONLY
    
        @par Example:
        @code{.py}
    int type = key.getType());
        @endcode
    
        @throw XMLSECKEY-ERROR error reported by libxmlsec getting the type
    */
    public long getType() throws Throwable {
    }

    // nothing XmlSecKey::setCertificate(data cert, int format){}
    /** @brief Assigns an X.509 certificate to the \c XmlSecKey object
     @par Example:
        @code{.py}
    key.setCertificate(cert_pem, xmlSecKeyDataFormatCertPem);
        @endcode
    
        @param cert the certificate in PEM or DER format
        @param format the format of the key (for possible values, see @ref xmlsec_keydataformat_constants for possible values)
    */
    public void setCertificate(qore.Qore.data cert, long format) throws Throwable {
    }

    // nothing XmlSecKey::setCertificate(SSLCertificate cert){}
    /** @brief Assigns an X.509 certificate to the \c XmlSecKey object
     @par Example:
        @code{.py}
    key.setCertificate(cert);
        @endcode
    
        @param cert X.509 certificate to set
    */
    public void setCertificate(qore.Qore.SSLCertificate cert) throws Throwable {
    }

    // nothing XmlSecKey::setName(string name){}
    /** @brief Sets the key name in the \c XmlSecKey object
     @par Example:
        @code{.py}
    key.setName(name);
        @endcode
    
        @throw XMLSECKEY-ERROR error reported by libxmlsec setting the name
    */
    public void setName(String name) throws Throwable {
    }

    // nothing XmlSecKey::verify(string signed_string, ...){}
    /** @brief Verifies the signature of the signed XML string passed
     @par Example:
        @code{.py}
    key.verify(signed_string);
        @endcode
    
        @param signed_string the signed XML string to verify
    
        If any errors occur, an exception is thrown
    
        @throw XMLSEC-SIGN-ERROR: error in arguments to the methods; signature verification failed
        @throw XMLSEC-DSIGCTX-ERROR: signature verification could not be processed by libxmlse
    */
    public void verify(String signed_string) throws Throwable {
    }

}
