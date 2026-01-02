%define module_api %(qore --module-api 2>/dev/null)
%define module_dir %{_libdir}/qore-modules

%if 0%{?sles_version}

%define dist .sles%{?sles_version}

%else
%if 0%{?suse_version}

# get *suse release major version
%define os_maj %(echo %suse_version|rev|cut -b3-|rev)
# get *suse release minor version without trailing zeros
%define os_min %(echo %suse_version|rev|cut -b-2|rev|sed s/0*$//)

%if %suse_version > 1010
%define dist .opensuse%{os_maj}_%{os_min}
%else
%define dist .suse%{os_maj}_%{os_min}
%endif

%endif
%endif

# see if we can determine the distribution type
%if 0%{!?dist:1}
%define rh_dist %(if [ -f /etc/redhat-release ];then cat /etc/redhat-release|sed "s/[^0-9.]*//"|cut -f1 -d.;fi)
%if 0%{?rh_dist}
%define dist .rhel%{rh_dist}
%else
%define dist .unknown
%endif
%endif

Summary: XML Security Module for Qore
Name: qore-xmlsec-module
Version: 1.0.1
Release: 1%{dist}
License: LGPL-2.1-or-later
Group: Development/Languages
URL: https://qoretechnologies.com/qore
Source: https://github.com/qoretechnologies/module-xmlsec/releases/download/v%{version}/%{name}-%{version}.tar.bz2
BuildRoot: %{_tmppath}/%{name}-%{version}-%{release}-root
Requires: /usr/bin/env
Requires: qore-module-api-%{module_api}
BuildRequires: gcc-c++
BuildRequires: cmake >= 2.8.12
BuildRequires: qore-devel >= 1.0
BuildRequires: qore >= 1.0
BuildRequires: libxml2-devel
Requires: xmlsec1
Requires: xmlsec1-openssl
BuildRequires: xmlsec1-devel
BuildRequires: xmlsec1-openssl-devel
%if 0%{?suse_version}
BuildRequires: pkg-config
%else
BuildRequires: pkgconfig
%endif

%description
This module provides classes and functions supporting the xmlenc and xmldsig
standards from the xmlsec library for the Qore Programming Language.

%if 0%{?suse_version}
%debug_package
%endif

%prep
%setup -q

%build
%if 0%{?el7}
# enable devtoolset-7 for C++11 support on RHEL 7
. /opt/rh/devtoolset-7/enable
%endif
export CXXFLAGS="%{?optflags}"
cmake -DCMAKE_INSTALL_PREFIX=%{_prefix} -DCMAKE_BUILD_TYPE=RELEASE .
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root,-)
%{module_dir}
%doc COPYING README RELEASE-NOTES ChangeLog AUTHORS

%changelog
* Thu Jan 02 2025 David Nichols <david@qore.org> 1.0.1
- updated to version 1.0.1
- fixed memory leak in QoreXmlDoc::getString()
- fixed typo in error message
- fixed thread safety issues in key operations
- added comprehensive test coverage
- updated documentation with examples

* Thu Nov 25 2021 David Nichols <david@qore.org> 1.0.0
- updated to version 1.0.0
- fixed build with newer libxmlsec1 builds
- fixed tests

* Tue Sep 2 2008 David Nichols <david_nichols@users.sourceforge.net>
- initial spec file for separate xmlsec release
