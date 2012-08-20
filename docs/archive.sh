#!/bin/bash
# Genère les .zip et .tar.gz pour la rubrique téléchargement

short=`git describe --abbrev=0`
version=`git describe`
directory=`pwd`/build/html/files

# Dossier de sortie
mkdir -p $directory

# Remonte au dossier racine
cd ../

# Génère les zip et tgz du SDK
sdk_zip=RhobanSDK-$version.zip
if [ ! -f $directory/$sdk_zip ]; then
    rm $directory/$sdk_zip
    zip -r "$directory/$sdk_zip" sdk/ -x *.git*
fi

sdk_tgz=RhobanSDK-$version.tar.gz
if [ ! -f $directory/$sdk_tgz ]; then
    rm $directory/$sdk_tgz
    tar zcvf "$directory/$sdk_tgz" --exclude "*/.git" sdk/
fi

# Retour dans le dossier docs/
cd docs/

sdk_url_zip="files\\/$sdk_zip"
sdk_url_tgz="files\\/$sdk_tgz"

cat source/_build/telechargement.in.rst \
    |sed -s s/"%SDK_VERSION%"/"$version"/g \
    |sed -s s/"%SDK_ZIP%"/"$sdk_url_zip"/g \
    |sed -s s/"%SDK_TGZ%"/"$sdk_url_tgz"/g \
    > source/telechargements.rst

echo -e "release = '$short'\nversion = '$version'" > source/generated.py
