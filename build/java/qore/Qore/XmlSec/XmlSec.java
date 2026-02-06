// Java created from Qore class Qore::XmlSec::XmlSec
package qore.Qore.XmlSec;

/** @brief The \c XmlSec class implements static methods for working with XmlSec keys
   */
public class XmlSec {
    // XmlSec::constructor() {}
    /** @brief throws an exception as this class is not meant to be instantiated
     @throw XMLSEC-CONSTRUCTOR-ERROR this class is not meant to be instantiated
    */
    public XmlSec() throws Throwable {
    }

    // static data XmlSec::decrypt(string xml, XmlSecKey key){}
    /** @brief Decrypts the encrypted XML data in the XML string using the given key
     @par Code Flags:
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
    static public qore.Qore.data decrypt(String xml, qore.Qore.XmlSecKey key) throws Throwable {
    }

    // static data XmlSec::decrypt(string xml, XmlSecKeyManager key_manager){}
    /** @brief Decryps the encrypted XML data that was encrypted with a session key using the @ref Qore::XmlSec::XmlSecKeyManager "XmlSecKeyManager" object to decrypt the session key and then decrypt the message using the decrypted session key
     @par Code Flags:
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
    static public qore.Qore.data decrypt(String xml, qore.Qore.XmlSecKeyManager key_manager) throws Throwable {
    }

    // static string XmlSec::encrypt(string str_data, string tmpl, XmlSecKey key, *XmlSecKeyManager key_manager){}
    /** @brief Encrypts data using an XML template and an @ref Qore::XmlSec::XmlSecKey "XmlSecKey" object and optionally an @ref Qore::XmlSec::XmlSecKeyManager "XmlSecKeyManager" object
     @par Example:
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
    static public String encrypt(String str_data, String tmpl, qore.Qore.XmlSecKey key, qore.Qore.XmlSecKeyManager key_manager) throws Throwable {
    }

    // static string XmlSec::encrypt(binary bin_data, string tmpl, XmlSecKey key, *XmlSecKeyManager key_manager){}
    /** @brief Encrypts data using an XML template and an @ref Qore::XmlSec::XmlSecKey "XmlSecKey" object and optionally an @ref Qore::XmlSec::XmlSecKeyManager "XmlSecKeyManager" object
     @par Example:
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
    static public String encrypt(byte[] bin_data, String tmpl, qore.Qore.XmlSecKey key, qore.Qore.XmlSecKeyManager key_manager) throws Throwable {
    }

    // static string XmlSec::sign(string tmpl, XmlSecKey key){}
    /** @brief Creates a signed XML string based on an XML template string and an @ref Qore::XmlSec::XmlSecKey "XmlSecKey" object
     @par Code Flags:
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
    static public String sign(String tmpl, qore.Qore.XmlSecKey key) throws Throwable {
    }

    // static nothing XmlSec::verify(string signed_string, XmlSecKey key, ...){}
    /** @brief Verifies the signature of the signed XML string passed as the first argument with the given key
     @par Example:
        @code{.py}
    XmlSec::verify(signed_string, key);
        @endcode
    
        @param signed_string the signed XML string to verify
        @param key the key to use to verify the signed string
    
        If any errors occur, an exception is thrown
    
        @throw XMLSEC-SIGN-ERROR: error in arguments to the methods; signature verification failed
        @throw XMLSEC-DSIGCTX-ERROR: signature verification could not be processed by libxmlse
    */
    static public void verify(String signed_string, qore.Qore.XmlSecKey key) throws Throwable {
    }

    // static nothing XmlSec::verify(string signed_string, XmlSecKeyManager mgr, ...){}
    /** @brief Verifies the signature of the signed XML string passed as the first argument with the given key manager
     @par Example:
        @code{.py}
    XmlSec::verify(signed_string, mgr);
        @endcode
    
        @param signed_string the signed XML string to verify
        @param mgr the key manager to use to verify the signed string
    
        If any errors occur, an exception is thrown
    
        @throw XMLSEC-SIGN-ERROR: error in arguments to the methods; signature verification failed
        @throw XMLSEC-DSIGCTX-ERROR: signature verification could not be processed by libxmlse
    */
    static public void verify(String signed_string, qore.Qore.XmlSecKeyManager mgr) throws Throwable {
    }

}
