Name: liblivebox-service
Summary: Service API for gathering installed livebox information.
Version: 0.3.6
Release: 1
Group: framework/livebox
License: Flora License
Source0: %{name}-%{version}.tar.gz
BuildRequires: cmake, gettext-tools, coreutils
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(gio-2.0)
BuildRequires: pkgconfig(com-core)
BuildRequires: pkgconfig(sqlite3)
BuildRequires: pkgconfig(db-util)
BuildRequires: pkgconfig(pkgmgr)
BuildRequires: pkgconfig(x11)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(ail)

%description
Service API for gathering information of installed liveboxes

%package devel
Summary: Files for livebox service.
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}

%description devel
Gathering the installed livebox information.

%prep
%setup -q

%build
cmake . -DCMAKE_INSTALL_PREFIX=%{_prefix}
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install
mkdir -p %{buildroot}/%{_datarootdir}/license

%post

%files -n liblivebox-service
%manifest liblivebox-service.manifest
%defattr(-,root,root,-)
%{_libdir}/*.so*
%{_datarootdir}/license/*

%files devel
%defattr(-,root,root,-)
%{_includedir}/livebox-service/livebox-service.h
%{_datarootdir}/doc/livebox-service/livebox-service_PG.h
%{_libdir}/pkgconfig/*.pc

# End of a file
