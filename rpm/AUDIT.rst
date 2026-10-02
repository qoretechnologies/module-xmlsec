XML Security RPM review
=======================

Copyright 2026 Qore Technologies, s.r.o.

Scope: portable spec and rpm/ qualification helper, tests and README. Native
changes have test/audits/parser-context.rst. Canonical builds remain required.

.. list-table:: Complete audit-changes checklist
   :header-rows: 1

   * - Check
     - Status
     - Evidence

   * - Entry exists in doxygen/lang/120_modules.dox.tmpl (for modules in the Qore repo; N/A for external module repos)
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Entry exists in doxygen/lang/900_release_notes.dox.tmpl (for modules in the Qore repo; external modules have release notes in their .qm)
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - qore_user_module() or qore_external_user_module() call in CMakeLists.txt
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Module added to QMOD list in CMakeLists.txt
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - .qm file has @section <lowercasemodname>intro as first doc section — must be all lowercase (e.g., avrodataproviderintro, not AvroDataProviderintro)
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - %modern in .qm file — no redundant %new-style, %require-types, %strict-args, %enable-all-warnings
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - No parse directives (%requires, %modern, %new-style) in separated .qc files (check OUTSIDE of @code blocks only)
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - No %include usage (deprecated for modules)
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Copyright 2026 on all new files
     - Pass
     - New spec, fixture, tests and README carry 2026 copyright notices.

   * - Directory layout: .qm inside qlib/<ModuleName>/ directory (not at qlib/<ModuleName>.qm for multi-file modules)
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - No second .qm for the same module at qlib/<ModuleName>.qm
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - ns=Qore::XX matches the QoreNamespace constructor path
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - %modern directive present
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Executable permission set (chmod +x)
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Uses %prepend-module-path  before %requires for in-repo modules (Qore and Qore modules only; not Qorus)
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - External module dependencies use %try-module — except modules delivered with the project itself (Qore ex: DataProvider, ConnectionProvider, QUnit, etc.) which use hard %requires
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - No filesystem operations (fopen, open, creat, unlink, remove, rename, mkdir, rmdir, stat, chmod) without sandbox checks
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - No network operations (connect, bind, socket, getaddrinfo, gethostbyname) without sandbox checks
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - If filesystem/network ops exist, verify QoreSandboxManagerHelper usage
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - No File::, Dir::, Socket::, HTTPClient:: usage without justification
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - All for/while loops that could iterate >100 times have qore_check_cancel() checks
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Uses qore_check_cancel() (NOT deprecated qore_check_io_interrupt())
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Check frequency: every 100 iterations for tight loops, every 10 for expensive iterations
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - No blocking operations without cancellation support
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Every action has display_name, short_desc (plain text, <80 chars), desc (markdown)
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Every action has options populated via getActionOptionFromFields() — without this, the action shows an empty, unusable form
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Every action has output_type set to a typed data type constant (e.g., MyResponseDataType) — not omitted
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - DPAT_API actions: provider has "supports_request": True and implements doRequestImpl()
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - DPAT_FIND actions: every option exists in SearchOptions, getRecordTypeImpl() returns *hash<string, AbstractDataField>
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Scheme-based apps (with "scheme" in registerApp): actions use "path" and do NOT use "cls" — having both scheme and cls causes a runtime error
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Single-key hash slices use trailing comma: Fields{"key",} (without trailing comma, Fields{"key"} returns the value, not a hash)
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Typed data type classes exist for request and response types — inherit HashDataType, have const Fields hash, call addQoreFields(Fields) in constructor, export public constant at bottom (e.g., public const MyDataType = new MyDataType();)
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Request/input types use public Fields (enables ClassName::Fields in action registration)
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Response/output types use private Fields
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Each field in data types has display_name, type, and desc (markdown-formatted)
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Input fields have example_value where useful (string fields, endpoint URIs, SQL queries, etc.)
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Fields with finite allowed values use allowed_values with AllowedValueInfo containing both value and display_name (Title Case, human-readable) — never bare values, never described only in text
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Password/secret fields have "sensitive": True
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - groups uses AppGroup enum values from qlib/DataProvider/AppGroup.qc
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - App logo stored as separate file, loaded at module level in Priv namespace
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - App desc uses markdown: bullet list of capabilities, links to project website, business-language explanation of value
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - display_name is user-friendly ("Apache Avro" not "avro")
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - short_desc is plain text, under 80 chars, single sentence — no markdown
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - desc uses markdown: backticks for code/field refs ( field_name ,  True ,  pdf ), \n\n for paragraphs, -  bullet lists for enumerations, bold for caveats
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Descriptions use plain business language relating to common challenges — not just technical "what" but "why" and "when to use"
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - No bare True/False/NOTHING — must be backtick-wrapped in desc
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - No bare field/option names in prose — must use backticks
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Long descriptions (>500 chars) use bold section headers and bullet lists
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Factory registration in Qore repo: every factory name registered in qlib/DataProvider/DataProvider.qc → FactoryMap (without this, module loads but doesn't appear in Qorus apps)
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - getRecordTypeImpl() signature: must be private *hash<string, AbstractDataField> getRecordTypeImpl(*hash<auto> search_options) — NOT returning *AbstractDataProviderType
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Dependency JARs committed (for JNI modules): JAR files in qlib/*/jar/ may be gitignored — use git add -f to ensure they're tracked, otherwise CI compilation fails
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - JAR install rules in CMakeLists.txt for all dependency JARs
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - No workarounds: No TODOs, FIXMEs, stubs, or partially-implemented features
     - Pass
     - Distribution XML security libraries remain in use; XML is required before tests can skip it. Strict docs and all tests remain enabled.

   * - Exception safety: C++ uses ReferenceHolder for Qore allocations, std::unique_ptr for C++ allocations, *xsink checked after every fallible operation
     - Pass
     - TemporaryDirectory owns fixture files; subprocesses have checked exit codes and time limits.

   * - Thread safety: All mutable shared state protected by std::lock_guard<std::mutex> or documented as immutable-after-construction
     - Pass
     - No shared mutable state added. Four-thread native cryptographic regression runs by default.

   * - Type safety: Strongly-typed code<return(args)> instead of untyped code; static_cast instead of C casts; typed hashdecls for results; enums where appropriate
     - Pass
     - Native module lookup validates exact artifact cardinality; subprocess commands use argument lists.

   * - Performance: No O(n²) where O(n) is possible; no unnecessary copies; coordinate descent uses incremental residuals not full matrix multiply
     - Pass
     - One native lookup and one copied suite per qualification; no polling or unnecessary build loops.

   * - Error handling: All inputs validated (dimensions, empty data, unfitted models); C++ I/O handles EAGAIN/EINTR if applicable
     - Pass
     - Unit tests reject missing/duplicate native artifacts. XML dependency preload is mandatory; compiler/runtime subprocess failures propagate.

   * - Documentation: Doxygen @param, @return, @throw on all public methods; @par Example with realistic business scenarios; @note for important caveats
     - Pass
     - RPM README includes build/runtime/compiler commands and explains public test certificates and separate reference package.

   * - QPP flags: [flags=CONSTANT] on methods that never throw; [flags=RET_VALUE_ONLY] on methods that throw but have no side effects
     - N/A
     - Packaging/Python fixture changes add no Qore module, QPP/C++ implementation, DataProvider registration or Java dependency.

   * - Security: No user-controlled format strings; no buffer overflows; bounds checking on array indices; no credentials in code
     - Pass
     - Host module/library overrides are cleared; installed suites run in a temporary directory and cannot select development artifacts. Public keys are fixtures, not deployment credentials.

   * - Correctness: Algorithms verified against reference implementations; edge cases tested (empty data, single sample, all-zero features)
     - Pass
     - Candidate 1 RPM builds, installed runtime and SDK checks pass on Fedora, Leap and EL10. Three fixture regressions, 13 native cases/65 assertions, four-thread cryptographic checks, named-argument compiler example and actual HTML index verification pass. Native parser Valgrind audit is separate.
