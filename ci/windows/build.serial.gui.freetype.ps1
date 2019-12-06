# Dissolve Windows Build
# One of a sequence of scripts.
# - Build Freetype

$ErrorActionPreference = 'Stop'

# Retrieve / compile Freetype 2.10.1
cd $HOME
wget 'https://download.savannah.gnu.org/releases/freetype/ft2101.zip' -o 'ft.zip'
unzip ./ft.zip
Echo 'Compiling FreeType...'
mkdir freetype-build
cd freetype-build
cmake -G "Ninja" ..\freetype-2.10.1
ninja

# Set environment
$env:INCLUDE += "$HOME\\freetype-2.10.1;"
$env:LIB += "$HOME\\freetype-build;"
