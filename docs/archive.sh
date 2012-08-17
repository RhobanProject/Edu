#!/bin/bash
# Genere les .zip et .tar.gz pour la rubrique téléchargement

version=`git log -n 1 --pretty=oneline|cut -c1-6`
directory=`pwd`/build/html/files

# Output directory
mkdir -p $directory
