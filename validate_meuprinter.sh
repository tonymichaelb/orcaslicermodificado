#!/usr/bin/env bash

# ============================================
# OrcaSlicer MeuPrinter - Validation Script
# ============================================
# Verifica se todos os arquivos foram criados corretamente

set -e

# Cores para output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}╔════════════════════════════════════════╗${NC}"
echo -e "${BLUE}║  MeuPrinter Validation Script         ║${NC}"
echo -e "${BLUE}╚════════════════════════════════════════╝${NC}"

ERRORS=0
WARNINGS=0
SUCCESS=0

check_file() {
    local file=$1
    local type=$2
    
    if [ -f "$file" ]; then
        echo -e "${GREEN}✓${NC} $type: $(basename $file)"
        ((SUCCESS++))
    else
        echo -e "${RED}✗${NC} $type MISSING: $file"
        ((ERRORS++))
    fi
}

check_directory() {
    local dir=$1
    local name=$2
    
    if [ -d "$dir" ]; then
        echo -e "${GREEN}✓${NC} Diretório: $name"
        ((SUCCESS++))
    else
        echo -e "${RED}✗${NC} Diretório MISSING: $name ($dir)"
        ((ERRORS++))
    fi
}

validate_json() {
    local file=$1
    local name=$2
    
    if [ -f "$file" ]; then
        if python3 -m json.tool "$file" > /dev/null 2>&1; then
            echo -e "${GREEN}✓${NC} JSON válido: $name"
            ((SUCCESS++))
        else
            echo -e "${YELLOW}⚠${NC} JSON inválido: $name"
            ((WARNINGS++))
        fi
    else
        echo -e "${RED}✗${NC} Arquivo não encontrado: $file"
        ((ERRORS++))
    fi
}

echo ""
echo -e "${BLUE}1. Verificando Componentes GUI${NC}"
echo "──────────────────────────────"
check_file "src/slic3r/GUI/ColorMixing.hpp" "Cabeçalho"
check_file "src/slic3r/GUI/ColorMixing.cpp" "Implementação"
check_file "src/slic3r/GUI/ColorMixing_Integration.md" "Documentação técnica"

echo ""
echo -e "${BLUE}2. Verificando Diretórios de Perfil${NC}"
echo "──────────────────────────────"
check_directory "resources/profiles/Custom/machine" "Máquinas"
check_directory "resources/profiles/Custom/filament" "Filamentos"
check_directory "resources/profiles/Custom/process" "Processos"
check_directory "resources/profiles/Custom/color_mixing" "Mistura de Cores"

echo ""
echo -e "${BLUE}3. Verificando Máquina MeuPrinter${NC}"
echo "──────────────────────────────"
validate_json "resources/profiles/Custom/machine/MeuPrinter.json" "Modelo base"
validate_json "resources/profiles/Custom/machine/MeuPrinter 0.2 nozzle.json" "Bico 0.2mm"
validate_json "resources/profiles/Custom/machine/MeuPrinter 0.4 nozzle.json" "Bico 0.4mm"
validate_json "resources/profiles/Custom/machine/MeuPrinter 0.6 nozzle.json" "Bico 0.6mm"
validate_json "resources/profiles/Custom/machine/MeuPrinter 0.8 nozzle.json" "Bico 0.8mm"

echo ""
echo -e "${BLUE}4. Verificando Filamentos${NC}"
echo "──────────────────────────────"
validate_json "resources/profiles/Custom/filament/Generic PLA @MeuPrinter.json" "PLA"
validate_json "resources/profiles/Custom/filament/Generic PETG @MeuPrinter.json" "PETG"
validate_json "resources/profiles/Custom/filament/Generic ABS @MeuPrinter.json" "ABS"

echo ""
echo -e "${BLUE}5. Verificando Processos${NC}"
echo "──────────────────────────────"
validate_json "resources/profiles/Custom/process/0.12mm Fine @MeuPrinter.json" "Fine"
validate_json "resources/profiles/Custom/process/0.20mm Standard @MeuPrinter.json" "Standard"
validate_json "resources/profiles/Custom/process/0.28mm Draft @MeuPrinter.json" "Draft"

