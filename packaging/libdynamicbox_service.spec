%bcond_with wayland

Name: libdynamicbox_service
Summary: Service API for gathering installed dynamicbox information
Version: 1.0.0
Release: 1
Group: HomeTF/DynamicBox
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
BuildRequires: model-build-features

%if %{with wayland}
# Nothing provides
%else
BuildRequires: pkgconfig(x11)
%endif

%if "%{model_build_feature_livebox}" == "0"
ExclusiveArch:
%endif

%description
Service API for gathering information of installed dynamicboxes

%package devel
Summary: Files for dynamicbox service
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}

%description devel
Gathering the installed dynamicbox information.

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

%post -n %{name} -p /sbin/ldconfig
%postun -n %{name} -p /sbin/ldconfig

%files -n %{name}
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/libdynamicbox_service.so*
%{_datarootdir}/license/libdynamicbox_service

%files devel
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_includedir}/dynamicbox_service/dynamicbox_service.h
%{_includedir}/dynamicbox_service/dynamicbox_errno.h
%{_includedir}/dynamicbox_service/dynamicbox_cmd_list.h
%{_includedir}/dynamicbox_service/dynamicbox_buffer.h
%{_includedir}/dynamicbox_service/dynamicbox_script.h
%{_includedir}/dynamicbox_service/dynamicbox_conf.h
%{_libdir}/pkgconfig/dynamicbox_service.pc

#################################################
# liblivebox-service (for old version)
%package -n liblivebox-service
Summary: Library for developing the dynamicbox app provider (old version)
Group: HomeTF/Dynamicbox
License: Flora
Requires: %{name}

%description -n liblivebox-service
Provider APIs to develop the dynamicbox provider applications. (old version)

%package -n liblivebox-service-devel
Summary: Header & package configuration files to support development of the dynamicbox provider applications. (old version)
Group: Development/Libraries
Requires: liblivebox-service

%description -n liblivebox-service-devel
Dynamicbox provider application development library (dev) (old version)

%files -n liblivebox-service
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/liblivebox-service.so*
%{_datarootdir}/license/liblivebox-service

%files -n liblivebox-service-devel
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_includedir}/livebox-service/livebox-service.h
%{_includedir}/livebox-service/livebox-errno.h
%{_includedir}/livebox-service/livebox-service_product.h
%{_libdir}/pkgconfig/livebox-service.pc

# End of a file
