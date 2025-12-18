Name:           linux-wifi-hotspot
Version:        4.7.2
Release:        1%{?dist}
Summary:        GUI tool for creating virtual WiFi hotspots

License:        BSD
URL:            https://github.com/lakinduakash/linux-wifi-hotspot
Source0:        %{name}-%{version}.tar.gz

# Don't generate debug packages
%global debug_package %{nil}

BuildRequires:  gcc
BuildRequires:  gcc-c++
BuildRequires:  make
BuildRequires:  pkgconfig
BuildRequires:  gtk3-devel
BuildRequires:  libpng-devel
BuildRequires:  qrencode-devel
BuildRequires:  glib2-devel

Requires:       hostapd >= 2.0
Requires:       iw >= 5.4
Requires:       iproute >= 5.0
Requires:       util-linux >= 2.3
Requires:       procps-ng >= 3.3
Requires:       qrencode >= 4.0
Requires:       libpng >= 1.6
Requires:       dnsmasq
Requires:       iptables

%description
GUI tool for creating virtual hotspots using the same WiFi card that is
already connected to an access point. Features both command line and GUI
interfaces. Supports sharing WiFi connections, MAC filtering, QR code
generation, and more.

%prep
%setup -q

%build
make %{?_smp_mflags}

%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=%{buildroot}

%files
%license LICENSE
%doc README.md
/usr/bin/wihotspot-gui
/usr/bin/wihotspot
/usr/bin/create_ap
/usr/share/applications/wihotspot.desktop
/usr/share/pixmaps/wihotspot.png
/usr/share/icons/hicolor/*/apps/wihotspot.*
/usr/share/bash-completion/completions/create_ap
/usr/share/polkit-1/rules.d/90-org.opensuse.policykit.wihotspot.rules
/usr/share/polkit-1/actions/org.opensuse.policykit.wihotspot.policy
%config(noreplace) /etc/create_ap.conf
/usr/lib/systemd/system/create_ap.service
/usr/share/doc/create_ap/README.md

%post
# Enable firewall DHCP service for hotspot functionality
if command -v firewall-cmd >/dev/null 2>&1; then
    if systemctl is-active --quiet firewalld; then
        firewall-cmd --add-service=dhcp --permanent >/dev/null 2>&1 || true
        firewall-cmd --reload >/dev/null 2>&1 || true
    fi
fi

# Set SELinux to permissive for dnsmasq
if command -v semanage >/dev/null 2>&1; then
    semanage permissive -a dnsmasq_t >/dev/null 2>&1 || true
fi

%postun
# Only remove on uninstall, not upgrade
if [ $1 -eq 0 ]; then
    # Remove firewall DHCP service
    if command -v firewall-cmd >/dev/null 2>&1; then
        if systemctl is-active --quiet firewalld; then
            firewall-cmd --remove-service=dhcp --permanent >/dev/null 2>&1 || true
            firewall-cmd --reload >/dev/null 2>&1 || true
        fi
    fi
fi

%changelog
* Tue Dec 17 2025 Weydson Lima <weyseal@gmail.com> - 4.7.2-1
- Added Fedora packaging support
- Added automatic firewall DHCP configuration
- Fixed DHCP issues on Fedora with firewalld

* Mon Jul 08 2024 Lakindu Akash <lakinduakash@gmail.com> - 4.7.2-0
- Update to 4.7.2
- Add option to log DNS requests to a file with dnsmasq
- Add support of WPA3 Transition Mode
- Fix typo for tv_mac_filter
- Set program name to fix wayland icon issue
