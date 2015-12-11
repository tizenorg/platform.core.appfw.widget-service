%bcond_with wayland

Name: libwidget_service
Summary: Service API for gathering installed widget information
Version: 1.2.0
Release: 1
Group: Applications/Core Applications
License: Flora-1.1, MIT
Source0: %{name}-%{version}.tar.gz
Source1001: %{name}.manifest
Source2001: wayland-drm.xml
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
BuildRequires: pkgconfig(capi-base-common)
BuildRequires: pkgconfig(libdrm)
BuildRequires: pkgconfig(capi-system-info)
BuildRequires: pkgconfig(libtzplatform-config)
BuildRequires: pkgconfig(libxml-2.0)

%if %{with wayland}
BuildRequires: pkgconfig(wayland-client)
%else
BuildRequires: pkgconfig(x11)
BuildRequires: pkgconfig(libdri2)
BuildRequires: pkgconfig(dri2proto)
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
%if %{with wayland}
mkdir -p wayland-drm/include
mkdir -p wayland-drm/src
wayland-scanner client-header < %{SOURCE2001} > wayland-drm/include/widget_wayland-drm-client-protocol.h
wayland-scanner server-header < %{SOURCE2001} > wayland-drm/include/widget_wayland-drm-server-protocol.h
wayland-scanner code < %{SOURCE2001} > wayland-drm/src/wayland-drm.c
%endif

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

sqlite3 .widget.db < ./parser/widget.sql

%cmake . -DWAYLAND_SUPPORT=${WAYLAND_SUPPORT} -DX11_SUPPORT=${X11_SUPPORT}
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install
mkdir -p %{buildroot}%{_datarootdir}/license
mkdir -p %{buildroot}%{TZ_SYS_DB}
mkdir -p %{buildroot}%{_sysconfdir}/skel/.applications/dbspace

install -m 0644 .widget.db %{buildroot}%{TZ_SYS_DB}
install -m 0644 .widget.db %{buildroot}%{_sysconfdir}/skel/.applications/dbspace

%post -n %{name}
/sbin/ldconfig
chsmack -a "User::Home" %{TZ_SYS_DB}/.widget.db
chsmack -a "User::Home" %{_sysconfdir}/skel/.applications/dbspace/.widget.db

%postun -n %{name}
/sbin/ldconfig

%files -n %{name}
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_libdir}/libwidget_service.so*
%{_datarootdir}/license/libwidget_service
%{_sysconfdir}/package-manager/parserlib/libwidget-application.so
%{_sysconfdir}/skel/.applications/dbspace/.widget.db
%{TZ_SYS_DB}/.widget.db

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
%if %{with wayland}
%{_includedir}/widget_service/widget_wayland-drm-client-protocol.h
%{_includedir}/widget_service/widget_wayland-drm-server-protocol.h
%endif
%{_libdir}/pkgconfig/widget_service.pc

# End of a file
