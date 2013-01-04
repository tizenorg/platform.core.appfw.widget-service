Name: liblivebox-service
Summary: Library for the development of a livebox service
Version: 0.2.10
Release: 1
Group: main/app
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

%description
Livebox service library

%package devel
Summary: Files for livebox service.
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}

%description devel
Livebox service library (dev)

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
