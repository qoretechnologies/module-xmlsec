XML Security RPM packaging
==========================

Copyright 2026 Qore Technologies, s.r.o.

The portable recipe uses the distribution libxml2 and XML Security Library
with the OpenSSL backend. The native module and compiler metadata form the
runtime package; the HTML reference is a separate documentation package.
The Qore XML module is a test dependency. Builds keep strict documentation
and all signing, verification, encryption, key and concurrency tests enabled.

From qore-packaging, prepare and build a committed source bundle::

    python3 tools/packaging.py prepare --repo ../module-xmlsec --ref COMMIT \
      --name qore-xmlsec-module --version 1.0.1 \
      --spec qore-xmlsec-module.spec --output work/xmlsec-source
    python3 tools/build-local.py --source work/xmlsec-source \
      --image TARGET_SDK_IMAGE --output results/xmlsec-build --jobs 2

Installed qualification preloads the packaged XML module and exactly one
XML Security native artifact. It copies the suite and public test identities
to a temporary directory and clears development module/library overrides::

    python3 -B -W error rpm/test_fixture.py -v
    python3 -B -W error rpm/run-tests.py --installed

On an SDK image add ``--compiler`` to compile and execute the named-argument
key-management example. Missing integration dependencies fail qualification
before the suite can take its optional-dependency skip path. The fixture keys
are public test identities and are not deployment credentials.
