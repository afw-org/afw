%global __os_install_post %{nil}

Name:           afw
Version:        %{version}
Release:        %{release}
Summary:        Adaptive Framework
License:        MIT
URL:            https://github.com/afw-org/afw.git
Source0:        afw-%{version}-%{release}.tar.gz

%description
Adaptive Framework is a set of interfaces for developing servers, requests,
object access, etc. in an adaptive way, along with implementations,
language bindings, commands, and administrative applications.

%define _python_bytecompile_errors_terminate_build 0

%prep
%setup -q
%build
%install
mkdir -p %{buildroot}/
cp -r ./* %buildroot/

%files
/*
%changelog