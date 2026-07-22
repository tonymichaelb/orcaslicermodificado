#!/usr/bin/env bash

# ============================================
# OrcaSlicer MeuPrinter Build Script
# ============================================
# Compila o OrcaSlicer com suporte a "Mistura de Cores"
# Para: macOS ARM64 (Apple Silicon)
# Status: Pronto para compilação

set -e

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}========================================${NC}"
echo -e "${GREEN}OrcaSlicer MeuPrinter Build${NC}"
echo -e "${GREEN}========================================${NC}"

# Detectar arquitetura
ARCH=$(uname -m)
if [ "$ARCH" != "arm64" ]; then
    echo -e "${YELLOW}Aviso: Arquitetura detectada: $ARCH (esperado: arm64)${NC}"
fi

BUILD_DIR="build/arm64"
CONFIG_TYPE="RelWithDebInfo"

echo -e "\n${GREEN}Etapa 1: Limpando build anterior...${NC}"
if [ -d "$BUILD_DIR" ]; then
    rm -rf "$BUILD_DIR"
    echo -e "${GREEN}✓ Diretório de build removido${NC}"
fi

echo -e "\n${GREEN}Etapa 2: Criando diretório de build...${NC}"
mkdir -p "$BUILD_DIR"
echo -e "${GREEN}✓ Diretório criado: $BUILD_DIR${NC}"

echo -e "\n${GREEN}Etapa 3: Executando CMake...${NC}"
cd "$BUILD_DIR"
cmake \
    -DCMAKE_BUILD_TYPE=$CONFIG_TYPE \
    -DCMAKE_OSX_ARCHITECTURES=arm64 \
    -DCMAKE_OSX_DEPLOYMENT_TARGET=11.0 \
    ../..

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Erro ao executar CMake${NC}"
    exit 1
fi
echo -e "${GREEN}✓ CMake configurado com sucesso${NC}"

echo -e "\n${GREEN}Etapa 4: Compilando...${NC}"
echo -e "${YELLOW}Nota: Isso pode levar 10-30 minutos${NC}"

cmake --build . --config $CONFIG_TYPE --target all -- -j$(sysctl -n hw.ncpu)

if [ $? -ne 0 ]; then
    echo -e "${RED}✗ Erro durante a compilação${NC}"
    exit 1
fi

echo -e "\n${GREEN}✓ Compilação concluída com sucesso!${NC}"

# Verificar executável
if [ -f "src/OrcaSlicer.app/Contents/MacOS/OrcaSlicer" ]; then
    echo -e "${GREEN}✓ Executável criado: src/OrcaSlicer.app/Contents/MacOS/OrcaSlicer${NC}"
else
    echo -e "${RED}✗ Executável não encontrado!${NC}"
    exit 1
fi

echo -e "\n${GREEN}========================================${NC}"
echo -e "${GREEN}Compilação Concluída!${NC}"
echo -e "${GREEN}========================================${NC}"

echo -e "\n${YELLOW}Novos Componentes Compilados:${NC}"
echo "  - ColorMixing.hpp / ColorMixing.cpp"
echo "  - Interface gráfica 'Mistura de Cores'"
echo ""
echo -e "${YELLOW}Próximas Etapas:${NC}"
echo "  1. Execute: ./src/OrcaSlicer.app/Contents/MacOS/OrcaSlicer"
echo "  2. Procure pela aba 'Mistura de Cores' (Color Mixing)"
echo "  3. Teste as novas configurações"
echo ""
echo -e "${YELLOW}Arquivos de Configuração Adicionados:${NC}"
echo "  - resources/profiles/Custom/machine/MeuPrinter.json"
echo "  - resources/profiles/Custom/filament/Generic PLA @MeuPrinter.json"
echo "  - resources/profiles/Custom/process/0.20mm Standard @MeuPrinter.json"
echo "  - resources/profiles/Custom/color_mixing/PLA_MultiColor_Mix.json"
echo ""
echo -e "${GREEN}Para mais informações, veja: MEUPRINTER_COLOR_MIXING_GUIDE.md${NC}"
