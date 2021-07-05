# This file is part of Signus: The Artefact Wars
# https://github.com/signus-game
#
# Copyright (C) 2021 Martin Doucha <doucha@swarmtech.cz>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 2 as
# published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

%define        ver_app    1.96.0
%define        ver_data   1.96

Name:          signus
Version:       %{ver_app}
Release:       0
Source0:       %{name}-%{ver_app}.tar.bz2

Summary:       Turn-based strategy game
License:       GPL-2.0-only
Group:         Amusements/Games/Strategy/Turn Based
URL:           https://github.com/signus-game

BuildRequires: SDL2-devel
BuildRequires: SDL2_image-devel
BuildRequires: SDL2_mixer-devel
BuildRequires: SDL2_ttf-devel
BuildRequires: libvorbis-devel
BuildRequires: libjpeg-devel

Requires:      signus-data >= %{ver_data}

%description
Turn-based strategy wargame Signus: The Artefact Wars


%prep
%setup -q

%build
%configure
%make_build

%install
%make_install

%files
%doc AUTHORS
%license COPYING
%dir %{_datadir}/signus
%dir %{_datadir}/signus/%{ver_data}
%{_bindir}/*
%{_datadir}/signus/%{ver_data}/default_signus.ini
