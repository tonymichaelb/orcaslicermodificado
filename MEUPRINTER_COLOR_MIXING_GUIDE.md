# 🎨 Guia de Mistura de Cores - OrcaSlicer MeuPrinter

## Visão Geral

O sistema de **Mistura de Cores** (Color Mixing) do OrcaSlicer permite criar configurações avançadas de múltiplos filamentos e cores em uma única impressão.

## 📁 Estrutura de Arquivos

```
resources/profiles/Custom/
├── machine/
│   ├── MeuPrinter.json                    # Definição do modelo base
│   ├── MeuPrinter 0.2 nozzle.json        # Bico 0.2mm
│   ├── MeuPrinter 0.4 nozzle.json        # Bico 0.4mm
│   ├── MeuPrinter 0.6 nozzle.json        # Bico 0.6mm
│   └── MeuPrinter 0.8 nozzle.json        # Bico 0.8mm
├── filament/
│   ├── Generic PLA @MeuPrinter.json      # Perfil PLA
│   ├── Generic PETG @MeuPrinter.json     # Perfil PETG
│   └── Generic ABS @MeuPrinter.json      # Perfil ABS
├── process/
│   ├── 0.12mm Fine @MeuPrinter.json      # Qualidade fina
│   ├── 0.20mm Standard @MeuPrinter.json  # Qualidade padrão
│   └── 0.28mm Draft @MeuPrinter.json     # Qualidade rápida
└── color_mixing/
    ├── PLA_MultiColor_Mix.json            # Preset mistura de cores
    └── Advanced_Mixing_Preset.json        # Preset avançado
```

## 🖥️ Interface Gráfica - "Mistura de Cores"

A aba **Mistura de Cores** está localizada no painel de configurações principal, ao lado de "Impressora", "Filamento" e "Processo".

### Menu Principal

```
┌─────────────────────────────────────────────┐
│ Color Mixing Configuration                  │
├─────────────────────────────────────────────┤
│                                             │
│ Mix Name    │ Filaments        │ Purge Tower│
│ ─────────────────────────────────────────── │
│ Mix 1       │ PLA, PETG        │ Yes       │
│ Mix 2       │ ABS, ASA         │ No        │
│                                             │
│ [Add Mix]  [Remove Mix]                     │
│                                             │
├─────────────────────────────────────────────┤
│ Mixing Settings                             │
│ ☑ Enable Purge Tower                       │
│ Purge Length (mm): [100]                   │
│ Transition Length (mm): [50]               │
│ ☐ Enable Color Smoothing                    │
└─────────────────────────────────────────────┘
```

## 🎯 Como Usar - Passo a Passo

### 1. Adicionar uma Nova Mistura de Cores

1. Abra o OrcaSlicer
2. Clique na aba **"Mistura de Cores"** (Color Mixing)
3. Clique no botão **"Add Mix"** (Adicionar Mistura)
4. Uma nova entrada aparecerá na lista

### 2. Configurar Filamentos

1. Selecione a mistura da lista
2. Escolha os filamentos desejados (ex: PLA + PETG)
3. Defina as percentagens de cada filamento
4. Configure as propriedades de transição

### 3. Definir Torre de Purga

- **Enable Purge Tower**: Ativa/desativa a torre de purga
- **Purge Length**: Comprimento da torre em mm (padrão: 100mm)
- **Transition Length**: Comprimento de transição entre cores (padrão: 50mm)

### 4. Ativar Suavização de Cores (Opcional)

- **Enable Color Smoothing**: Cria transições mais suaves entre cores
- Reduz o banding (listras visíveis) de cores

### 5. Salvar Configuração

1. Após definir as propriedades, clique em **"Save Preset"**
2. Dê um nome descritivo (ex: "PLA-PETG Gradient")
3. A configuração será salva em `resources/profiles/Custom/color_mixing/`

## 📝 Exemplos de Configurações

### Exemplo 1: Mistura Simples PLA + PETG

```json
{
  "id": "mix_001",
  "name": "PLA Red + PETG Yellow",
  "filaments": ["Generic PLA @MeuPrinter", "Generic PETG @MeuPrinter"],
  "percentages": [50.0, 50.0],
  "purge_tower": {
    "enabled": true,
    "length_mm": 150.0
  },
  "transition": {
    "length_mm": 75.0,
    "smoothing": true
  }
}
```

### Exemplo 2: Mistura Avançada com 3 Cores

```json
{
  "id": "mix_003",
  "name": "Tri-Color: PLA, PETG, ABS",
  "filaments": [
    "Generic PLA @MeuPrinter",
    "Generic PETG @MeuPrinter",
    "Generic ABS @MeuPrinter"
  ],
  "percentages": [33.33, 33.33, 33.34],
  "purge_tower": {
    "enabled": true,
    "length_mm": 200.0,
    "width_mm": 60.0
  },
  "transition": {
    "length_mm": 100.0,
    "smoothing": true,
    "smoothing_strength": 0.7
  }
}
```

