# 🎨 MeuPrinter - Sistema Customizado de Impressora 3D

> Versão customizada do OrcaSlicer com suporte completo para a impressora **MeuPrinter** e novo menu **"Mistura de Cores"**

## 📋 Resumo do Projeto

Este projeto implementa uma **nova impressora customizada chamada "MeuPrinter"** no OrcaSlicer com:

✅ **Configuração de Máquina** (Hardware)
✅ **Perfis de Filamento** (Materiais)
✅ **Processos de Impressão** (Qualidade)
✅ **Menu "Mistura de Cores"** (GUI - Interface Gráfica)

---

## 🗂️ Estrutura de Arquivos Criados

```
OrcaSlicer-main/
├── src/slic3r/GUI/
│   ├── ColorMixing.hpp              ← Cabeçalho do componente
│   ├── ColorMixing.cpp              ← Implementação do componente
│   └── ColorMixing_Integration.md   ← Guia de integração
│
├── resources/profiles/
│   ├── Custom.json                  ← Configuração atualizada
│   └── Custom/
│       ├── machine/
│       │   ├── MeuPrinter.json                    ← Modelo base
│       │   ├── MeuPrinter 0.2 nozzle.json       ← Bico fino
│       │   ├── MeuPrinter 0.4 nozzle.json       ← Bico padrão
│       │   ├── MeuPrinter 0.6 nozzle.json       ← Bico médio
│       │   └── MeuPrinter 0.8 nozzle.json       ← Bico grosso
│       ├── filament/
│       │   ├── Generic PLA @MeuPrinter.json     ← Filamento PLA
│       │   ├── Generic PETG @MeuPrinter.json    ← Filamento PETG
│       │   └── Generic ABS @MeuPrinter.json     ← Filamento ABS
│       ├── process/
│       │   ├── 0.12mm Fine @MeuPrinter.json     ← Qualidade fina
│       │   ├── 0.20mm Standard @MeuPrinter.json ← Qualidade padrão
│       │   └── 0.28mm Draft @MeuPrinter.json    ← Qualidade rápida
│       └── color_mixing/
│           ├── PLA_MultiColor_Mix.json           ← Mistura simples
│           └── Advanced_Mixing_Preset.json       ← Mistura avançada
│
├── build_meuprinter_macos.sh         ← Script de build customizado
├── MEUPRINTER_COLOR_MIXING_GUIDE.md  ← Guia em português
└── MEUPRINTER_README.md              ← Este arquivo
```

---

## 🚀 Inicio Rápido

### 1️⃣ Compilar o Projeto

```bash
# Opção A: Usar o script de build customizado
chmod +x build_meuprinter_macos.sh
./build_meuprinter_macos.sh

# Opção B: Build manual (caso precise)
cd build/arm64
cmake --build . --config RelWithDebInfo --target OrcaSlicer -- -j$(sysctl -n hw.ncpu)
```

### 2️⃣ Executar o OrcaSlicer

```bash
./build/arm64/src/OrcaSlicer.app/Contents/MacOS/OrcaSlicer
```

### 3️⃣ Usar a MeuPrinter

1. Abra OrcaSlicer
2. No painel **"Impressora"**, selecione **"MeuPrinter"**
3. Escolha o tamanho de bico: 0.2mm, 0.4mm, 0.6mm, ou 0.8mm
4. Selecione o filamento: PLA, PETG, ou ABS
5. Escolha o processo: Fine (fina), Standard (padrão), ou Draft (rápida)

### 4️⃣ Usar "Mistura de Cores"

1. Clique na aba **"Mistura de Cores"** (Color Mixing) - ⚠️ Requer integração adicional no CMakeLists.txt
2. Clique **"Add Mix"** para criar nova mistura
3. Configure os filamentos, torre de purga e transição
4. Salve como preset

---

## 📊 Especificações da MeuPrinter

### Modelo Base

| Propriedade | Valor |
|------------|-------|
| Tipo | FFF (Fused Filament Fabrication) |
| Tecnologia | Multi-Bico (0.2mm, 0.4mm, 0.6mm, 0.8mm) |
| Modelo ID | `meu_printer_01` |
| Família | MeuPrinter |

### Tamanhos de Bico Suportados

| Bico | Arquivo | Uso |
|------|---------|-----|
| **0.2mm** | `MeuPrinter 0.2 nozzle.json` | Detalhes muito finos |
| **0.4mm** | `MeuPrinter 0.4 nozzle.json` | Padrão (recomendado) |
| **0.6mm** | `MeuPrinter 0.6 nozzle.json` | Impressão mais rápida |
| **0.8mm** | `MeuPrinter 0.8 nozzle.json` | Objetos grandes |

