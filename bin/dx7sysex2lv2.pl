#!/usr/bin/perl
#
# dx7sysex2lv2 - SysEx patchset to LV2 converter for
# DeeAxe7 - LV2 plugin
#
# (c) by H. Wirtz <dcoredump@googlemail.com>
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software Foundation,
# Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
#
# Hints:
# https://github.com/rogerallen/dxsyx/blob/master/dx7-sysex-format.txt
#
# $ hexdump -v -s6 -e '/1  "%_ad#    "' -e '/1    "%02X hex"' -e '/1 " | %03i dec"' -e '/1 " | %03o oct"' -e '/1 " | _%c\_\n"' rom1A.syx |less
#

use Data::Dumper;

$DXSYX="/usr/local/bin/dxsyx";

$LV2_BUNDLE_NAME="DeeAxe7.lv2";
$LV2_URI="https://github.com/dcoredump/".$LV2_BUNDLE_NAME;
$LV2_INSTALL_PATH=$ENV{'HOME'}."/.lv2";

$SYSEX=$ARGV[0];
$BANK=$SYSEX;
$BANK=~s{.*/}{};      # removes path  
$BANK=~s{\.[^.]+$}{}; # removes extension

$_LV2_BUNDLE_NAME="DeeAxe7_lv2";

open(SYSEX,"$DXSYX -y $SYSEX |") || die ("Cannot open \'$DXSYX -y $SYSEX\': $!");
while($s=<SYSEX>)
{
	chop($s);
	next if($s=~/^\s*$/);
	next if($s=~/^\s*---\s*$/);
	next if($s=~/^\s*filename/);
	if($s=~/^\s+voice_name:\s+(.+)\s*/)
	{
		my($voice)=$1;
		$voice=~s/\s+$//; # right trim
		my($voice_name)=$voice;
        	$voice=~tr/ /_/;
        	$voice=~tr/-/_/;

		if($deeaxe_voice)
		{
			print $deeaxe_voice "        ] .\n";
			close($deeaxe_voice);
		}

		my($path)=$LV2_INSTALL_PATH."/".$_LV2_BUNDLE_NAME."-".$BANK."_".$voice.".lv2";
		mkdir($path) || die("Cannot create dir \'$path\':$!\n");
		write_manifest($path,$voice);

		open($deeaxe_voice,">$path/$voice.ttl") || die ("Cannot open \'$$path/$voice.ttl\': $!");
		write_preset_header($voice,$voice_name);
		next;
	}
	else
	{
		print $deeaxe_voice "        ] , [\n";
	}
	if($s=~/^\s+(.+):\s*(\d+)/)
	{
		print_lv2_port($1,$2);
	}
}
close($deeaxe_voice) if ($deeaxe_voice);
close(SYSEX);

sub write_preset_header
{
	($voice,$voice_name)=@_;
        print $deeaxe_voice "\@prefix atom: <http://lv2plug.in/ns/ext/atom#> .\n";
        print $deeaxe_voice "\@prefix lv2: <http://lv2plug.in/ns/lv2core#> .\n";
        print $deeaxe_voice "\@prefix pset: <http://lv2plug.in/ns/ext/presets#> .\n";
        print $deeaxe_voice "\@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n";
        print $deeaxe_voice "\@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .\n";
        print $deeaxe_voice "\@prefix state: <http://lv2plug.in/ns/ext/state#> .\n";
        print $deeaxe_voice "\@prefix xsd: <http://www.w3.org/2001/XMLSchema#> .\n";
        print $deeaxe_voice "\n";
        #print $deeaxe_voice "<".$LV2_URI."#".$BANK."_".$voice.">\n";
        print $deeaxe_voice "<>\n";
        print $deeaxe_voice "        a pset:Preset ;\n";
        print $deeaxe_voice "        lv2:appliesTo <".$LV2_URI."> ;\n";
        print $deeaxe_voice "        rdfs:label \"".$voice_name."\" ;\n";
        print $deeaxe_voice "        lv2:port [\n";
        print_lv2_port("cutoff","0.0");
	print $deeaxe_voice "        ] , [\n";
        print_lv2_port("resonance","0.0");
	print $deeaxe_voice "        ] , [\n";
        print_lv2_port("output","1.0");
}

sub write_manifest
{
	($path,$voice)=@_;

        open(MANIFEST,">$path/manifest.ttl")||die("Cannot open \'$path/manifest.ttl\': $!\n");
        print MANIFEST "\@prefix atom: <http://lv2plug.in/ns/ext/atom#> .\n";
        print MANIFEST "\@prefix lv2: <http://lv2plug.in/ns/lv2core#> .\n";
        print MANIFEST "\@prefix pset: <http://lv2plug.in/ns/ext/presets#> .\n";
        print MANIFEST "\@prefix rdf: <http://www.w3.org/1999/02/22-rdf-syntax-ns#> .\n";
        print MANIFEST "\@prefix rdfs: <http://www.w3.org/2000/01/rdf-schema#> .\n";
        print MANIFEST "\@prefix state: <http://lv2plug.in/ns/ext/state#> .\n";
        print MANIFEST "\@prefix xsd: <http://www.w3.org/2001/XMLSchema#> .\n";

        print MANIFEST "\n";
        #print MANIFEST "<".$LV2_URI."#".$BANK."_".$voice.">\n";
        print MANIFEST "<".$BANK."_".$voice.".ttl>\n";
        print MANIFEST "        lv2:appliesTo <".$LV2_URI."> ;\n";
        #print MANIFEST "        a pset:Bank $BANK ;\n";
        print MANIFEST "        a pset:Preset ;\n";
        print MANIFEST "        rdfs:seeAlso <".$BANK."_".$voice.".ttl> .\n";
        close(MANIFEST);
}

sub print_lv2_port
{
        ($port_name,$port_val)=@_;

        print $deeaxe_voice "                lv2:symbol \"".$port_name."\" ;\n";
	$port_val.=".0" if($port_val!~/\./);
        print $deeaxe_voice "                pset:value ".$port_val."\n";
}

