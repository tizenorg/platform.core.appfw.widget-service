Name: libwidget_service
Summary: Service API for gathering installed widget information
Version: 1.2.0
Release: 1
Group: Applications/Core Applications
License: Flora-1.1, MIT
Source0: %{name}-%{version}.tar.gz
Source1001: %{name}.manifest
BuildRequires: cmake, gettext-tools, coreutils
BuildRequires: pkgconfig(dlog)
BuildRequires: pkgconfig(glib-2.0)
BuildRequires: pkgconfig(gio-2.0)
BuildRequires: pkgconfig(sqlite3)
BuildRequires: pkgconfig(db-util)
BuildRequires: pkgconfig(pkgmgr)
BuildRequires: pkgconfig(pkgmgr-info)
BuildRequires: pkgconfig(uuid)
BuildRequires: pkgconfig(vconf)
BuildRequires: pkgconfig(icu-uc)
BuildRequires: pkgconfig(bundle)
BuildRequires: pkgconfig(capi-appfw-app-common)
BuildRequires: pkgconfig(capi-base-common)
BuildRequires: pkgconfig(capi-system-info)
BuildRequires: pkgconfig(libtzplatform-config)
BuildRequires: pkgconfig(libxml-2.0)
BuildRequires: pkgconfig(aul)
BuildRequires: pkgconfig(cynara-client)

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
MAJORVER=`echo %{version} | awk 'BEGIN {FS="."}{print $1}'`
%cmake . -DFULLVER=%{version} -DMAJORVER=${MAJORVER}
make %{?jobs:-j%jobs}

%install
rm -rf %{buildroot}
%make_install

%post -n %{name}
/sbin/ldconfig

%postun -n %{name}
/sbin/ldconfig

%files -n %{name}
%manifest %{name}.manifest
%defattr(-,root,root,-)
%license LICENSE
%{_libdir}/libwidget_service.so*
%{_sysconfdir}/package-manager/parserlib/libwidget-application.so
%{_bindir}/widget_test

%files devel
%manifest %{name}.manifest
%defattr(-,root,root,-)
%{_includedir}/widget_service/*
%{_libdir}/pkgconfig/widget_service.pc

# End of a file
