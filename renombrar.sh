#!/bin/bash
for f in libros/*.txt; do
    id=$(basename "$f" .txt)

    # Saca el título del encabezado y le quita caracteres que no sirven en nombres
    titulo=$(head -n 60 "$f" | grep -m1 "^Title:" | sed 's/^Title: *//' \
             | tr -d '\r' | sed 's/[\/:*?"<>|]//g' | tr ' ' '_')
    titulo=${titulo:0:80}

    # Si no tiene título, se queda con el número
    [ -z "$titulo" ] && continue

    # Si ya hay otro libro con ese título, le agrega el número
    nuevo="libros/$titulo.txt"
    [ -e "$nuevo" ] && nuevo="libros/${titulo}_$id.txt"
    mv -n "$f" "$nuevo"
done
