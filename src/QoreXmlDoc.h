/*
    Qore Programming Language

    Copyright 2003 - 2026 Qore Technologies, s.r.o.

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

#ifndef _QORE_XMLSEC_QOREXMLDOC_H

#define _QORE_XMLSEC_QOREXMLDOC_H

#include <cstdio>
#include <cstring>
#include <memory>
#include <new>

class QoreXmlDoc {
private:
    std::unique_ptr<xmlDoc, decltype(&xmlFreeDoc)> doc{nullptr, xmlFreeDoc};
    char parse_error[1024] = {};

#if LIBXML_VERSION >= 21200
    using ParserError = const xmlError;
#else
    using ParserError = xmlError;
#endif

    static void parserError(void* data, ParserError* error) noexcept {
        if (!error || !error->message) {
            return;
        }
#if LIBXML_VERSION >= 21300
        auto* self = static_cast<QoreXmlDoc*>(data);
#else
        auto* context = static_cast<xmlParserCtxt*>(data);
        auto* self = static_cast<QoreXmlDoc*>(context->_private);
#endif
        // Expected parse failures are reported by the caller's Qore exception.
        // Preserve warnings instead of silently accepting unexpected diagnostics.
        if (error->level == XML_ERR_WARNING) {
            std::fprintf(stderr, "XML parser warning: %s", error->message);
        } else if (!self->parse_error[0]) {
            std::snprintf(self->parse_error, sizeof(self->parse_error), "%s", error->message);
            self->parse_error[std::strcspn(self->parse_error, "\r\n")] = '\0';
        }
    }

public:
    DLLLOCAL explicit QoreXmlDoc(const char* str) {
        std::unique_ptr<xmlParserCtxt, decltype(&xmlFreeParserCtxt)> context(xmlNewParserCtxt(),
            xmlFreeParserCtxt);
        if (!context) {
            throw std::bad_alloc();
        }
#if LIBXML_VERSION >= 21300
        xmlCtxtSetErrorHandler(context.get(), parserError, this);
#else
        context->_private = this;
        context->sax->serror = parserError;
#endif
        // Preserve entity expansion, external DTD loading and default attributes
        // for every calling thread without changing libxml2's legacy defaults.
        doc.reset(xmlCtxtReadDoc(context.get(), reinterpret_cast<const xmlChar*>(str), nullptr, nullptr,
            XML_PARSE_NOENT | XML_PARSE_DTDLOAD | XML_PARSE_DTDATTR));
    }

    DLLLOCAL operator bool() const {
        return static_cast<bool>(doc);
    }

    DLLLOCAL const char* getParseError() const {
        return parse_error[0] ? parse_error : "XML document has no root element";
    }

    DLLLOCAL xmlNodePtr getRootElement() {
        return xmlDocGetRootElement(doc.get());
    }

    DLLLOCAL xmlNodePtr getChildren() {
        return doc->children;
    }

    DLLLOCAL void dump() {
        xmlDocDump(stdout, doc.get());
    }

    DLLLOCAL QoreStringNode* getString() {
        xmlChar* data = nullptr;
        int size = 0;
        xmlDocDumpMemory(doc.get(), &data, &size);
        std::unique_ptr<xmlChar, xmlFreeFunc> buffer(data, xmlFree);
        if (!buffer) {
            throw std::bad_alloc();
        }
        return new QoreStringNode(reinterpret_cast<const char*>(buffer.get()), static_cast<qore_size_t>(size),
            QCS_UTF8);
    }
};

#endif
