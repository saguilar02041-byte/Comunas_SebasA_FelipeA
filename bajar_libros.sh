#!/bin/bash
mkdir -p tmp libros

# Baja solo los .txt del espejo oficial de Gutenberg
rsync -a --include="*/" --exclude="*-8.txt" --include="*.txt" --exclude="*" \
      aleph.gutenberg.org::gutenberg tmp/ &
PID=$!

# Revisa el tamaño cada 10 segundos y corta al llegar a 4900 MB
while kill -0 $PID 2>/dev/null && [ $(du -sm tmp | cut -f1) -lt 4900 ]; do
    sleep 10
done
kill $PID 2>/dev/null

# Pasa todos los .txt a la carpeta libros y borra lo temporal
find tmp -name "*.txt" -exec mv -n {} libros/ \;
rm -r tmp
du -sh libros
