%bcond_with wayland

Name: libwidget_service
Summary: Service API for gathering installed widget information
Version: 1.1.2
Release: 1
Group: HomeTF/widget
License: Flora License Version 1.1
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
BuildRequires: pkgconfig(icu-uc)
BuildRequires: pkgconfig(bundle)
BuildRequires: model-build-features

%if %{with wayland}
# Nothing provides
%else
BuildRequires: pkgconfig(x11)
%endif

%if "%{model_build_feature_widget}" == "0"
ExclusiveArch:
%endif

%description
Service API for gathering information of installed widgetes

%package devel
Summary: Files for widget service
Group: Development/Libraries
Requires: %{name} = %{version}-%{release}

%description devel
Gathering the installed widget information.

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
%{_libdir}/libwidget_service.so*
%{_datarootdir}/license/libwidget_service

%files devel
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_includedir}/widget_service/widget_service.h
%{_includedir}/widget_service/widget_service_internal.h
%{_includedir}/widget_service/widget_errno.h
%{_includedir}/widget_service/widget_cmd_list.h
%{_includedir}/widget_service/widget_buffer.h
%{_includedir}/widget_service/widget_script.h
%{_includedir}/widget_service/widget_conf.h
%{_includedir}/widget_service/widget_abi.h
%{_includedir}/widget_service/widget_util.h
%{_libdir}/pkgconfig/widget_service.pc

# End of a file
