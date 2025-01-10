#!/bin/bash

echo "Instalador de recursos..."
echo "Actualizando dispositivo"
apt-get update && apt-get upgrade -y
echo "Instalando librerias"
apt-get install clang build-essential -y 
