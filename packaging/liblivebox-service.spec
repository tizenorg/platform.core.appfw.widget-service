Name: liblivebox-service
Summary: Library for the development of a livebox service
Version: 0.0.3
Release: 1
Group: main/app
License: Samsung Proprietary License
Source0: %{name}-%{version}.tar.gz
BuildRequires: cmake, gettext-tools
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(gio-2.0)
BuildRequires: pkgconfig(com-core)
BuildRequires: pkgconfig(sqlite3)
BuildRequires: pkgconfig(db-util)

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

%post

%files -n liblivebox-service
%manifest liblivebox-service.manifest
%defattr(-,root,root,-)
/usr/lib/*.so*

%files devel
%defattr(-,root,root,-)
/usr/include/livebox-service/livebox-service.h
/usr/share/doc/livebox-service/livebox-service_PG.h
/usr/lib/pkgconfig/*.pc
