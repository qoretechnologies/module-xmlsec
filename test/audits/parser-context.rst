XML Security parser context and documentation review
====================================================

Copyright 2026 Qore Technologies, s.r.o.

Scope: CMake policy/docs, QoreXmlDoc, module initialization, parser error
propagation, documentation and parser/concurrency regressions. Packaging files
are reviewed separately. Baseline worker-thread regression fails before the
fix; target libxml2 versions are 2.12.10, 2.13.8 and 2.12.5. Evidence is retained
in qore-packaging/results/*-xmlsec-concurrent-1.json and xmlsec-doc-policy-1.json.

.. list-table:: Complete audit-changes checklist
   :header-rows: 1
   :widths: 48 8 44

   * - Check
     - Status
     - Evidence

   * - Entry exists in doxygen/lang/120_modules.dox.tmpl (for modules in the Qore repo; N/A for external module repos)
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - Entry exists in doxygen/lang/900_release_notes.dox.tmpl (for modules in the Qore repo; external modules have release notes in their .qm)
     - Pass
     - README, RELEASE-NOTES and the module mainpage describe parser context isolation, exception-safe serialization and strict documentation.

   * - qore_user_module() or qore_external_user_module() call in CMakeLists.txt
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - Module added to QMOD list in CMakeLists.txt
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - .qm file has @section <lowercasemodname>intro as first doc section — must be all lowercase (e.g., avrodataproviderintro, not AvroDataProviderintro)
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - %modern in .qm file — no redundant %new-style, %require-types, %strict-args, %enable-all-warnings
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - No parse directives (%requires, %modern, %new-style) in separated .qc files (check OUTSIDE of @code blocks only)
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - No %include usage (deprecated for modules)
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - Copyright 2026 on all new files
     - Pass
     - Updated native sources and test carry 2026 notices; new audit has a 2026 notice.

   * - Directory layout: .qm inside qlib/<ModuleName>/ directory (not at qlib/<ModuleName>.qm for multi-file modules)
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - No second .qm for the same module at qlib/<ModuleName>.qm
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - ns=Qore::XX matches the QoreNamespace constructor path
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - %modern directive present
     - Pass
     - The existing Qore suite retains %modern.

   * - Executable permission set (chmod +x)
     - Pass
     - test/xmlsec.qtest is executable.

   * - Uses %prepend-module-path  before %requires for in-repo modules (Qore and Qore modules only; not Qorus)
     - Pass
     - Binary tests preload the exact development artifact; existing in-repo build prepend remains. QUnit is supplied by the Qore SDK.

   * - External module dependencies use %try-module — except modules delivered with the project itself (Qore ex: DataProvider, ConnectionProvider, QUnit, etc.) which use hard %requires
     - Pass
     - The external XML dependency retains %try-module; package qualification requires it before running the suite. The same-repository xmlsec module remains mandatory.

   * - No filesystem operations (fopen, open, creat, unlink, remove, rename, mkdir, rmdir, stat, chmod) without sandbox checks
     - Pass
     - No direct filesystem operations added. Existing libxml2 entity/DTD loading policy is preserved by explicit per-document options.

   * - No network operations (connect, bind, socket, getaddrinfo, gethostbyname) without sandbox checks
     - Pass
     - No socket/network operations added; external resource semantics are unchanged from the loading thread. Resource-loading API redesign is outside this parser-state fix.

   * - If filesystem/network ops exist, verify QoreSandboxManagerHelper usage
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - No File::, Dir::, Socket::, HTTPClient:: usage without justification
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - All for/while loops that could iterate >100 times have qore_check_cancel() checks
     - Pass
     - No production loops added; the test uses three iterations per worker.

   * - Uses qore_check_cancel() (NOT deprecated qore_check_io_interrupt())
     - Pass
     - No deprecated Qore cancellation entry points introduced.

   * - Check frequency: every 100 iterations for tight loops, every 10 for expensive iterations
     - Pass
     - The bounded test loop runs three iterations; no cancellation polling required.

   * - No blocking operations without cancellation support
     - Pass
     - No new blocking primitive; the existing XML parse now has its own context and error handler.

   * - Every action has display_name, short_desc (plain text, <80 chars), desc (markdown)
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - Every action has options populated via getActionOptionFromFields() — without this, the action shows an empty, unusable form
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - Every action has output_type set to a typed data type constant (e.g., MyResponseDataType) — not omitted
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - DPAT_API actions: provider has "supports_request": True and implements doRequestImpl()
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - DPAT_FIND actions: every option exists in SearchOptions, getRecordTypeImpl() returns *hash<string, AbstractDataField>
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - Scheme-based apps (with "scheme" in registerApp): actions use "path" and do NOT use "cls" — having both scheme and cls causes a runtime error
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - Single-key hash slices use trailing comma: Fields{"key",} (without trailing comma, Fields{"key"} returns the value, not a hash)
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - Typed data type classes exist for request and response types — inherit HashDataType, have const Fields hash, call addQoreFields(Fields) in constructor, export public constant at bottom (e.g., public const MyDataType = new MyDataType();)
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - Request/input types use public Fields (enables ClassName::Fields in action registration)
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - Response/output types use private Fields
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - Each field in data types has display_name, type, and desc (markdown-formatted)
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - Input fields have example_value where useful (string fields, endpoint URIs, SQL queries, etc.)
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - Fields with finite allowed values use allowed_values with AllowedValueInfo containing both value and display_name (Title Case, human-readable) — never bare values, never described only in text
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - Password/secret fields have "sensitive": True
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - groups uses AppGroup enum values from qlib/DataProvider/AppGroup.qc
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - App logo stored as separate file, loaded at module level in Priv namespace
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - App desc uses markdown: bullet list of capabilities, links to project website, business-language explanation of value
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - display_name is user-friendly ("Apache Avro" not "avro")
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - short_desc is plain text, under 80 chars, single sentence — no markdown
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - desc uses markdown: backticks for code/field refs ( field_name ,  True ,  pdf ), \n\n for paragraphs, -  bullet lists for enumerations, bold for caveats
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - Descriptions use plain business language relating to common challenges — not just technical "what" but "why" and "when to use"
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - No bare True/False/NOTHING — must be backtick-wrapped in desc
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - No bare field/option names in prose — must use backticks
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - Long descriptions (>500 chars) use bold section headers and bullet lists
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - Factory registration in Qore repo: every factory name registered in qlib/DataProvider/DataProvider.qc → FactoryMap (without this, module loads but doesn't appear in Qorus apps)
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - getRecordTypeImpl() signature: must be private *hash<string, AbstractDataField> getRecordTypeImpl(*hash<auto> search_options) — NOT returning *AbstractDataProviderType
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - Dependency JARs committed (for JNI modules): JAR files in qlib/*/jar/ may be gitignored — use git add -f to ensure they're tracked, otherwise CI compilation fails
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - JAR install rules in CMakeLists.txt for all dependency JARs
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - No workarounds: No TODOs, FIXMEs, stubs, or partially-implemented features
     - Pass
     - A failing regression demonstrates lost DTD defaults/entities on worker threads. Explicit parser options fix the cause. No warning suppressions; parser failures become detailed Qore exceptions, and parser warnings remain visible.

   * - Exception safety: C++ uses ReferenceHolder for Qore allocations, std::unique_ptr for C++ allocations, *xsink checked after every fallible operation
     - Pass
     - unique_ptr owns document, parser context and serialized XML buffer. Allocation failure cannot leak the serialization buffer; C error callbacks are noexcept and use fixed-size bounded storage.

   * - Thread safety: All mutable shared state protected by std::lock_guard<std::mutex> or documented as immutable-after-construction
     - Pass
     - Each document owns its parser context and error buffer. Module load no longer changes libxml2 thread-local defaults. Four-thread sign/verify/encrypt/decrypt checks now assert their error and iteration totals.

   * - Type safety: Strongly-typed code<return(args)> instead of untyped code; static_cast instead of C casts; typed hashdecls for results; enums where appropriate
     - Pass
     - Typed libxml objects and unique_ptr deleters; explicit C++ casts; callback signature is selected for libxml2 versions before/after the const-error API transition.

   * - Performance: No O(n²) where O(n) is possible; no unnecessary copies; coordinate descent uses incremental residuals not full matrix multiply
     - Pass
     - Per-document context replaces the context implicitly created by xmlParseDoc. Error capture is bounded and allocation-free; no repeated document copies added.

   * - Error handling: All inputs validated (dimensions, empty data, unfitted models); C++ I/O handles EAGAIN/EINTR if applicable
     - Pass
     - Malformed and empty inputs are covered; original Qore error names are preserved and include parser details. Both pre-2.13 SAX and modern per-context error handlers pass on the target libraries.

   * - Documentation: Doxygen @param, @return, @throw on all public methods; @par Example with realistic business scenarios; @note for important caveats
     - Pass
     - Two mainpage references incorrectly included their trailing colon in the target; explicit reference labels fix them. Strict Doxygen policy survives regeneration and rejects a deliberately invalid reference on all targets.

   * - QPP flags: [flags=CONSTANT] on methods that never throw; [flags=RET_VALUE_ONLY] on methods that throw but have no side effects
     - N/A
     - No new Qore module, QPP class, DataProvider registration or Java dependency. Existing public QPP method flags and signatures are unchanged.

   * - Security: No user-controlled format strings; no buffer overflows; bounds checking on array indices; no credentials in code
     - Pass
     - Error callback uses fixed format strings and bounded storage, without allocations or global handler changes. Test identities are public fixtures. Existing entity/DTD policy is retained.

   * - Correctness: Algorithms verified against reference implementations; edge cases tested (empty data, single sample, all-zero features)
     - Pass
     - All 13 cases/65 assertions pass normally and under Valgrind on Fedora, Leap and EL10, including 12 cryptographic iterations across four threads. Zero memory errors, lost allocations or suppressions. Strict positive/negative docs tests pass on all three.