## ⚙️ Configurações Avançadas

### Modos de Transição

- **Linear**: Transição direta entre cores
- **Gradient**: Transição suave com múltiplos passos
- **Minimal**: Transição mínima (economia de material)

### Estratégia de Mudança de Cor

```
1. Color Change (M600)
   └─ Impressora pausa e aguarda mudança manual

2. Automatic Filament Switch
   └─ Sistema automático de mudança (multi-extrusor)

3. Purge Tower
   └─ Material antigo é purgado em torre separada
   └─ Reduz cores indesejadas na peça
```

### Compensação de Fluxo

- Ajusta a quantidade de material extruso durante a transição
- Evita sub/super-extrusão em mudanças de cor
- Valores: 0.9 - 1.1 (padrão: 1.0)

## 🔧 Configuração do Bico

A MeuPrinter suporta múltiplos tamanhos de bico:

| Bico | Ideal Para | Tempo de Impressão |
|------|------------|-------------------|
| 0.2mm | Detalhes finos | Lento |
| 0.4mm | **Recomendado** | Médio |
| 0.6mm | Impressões rápidas | Rápido |
| 0.8mm | Objetos grandes | Muito rápido |

## 📊 Perfis de Filamento Disponíveis

### MeuPrinter - Filamentos Suportados

1. **Generic PLA @MeuPrinter**
   - Temperatura: 200-210°C
   - Cama: 60°C
   - Suporte: Sim

2. **Generic PETG @MeuPrinter**
   - Temperatura: 230-240°C
   - Cama: 70°C
   - Suporte: Sim

3. **Generic ABS @MeuPrinter**
   - Temperatura: 240-250°C
   - Cama: 80°C
   - Suporte: Sim

## 💡 Dicas e Boas Práticas

### Torre de Purga Eficiente

✅ **Faça:**
- Localize a torre de purga na borda do objeto
- Use comprimento mínimo necessário (100-150mm)
- Ative em modo "material_savings_mode" para economizar

❌ **Evite:**
- Torres muito curtas (< 50mm)
- Colocar torre dentro de área de impressão
- Desativar sem testar antes

### Qualidade de Cor

✅ **Faça:**
- Ative suavização de cores para gradientes
- Use comprimento de transição 50-100mm
- Teste com objeto de amostra primeiro

❌ **Evite:**
- Transições muito curtas (< 20mm)
- Suavização em impressões de alta velocidade
- Mudar de cores sem torre de purga

### Economia de Material

✅ **Faça:**
- Use modo "Eco" para testes
- Considere altura de camada
- Ajuste densidade de infill da torre

❌ **Evite:**
- Torre muito grande
- Múltiplas misturas desnecessárias
- Reiníciar impressão sem necessidade

## 🐛 Troubleshooting

### Problema: Cores indesejadas aparecem no objeto

**Solução:**
1. Aumente o comprimento de purga de 100mm → 150mm
2. Ative suavização de cores
3. Verifique se torre de purga está distante da peça

### Problema: Transição muito brusca entre cores

**Solução:**
1. Aumente "Transition Length" para 75-100mm
2. Ative "Enable Color Smoothing"
3. Reduza velocidade de impressão durante transição

### Problema: Material excessivo saindo

**Solução:**
1. Reduza percentagem de primeira cor
2. Aumente tempo de espera entre cores
3. Verifique configuração de retração

## 📚 Referências Técnicas

### Arquivo de Configuração Global

Local: `Custom.json`

```json
{
  "name": "Custom Printer",
  "machine_model_list": [...],
  "filament_list": [...],
  "process_list": [...]
}
```

### Estrutura de Preset de Mistura

```json
{
  "type": "color_mixing",
  "name": "Mix Name",
  "compatible_printers": ["MeuPrinter 0.4 nozzle"],
  "mixes": [{...}],
  "global_settings": {...}
}
```

## 🔗 Links Úteis

- **Documentação OrcaSlicer**: https://github.com/SoftFever/OrcaSlicer
- **Guia de Impressão Multi-Color**: [em desenvolvimento]
- **Comunidade**: Discord/Forum OrcaSlicer

## 📝 Histórico de Versões

- **v1.0** (Inicial)
  - Suporte básico de mistura de cores
  - Interface gráfica com lista de misturas
  - Perfis PLA, PETG, ABS para MeuPrinter
  
- **v1.1** (Planejado)
  - Suporte a gradientes de cores
  - Integração com G-code avançado
  - Suporte a múltiplos extrusores

---

**Desenvolvido para OrcaSlicer com MeuPrinter**  
Última atualização: 2024