echo ""
echo -e "${BLUE}6. Verificando Presets de Mistura${NC}"
echo "──────────────────────────────"
validate_json "resources/profiles/Custom/color_mixing/PLA_MultiColor_Mix.json" "Mistura simples"
validate_json "resources/profiles/Custom/color_mixing/Advanced_Mixing_Preset.json" "Mistura avançada"

echo ""
echo -e "${BLUE}7. Verificando Arquivo Custom.json${NC}"
echo "──────────────────────────────"
if [ -f "resources/profiles/Custom.json" ]; then
    validate_json "resources/profiles/Custom.json" "Configuração principal"
    
    # Verificar se MeuPrinter foi adicionado
    if grep -q "MeuPrinter" "resources/profiles/Custom.json"; then
        echo -e "${GREEN}✓${NC} MeuPrinter foi adicionado ao Custom.json"
        ((SUCCESS++))
    else
        echo -e "${RED}✗${NC} MeuPrinter NÃO foi adicionado ao Custom.json"
        ((ERRORS++))
    fi
else
    echo -e "${RED}✗${NC} Custom.json não encontrado"
    ((ERRORS++))
fi

echo ""
echo -e "${BLUE}8. Verificando Documentação${NC}"
echo "──────────────────────────────"
check_file "MEUPRINTER_README.md" "README principal"
check_file "MEUPRINTER_COLOR_MIXING_GUIDE.md" "Guia de cores (PT)"
check_file "MEUPRINTER_SUMMARY.txt" "Sumário visual"
check_file "build_meuprinter_macos.sh" "Script de build"

echo ""
echo -e "${BLUE}9. Verificando Estrutura de Compatibilidade${NC}"
echo "──────────────────────────────"

# Contar referências a MeuPrinter
MEUPRINTER_REFS=$(grep -r "MeuPrinter" resources/profiles/Custom/ 2>/dev/null | wc -l || echo "0")
echo -e "${GREEN}✓${NC} Encontradas $MEUPRINTER_REFS referências a MeuPrinter"

if [ "$MEUPRINTER_REFS" -gt 20 ]; then
    echo -e "${GREEN}✓${NC} Configuração bem distribuída"
    ((SUCCESS++))
else
    echo -e "${YELLOW}⚠${NC} Poucas referências a MeuPrinter"
    ((WARNINGS++))
fi

echo ""
echo -e "${BLUE}═══════════════════════════════════════${NC}"
echo -e "${BLUE}RESULTADO DA VALIDAÇÃO${NC}"
echo -e "${BLUE}═══════════════════════════════════════${NC}"

echo ""
echo -e "  ${GREEN}✓ Sucessos${NC}: $SUCCESS"
echo -e "  ${YELLOW}⚠ Avisos${NC}: $WARNINGS"
echo -e "  ${RED}✗ Erros${NC}: $ERRORS"

echo ""

if [ $ERRORS -eq 0 ]; then
    echo -e "${GREEN}╔════════════════════════════════════════╗${NC}"
    echo -e "${GREEN}║   ✨ VALIDAÇÃO CONCLUÍDA COM SUCESSO!  ║${NC}"
    echo -e "${GREEN}╚════════════════════════════════════════╝${NC}"
    echo ""
    echo -e "${GREEN}Todos os arquivos foram criados corretamente.${NC}"
    echo -e "Próximo passo: atualizar CMakeLists.txt e compilar"
    echo ""
    echo -e "  ${YELLOW}$ ./build_meuprinter_macos.sh${NC}"
    
    exit 0
else
    echo -e "${RED}╔════════════════════════════════════════╗${NC}"
    echo -e "${RED}║   ✗ VALIDAÇÃO FALHOU!                 ║${NC}"
    echo -e "${RED}╚════════════════════════════════════════╝${NC}"
    echo ""
    echo -e "${RED}$ERRORS erro(s) encontrado(s).${NC}"
    echo -e "Verifique os arquivos listados acima."
    
    exit 1
fi