### Filamentos Disponíveis

1. **Generic PLA @MeuPrinter** ✓
   - Temperatura do Nozzle: 200-210°C
   - Temperatura da Cama: 60°C
   - Velocidade de Carregamento: 10 mm/s

2. **Generic PETG @MeuPrinter** ✓
   - Temperatura do Nozzle: 230-240°C
   - Temperatura da Cama: 70°C
   - Velocidade de Carregamento: 8 mm/s

3. **Generic ABS @MeuPrinter** ✓
   - Temperatura do Nozzle: 240-250°C
   - Temperatura da Cama: 80°C
   - Velocidade de Carregamento: 8 mm/s

### Processos de Impressão

| Nome | Altura de Camada | Aplicação | Arquivo |
|------|-----------------|-----------|---------|
| **Fine** | 0.12mm | Detalhes finos | `0.12mm Fine @MeuPrinter.json` |
| **Standard** | 0.20mm | Uso geral | `0.20mm Standard @MeuPrinter.json` |
| **Draft** | 0.28mm | Impressão rápida | `0.28mm Draft @MeuPrinter.json` |

---

## 🎨 Sistema de Mistura de Cores

### Componentes GUI Criados

#### 1. **ColorMixing.hpp** - Definição
```cpp
// Estrutura de configuração de mistura
struct ColorMixingConfig {
    std::string name;
    std::vector<std::string> filaments;
    std::vector<float> percentages;
    float purgeLength;
    float transitionLength;
    bool enablePurgeTower;
    bool enableSmoothing;
};

// Painel da interface gráfica
class ColorMixingPanel : public wxPanel {
    // ... métodos para gerenciar misturas
};
```

#### 2. **ColorMixing.cpp** - Implementação
- ✅ Adicionar/remover misturas de cores
- ✅ Selecionar múltiplos filamentos
- ✅ Configurar torre de purga
- ✅ Ajustar comprimento de transição
- ✅ Ativar/desativar suavização de cores

#### 3. **ColorMixing_Integration.md** - Guia de Integração
Instruções passo a passo para integrar o componente no GUI do OrcaSlicer

---

## 📝 Perfis de Exemplo

### Exemplo 1: Mistura Simples

**Arquivo**: `resources/profiles/Custom/color_mixing/PLA_MultiColor_Mix.json`

```json
{
  "mixes": [
    {
      "name": "Red-Yellow Gradient",
      "filaments": ["Generic PLA @MeuPrinter", "Generic PETG @MeuPrinter"],
      "percentages": [50.0, 50.0],
      "purge_tower": {"enabled": true, "length_mm": 150.0},
      "transition": {"length_mm": 75.0, "smoothing": true}
    }
  ]
}
```

### Exemplo 2: Mistura Avançada

**Arquivo**: `resources/profiles/Custom/color_mixing/Advanced_Mixing_Preset.json`

Contém 3 presets:
- ✓ Smooth Color Gradient (Gradiente Suave)
- ✓ Fast Color Change (Mudança Rápida)
- ✓ Eco Mode (Modo Econômico)

---

## 🔄 Fluxo de Configuração

```
Seleção de Impressora
        ↓
    MeuPrinter
        ↓
    ┌─────────────────┐
    │  Tamanho Bico   │
    │  (0.2/0.4/etc)  │
    └─────────────────┘
        ↓
    ┌─────────────────┐
    │   Filamento     │
    │  (PLA/PETG/ABS) │
    └─────────────────┘
        ↓
    ┌─────────────────┐
    │    Processo     │
    │ (Fine/Std/Fast) │
    └─────────────────┘
        ↓
    [OPCIONAL]
    Mistura de Cores
    └─ Torre de Purga
    └─ Transição de Cores
    └─ Suavização
```

---

## 🛠️ Próximos Passos para Integração Completa

Para que o menu **"Mistura de Cores"** apareça na interface, são necessários:

### 1. Atualizar CMakeLists.txt

```cmake
# src/slic3r/GUI/CMakeLists.txt
set(SLIC3R_GUI_SOURCES
    ...
    ColorMixing.hpp
    ColorMixing.cpp
    ...
)
```

### 2. Criar Tab ColorMixing em Tab.hpp/cpp

```cpp
namespace Slic3r::GUI {
    class Tab::ColorMixing : public Tab {
        void build() override;
        ColorMixingPanel* m_color_mixing_panel;
    };
}
```

