# Copyright (C) 2026 Qore Technologies, s.r.o.
# SPDX-License-Identifier: MIT
# Use the pinned source epoch for RPM headers and installed file timestamps.
%global source_date_epoch_from_changelog 1
%global use_source_date_epoch_as_buildtime 1
%if v"%{rpmversion}" >= v"4.20"
%global build_mtime_policy clamp_to_source_date_epoch
%else
%global clamp_mtime_to_source_date_epoch 1
%endif
%bcond_without tests
%bcond_without docs
Name: qore-xmlsec-module
Version: 1.0.1
Release: 2%{?dist}
Summary: XML signing, verification and encryption for Qore
License: LGPL-2.1-or-later
URL: https://github.com/qoretechnologies/module-xmlsec
Source0: %{name}-%{version}.tar.xz
BuildRequires: cmake >= 3.5
BuildRequires: make
BuildRequires: gcc-c++
BuildRequires: pkgconfig(xmlsec1-openssl)
BuildRequires: pkgconfig(libxml-2.0)
BuildRequires: python3
%if %{with tests}
BuildRequires: qore-xml-module >= 2.3.0
%endif
BuildRequires: qore-devel >= 3.0.0~
BuildRequires: qore-rpm-macros >= 3.0.0~
%if %{with docs}
BuildRequires: doxygen
%if 0%{?suse_version}
BuildRequires: util-linux
%else
BuildRequires: util-linux-core
%endif
%endif

%description
Native XML signatures, verification, encryption, decryption and key management
using the distribution XML Security Library with its OpenSSL backend. Includes
compiler metadata; the XML integration module is needed only by the test suite.

%if %{with docs}
%package doc
Summary: XML Security reference documentation
BuildArch: noarch
%description doc
API reference for Qore's XML Security classes.
%endif

%prep
%autosetup
%build
%{?set_build_flags}
. %{_rpmconfigdir}/qore/module-env.sh
qore_set_source_prefix_maps "%{qore_debug_source_dir}"
cmake -S . -B build -G 'Unix Makefiles' \
  -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS_RELEASE=-DNDEBUG \
  -DCMAKE_INSTALL_PREFIX=%{_prefix} -DCMAKE_INSTALL_LIBDIR=%{_lib} \
  -DCMAKE_SKIP_RPATH=ON -DCMAKE_IGNORE_PREFIX_PATH=/usr/local \
  -DQore_DIR=%{_libdir}/cmake/Qore -DQORE_EXECUTABLE=/usr/bin/qore \
  -DQORE_QPP_EXECUTABLE=/usr/bin/qpp -DQORE_QCC_EXECUTABLE=/usr/bin/qcc \
  -DQORE_XMLSEC_STRICT_DOCS=ON \
  -DCMAKE_DISABLE_FIND_PACKAGE_Doxygen=%{!?with_docs:ON}%{?with_docs:OFF}
cmake --build build -- %{?_smp_mflags}
%if %{with docs}
cmake --build build --target docs -- %{?_smp_mflags}
%endif
%install
DESTDIR=%{buildroot} cmake --install build
chmod 755 %{buildroot}%{_libdir}/qore-modules/xmlsec-api-*.qmod
%if %{with docs}
install -d %{buildroot}%{_docdir}/%{name}-doc
cp -a build/docs/xmlsec/html %{buildroot}%{_docdir}/%{name}-doc/
hardlink -t -O %{buildroot}%{_docdir}/%{name}-doc
%endif
%check
%if %{with tests}
. %{_rpmconfigdir}/qore/module-env.sh
python3 -B -W error rpm/test_fixture.py -v
python3 -B -W error rpm/run-tests.py --build-dir "$PWD/build"
%endif
%files
%license COPYING
%doc README RELEASE-NOTES
%{_libdir}/qore-modules/xmlsec-api-*.qmod
%dir %{_datadir}/qore/metadata/xmlsec
%{_datadir}/qore/metadata/xmlsec/*.meta.json
%if %{with docs}
%files doc
%license COPYING
%doc %{_docdir}/%{name}-doc/
%endif
%changelog
* Fri Oct 02 2026 David Nichols <david@qore.org> - 1.0.1-2
- Package native XML Security bindings, compiler metadata and API reference.
- Require all XML integration tests and isolate installed package qualification.
