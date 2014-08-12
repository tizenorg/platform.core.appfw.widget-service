%bcond_with wayland

Name: liblivebox-service
Summary: Service API for gathering installed livebox information
Version: 0.11.4
Release: 1
Group: HomeTF/Livebox
License: Flora
Source0: %{name}-%{version}.tar.gz
Source1001: %{name}.manifest
BuildRequires: cmake, gettext-tools, coreutils
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(gio-2.0)
BuildRequires: pkgconfig(com-core)
BuildRequires: pkgconfig(sqlite3)
BuildRequires: pkgconfig(db-util)
BuildRequires: pkgconfig(pkgmgr)
BuildRequires: pkgconfig(pkgmgr-info)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(ail)
BuildRequires: pkgconfig(icu-uc)

%if %{with wayland}
# Nothing provides
%else
BuildRequires: pkgconfig(x11)
%endif

%if "%{sec_product_feature_livebox}" == "0"
ExclusiveArch:
%endif

%description
Service API for gathering information of installed liveboxes

%package devel
Summary: Files for livebox service
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}

%description devel
Gathering the installed livebox information.

%prep
%setup -q
cp %{SOURCE1001} .

%build
%if 0%{?sec_build_binary_debug_enable}
export CFLAGS="$CFLAGS -DTIZEN_DEBUG_ENABLE"
export CXXFLAGS="$CXXFLAGS -DTIZEN_DEBUG_ENABLE"
export FFLAGS="$FFLAGS -DTIZEN_DEBUG_ENABLE"
%endif

%if 0%{?tizen_build_binary_release_type_eng}
export CFLAGS="${CFLAGS} -DTIZEN_ENGINEER_MODE"
export CXXFLAGS="${CXXFLAGS} -DTIZEN_ENGINEER_MODE"
export FFLAGS="${FFLAGS} -DTIZEN_ENGINEER_MODE"
%endif

%if %{with wayland}
export WAYLAND_SUPPORT=On
export X11_SUPPORT=Off
%else
export WAYLAND_SUPPORT=Off
export X11_SUPPORT=On
%endif

%cmake . -DWAYLAND_SUPPORT=${WAYLAND_SUPPORT} -DX11_SUPPORT=${X11_SUPPORT}
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install
mkdir -p %{buildroot}/%{_datarootdir}/license

%post -n liblivebox-service -p /sbin/ldconfig
%postun -n liblivebox-service -p /sbin/ldconfig

%files -n liblivebox-service
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/*.so*
%{_datarootdir}/license/*

%files devel
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_includedir}/livebox-service/livebox-service.h
%{_includedir}/livebox-service/livebox-service_product.h
%{_includedir}/livebox-service/livebox-errno.h
%{_libdir}/pkgconfig/*.pc
%{_includedir}/dynamicbox_service/dynamicbox_service.h
%{_includedir}/dynamicbox_service/dynamicbox_errno.h

# End of a file