### 3. Integrar à MainFrame

```cpp
// Adicionar aba ao notebook
m_tabs[Preset::TYPE_COLOR_MIXING] = new Tab::ColorMixing(m_notebook);
m_notebook->AddPage(..., "Color Mixing");
```

### 4. Atualizar Preset System

Adicionar suporte a tipo `TYPE_COLOR_MIXING` no sistema de presets

---

## 🧪 Testes Recomendados

### Teste 1: Seleção de Máquina
- [ ] MeuPrinter aparece na lista de impressoras
- [ ] Tamanhos de bico mudam corretamente (0.2/0.4/0.6/0.8)

### Teste 2: Filamentos
- [ ] PLA @MeuPrinter é compatível com todos os bicos
- [ ] PETG é compatível com bicos ≥ 0.4mm
- [ ] ABS é compatível com todos os bicos

### Teste 3: Processos
- [ ] Fine (0.12mm) está disponível para bicos 0.2 e 0.4mm
- [ ] Standard (0.20mm) está disponível para todos
- [ ] Draft (0.28mm) está disponível para bicos ≥ 0.4mm

### Teste 4: Mistura de Cores (Após Integração)
- [ ] Aba "Mistura de Cores" aparece no painel
- [ ] Botão "Add Mix" adiciona nova mistura
- [ ] Configurações de torre de purga funcionam
- [ ] Suavização de cores alterna

---

## 📚 Documentação Completa

| Documento | Localização | Descrição |
|-----------|------------|-----------|
| **Guia de Cores** | `MEUPRINTER_COLOR_MIXING_GUIDE.md` | Guia completo em português |
| **Integração GUI** | `src/slic3r/GUI/ColorMixing_Integration.md` | Como integrar o componente |
| **Este README** | `MEUPRINTER_README.md` | Visão geral geral do projeto |

---

## 🐛 Troubleshooting

### Problema: MeuPrinter não aparece

**Solução:**
1. Verifique se `Custom.json` foi atualizado
2. Reinicie o OrcaSlicer
3. Verifique permissões dos arquivos JSON

### Problema: Filamentos incompatíveis

**Solução:**
1. Verifique `compatible_printers` no JSON do filamento
2. Certifique-se que nomes correspondem exatamente
3. Exemplo correto: `"MeuPrinter 0.4 nozzle"`

### Problema: Compilação falha com ColorMixing

**Solução:**
1. Adicione `ColorMixing.hpp` e `ColorMixing.cpp` ao CMakeLists.txt
2. Inclua `#include "ColorMixing.hpp"` nos arquivos necessários
3. Compile: `cmake --build build/arm64 --config RelWithDebInfo`

---

## 📜 Créditos

- **Base**: OrcaSlicer (https://github.com/SoftFever/OrcaSlicer)
- **Impressora**: MeuPrinter (Customizada)
- **Sistema de Cores**: Novo componente GUI Color Mixing
- **Configuração**: Perfis JSON customizados

---

## 📄 Licença

Este projeto segue a licença do OrcaSlicer. Veja LICENSE.txt para detalhes.

---

## 🔗 Referências Úteis

- **Documentação OrcaSlicer**: https://github.com/SoftFever/OrcaSlicer/wiki
- **Perfis de Impressoras**: `resources/profiles/`
- **Sistema de Presets**: `src/libslic3r/Preset.hpp`

---

## ✨ Status do Projeto

| Componente | Status | Notas |
|-----------|--------|-------|
| Machine Profile | ✅ Completo | MeuPrinter com 4 bicos |
| Filament Profiles | ✅ Completo | PLA, PETG, ABS |
| Process Profiles | ✅ Completo | Fine, Standard, Draft |
| Color Mixing GUI | ⏳ Pronto para Integração | Código escrito, precisa CMakeLists.txt |
| Color Mixing Logic | ✅ Completo | Validação de misturas |
| Documentação | ✅ Completo | Guias em português |

---

## 🎯 Roadmap Futuro

- [ ] Integração completa de ColorMixing no CMakeLists.txt
- [ ] Suporte a gradientes de cores personalizados
- [ ] Preview de mistura de cores em tempo real
- [ ] Otimização automática de torre de purga
- [ ] Exportação de G-code com mistura de cores
- [ ] Suporte a multi-extrusor

---

**Última Atualização**: 2024  
**Versão**: 1.0  
**Status**: Funcional para máquina e filamentos

---

Para dúvidas ou sugestões, consulte a documentação detalhada em `MEUPRINTER_COLOR_MIXING_GUIDE.md`
