
%define        ver_app    1.96.0
%define        ver_data   1.96.0

Name:          signus
Version:       %{ver_app}
Release:       1
Source0:       %{name}-%{ver_app}.tar.bz2
Source1:       %{name}-data-%{ver_data}.tar.bz2

Prefix:        /usr
Summary:       Strategy game Signus: The Artefact Wars
License:       GPL
Group:         Games/Strategy
URL:           http://signus.sourceforge.net
Packager:      Vaclav Slavik <vslavik@fastmail.fm>
BuildRoot:     /var/tmp/%{name}-%{version}-root

Requires:      SDL
BuildRequires: SDL-devel
Requires:      SDL_mixer >= 1.2.5

Requires:      signus-data = %{ver_data}
Requires:      signus-data-l10n = %{ver_data}

%description
TODO TODO FIXME FIXME TODO TODO


%package data
Summary:       Data files for Signus
Group:         Games/Strategy
Requires:      signus

%description data
Data files for Signus: The Artefact Wars


%package data-l10n-cs
Summary:       Czech data for Signus
Group:         Games/Strategy
Requires:      signus
Provides:      signus-l10n = %{ver_data}

%description data-l10n-cs
Czech data files for Signus: The Artefact Wars


%package data-l10n-en
Summary:       English data for Signus
Group:         Games/Strategy
Requires:      signus
Provides:      signus-l10n = %{ver_data}

%description data-l10n-en
English data files for Signus: The Artefact Wars


%prep
%setup -q -a1

%build
CFLAGS="%{optflags}" CXXFLAGS="%{optflags}" ./configure --prefix=%{prefix}
%make

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT

%clean
rm -rf $RPM_BUILD_ROOT

%files
%defattr(-,root,root)
%doc NEWS README TODO AUTHORS COPYING
%{_bindir}/*

%files data
%defattr(-,root,root)
%dir %{_datadir}/signus
%{_datadir}/signus/fonts.dat
%{_datadir}/signus/graphics.dat
%{_datadir}/signus/missions.dat
%{_datadir}/signus/music
%{_datadir}/signus/screens
%{_datadir}/signus/sfx
%{_datadir}/signus/unitstd.idx
%{_datadir}/signus/version-data

%files data-l10n-cs
%defattr(-,root,root)
%{_datadir}/signus/graphics-cs.dat
%{_datadir}/signus/texts-cs.dat
%dir %{_datadir}/signus/screens-cs
%{_datadir}/signus/screens-cs/*
%dir %{_datadir}/signus/speech-cs
%{_datadir}/signus/speech-cs/*

%files data-l10n-en
%defattr(-,root,root)
%{_datadir}/signus/graphics-en.dat
%{_datadir}/signus/texts-en.dat
%dir %{_datadir}/signus/screens-en
%{_datadir}/signus/screens-en/*
%dir %{_datadir}/signus/speech-en
%{_datadir}/signus/speech-en/*


